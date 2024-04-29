/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 11:59:26 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/28 11:59:26 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static void	displayTopic(Client *client, Channel *channel, Server *server)
{
	if (channel->getTopic() == "")
		R331(client->getFd(), client->getNickname(), server, channel->getName());
	else
		R332(client->getFd(), client->getNickname(), server, channel->getName(), channel->getTopic());
}

static void	changeTopic(Client *client, Channel *channel, std::string &topic, Server *server)
{
	if (channel->getModes().find('t') != std::string::npos
		&& !channel->isOperator(client->getUsername()))
		E482(client->getFd(), client->getNickname(), server, channel->getName());
	else
	{
		if (topic.find(':') != 0)
		{
			E461(client->getFd(), client->getNickname(), server, "TOPIC");
			return ;
		} 
		topic = topic.substr(1);
		channel->setTopic(topic);
		for (std::map<int, Client *>::const_iterator it = channel->getTabJoined().begin(); it != channel->getTabJoined().end(); it++)
			R332(it->first, it->second->getNickname(),server,  channel->getName(), topic);
	}
}

void	topic(const std::string &message, Client *client, Server *server)
{
	std::string	channel_name;
	std::string	topic;
	Channel		*channel;

	channel_name = parseFirstWord(message);
	if (channel_name == "")
	{
		E461(client->getFd(), client->getNickname(), server, "TOPIC");
		return ;
	}
	channel = server->isChannel(channel_name);
	if (!channel || !channel->isInChannel(client))
	{
		E442(client->getFd(), client->getNickname(), server, channel_name);
		return ;
	}
	topic = eraseEarlySpaces(message.substr(channel_name.size()));
	if (topic == "")
		displayTopic(client, channel, server);
	else
		changeTopic(client, channel, topic, server);
}
