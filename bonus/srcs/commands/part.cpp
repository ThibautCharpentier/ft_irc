/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:25:25 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/27 13:25:25 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void	part(const std::string &message, Client *client, Server *server)
{
	std::vector<std::string>							channels;
	size_t												i = 0;
	std::map<std::string, Channel *>::const_iterator	it_map;
	std::stringstream									output;
	std::string											msg;

	channels = parseChannels(message, &i);
	if (channels.empty())
	{
        E461(client->getFd(), client->getNickname(), server, "PART");
		return ;
	}
	while (message.c_str()[i] == ' ')
		i++;
	if (message.c_str()[i] == ':')
		msg = message.substr(i + 1);
	else
		msg = "";
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		it_map = server->getTabChannels().find(*it);
		if (it_map != server->getTabChannels().end())
		{
			Channel		*channel = it_map->second;

			if (channel->lastClient(client))
			{
				output.str("");
				output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
					<< " PART " << channel->getName() << " " << msg;
				send_to_client_things(output.str(), client->getFd(), server);
				server->delChannel(it_map->second->getName(), it_map->second);
				server->displayServerMessage();
			}
			else
			{
				if (channel->isInChannel(client))
				{
					output.str("");
					output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
					<< " PART " << channel->getName() << " " << msg;
					for (std::map<int, Client *>::const_iterator it_client = channel->getTabJoined().begin(); it_client != channel->getTabJoined().end(); it_client++)
							send_to_client_things(output.str(), it_client->first, server);
					channel->delJoined(client);
				}
				else
					E442(client->getFd(), client->getNickname(), server, channel->getName());
			}
		}
		else
			E403(client->getFd(), client->getNickname(), server, *it);
	}
}
