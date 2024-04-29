/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 12:47:48 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/30 12:47:48 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void    invite(const std::string &message, Client *client, Server *server)
{
	std::string			nickname;
	std::string			channel_name;
	Channel				*channel;
	Client				*target;
	std::stringstream	output;

	nickname = parseFirstWord(message);
	if (nickname == "")
	{
		E461(client->getFd(), client->getNickname(), server, "INVITE");
		return ;
	}
	channel_name = parseFirstWord(message.substr(message.find(nickname) + nickname.size()));
	if (channel_name == "")
	{
		E461(client->getFd(), client->getNickname(), server, "INVITE");
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
	else if (channel->getModes().find('i') != std::string::npos
		&& !channel->isOperator(client->getUsername()))
		E482(client->getFd(), client->getNickname(), server, channel_name);
	else if (channel->isInChannel(target))
		E443(client->getFd(), client->getNickname(), server, channel_name, nickname);
	else
	{
		if (!channel->isInInvited(target->getUsername()))
			channel->addInvited(target->getUsername());
		R341(client->getFd(), client->getNickname(), server, channel_name, nickname);
		output.str("");
		output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
			<< " INVITE " << nickname << " " << channel_name;
		send_to_client_things(output.str(), target->getFd(), server);
	}
}
