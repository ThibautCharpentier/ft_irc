/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 12:49:41 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/01 12:49:41 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static void	sendToAllClientInChannel(Channel *channel, Client *client, std::string msg, Server *server)
{
	std::stringstream	output;

	output.str("");
	output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
		<< " PRIVMSG " << channel->getName() << " " << msg;
	for (std::map<int, Client *>::const_iterator it = channel->getTabJoined().begin(); it != channel->getTabJoined().end(); it++)
	{
		if (it->first != client->getFd())
			send_to_client_things(output.str(), it->first, server);
	}
}

static void	sendToAllOpInChannel(Channel *channel, Client *client, std::string msg, Server *server)
{
	std::stringstream	output;

	output.str("");
	output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
		<< " PRIVMSG " << channel->getName() << " " << msg;
	for (std::map<int, Client *>::const_iterator it = channel->getTabJoined().begin(); it != channel->getTabJoined().end(); it++)
	{
		if (it->first != client->getFd() && channel->isOperator(it->second->getUsername()))
			send_to_client_things(output.str(), it->first, server);
	}
}

static void	sendToChannel(std::string channel, Client *client, Server *server, std::string msg, bool op)
{
	for (std::map<std::string, Channel *>::const_iterator it = server->getTabChannels().begin(); it != server->getTabChannels().end(); it++)
	{
		if (it->first == channel)
		{
			if (op == false)
				sendToAllClientInChannel(it->second, client, msg, server);
			else
				sendToAllOpInChannel(it->second, client, msg, server);
			return ;
		}
	}
	E401(client->getFd(), client->getNickname(), server, channel);
}

static void	sendToClient(std::string nickname, Client *client, Server *server, std::string msg)
{
	std::stringstream	output;

	for (std::map<int, Client *>::const_iterator it = server->getTabClients().begin(); it != server->getTabClients().end(); it++)
	{
		if (it->second->getNickname() == nickname)
		{
			output.str("");
			output << clientId(client->getNickname(), client->getUsername(), client->getHostname())
				<< " PRIVMSG " << nickname << " " << msg;
			send_to_client_things(output.str(), it->first, server);
			return ;
		}
	}
	E401(client->getFd(), client->getNickname(), server, nickname);
}

void    privmsg(const std::string &message, Client *client, Server *server)
{
	std::vector<std::string>	args;
	size_t						i = 0;
	std::string					msg;

	args = parseArgs(message, &i);
	if (args.empty())
	{
        E411(client->getFd(), client->getNickname(), server, "PRIVMSG");
		return ;
	}
	while (message.c_str()[i] == ' ')
		i++;
	if (message.c_str()[i] == ':')
		msg = message.substr(i + 1);
	else
	{
		E412(client->getFd(), client->getNickname(), server);
		return ;
	}
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		if (it->find('#') == 0 || it->find('&') == 0)
			sendToChannel(*it, client, server, msg, false);
		else if (it->find('@') == 0 && (it->find('#') == 1 || it->find('&') == 1))
			sendToChannel(it->substr(1), client, server, msg, true);
		else
			sendToClient(*it, client, server, msg);
	}
}
