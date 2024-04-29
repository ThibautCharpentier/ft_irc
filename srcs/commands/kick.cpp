/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 13:25:25 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/27 13:25:25 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void    kick(const std::string &message, Client *client, Server *server)
{
	std::string			channel_name;
	std::string			nickname;
	std::string			msg;
	Channel				*channel;
	Client				*target;
	std::stringstream	output;
	size_t				i;

	channel_name = parseFirstWord(message);
	if (channel_name == "")
	{
		E461(client->getFd(), client->getNickname(), server, "INVITE");
		return ;
	}
	msg = message.substr(message.find(channel_name) + channel_name.size());
	nickname = parseFirstWord(msg);
	if (nickname == "")
	{
		E461(client->getFd(), client->getNickname(), server,"INVITE");
		return ;
	}
	target = server->isClient(nickname);
	if (!target)
	{
		E401(client->getFd(), client->getNickname(), server, nickname);
		return ;
	}
	channel = server->isChannel(channel_name);
	if (!channel)
		E403(client->getFd(), client->getNickname(), server, channel_name);
	else if (!channel->isInChannel(client))
		E442(client->getFd(), client->getNickname(), server, channel_name);
	else if (!channel->isOperator(client->getUsername()))
		E482(client->getFd(), client->getNickname(), server, channel_name);
	else if (!channel->isInChannel(target))
		E441(client->getFd(), client->getNickname(), server, channel_name, nickname);
	else if (target->getUsername() != *(channel->getTabOperator().begin()))
	{
		msg = msg.substr(msg.find(nickname) + nickname.size());
		i = msg.find(':');
		if (i == std::string::npos)
			msg = "";
		else
			msg = msg.substr(i + 1);
		output.str("");
		output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
			<< " KICK " << channel_name << " " << nickname << " " << msg;
		for (std::map<int, Client *>::const_iterator it_client = channel->getTabJoined().begin(); it_client != channel->getTabJoined().end(); it_client++)
			send_to_client_things(output.str(), it_client->first, server);
		channel->delJoined(target);
	}
}
