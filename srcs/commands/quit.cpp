/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 16:28:44 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/26 16:28:44 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void	quit(const std::string &message, Client *client, Server *server)
{
	std::vector<std::string>	name_chan;
	std::stringstream			output;
	std::string					msg;
	size_t						i = 0;

	while (message.c_str()[i] != '\0' && message.c_str()[i] != ':')
		i++;
	if (message.c_str()[i] == '\0')
		msg = "";
	else
		msg = message.substr(i + 1);
	for (std::map<std::string, Channel *>::const_iterator it = server->getTabChannels().begin(); it != server->getTabChannels().end(); it++)
		name_chan.push_back(it->first);
	for (std::vector<std::string>::iterator it_chan = name_chan.begin(); it_chan != name_chan.end(); it_chan++)
	{
		Channel	*channel = server->isChannel(*it_chan);
		if (channel->lastClient(client))
			server->delChannel(*it_chan, channel);
		else if (channel->isInChannel(client))
		{
			output.str("");
			output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
				<< " QUIT " << msg;
			for (std::map<int, Client *>::const_iterator it_client = channel->getTabJoined().begin(); it_client != channel->getTabJoined().end(); it_client++)
			{
				if (it_client->first != client->getFd())
					send_to_client_things(output.str(), it_client->first, server);
			}
			channel->delJoined(client);
		}
	}
}
