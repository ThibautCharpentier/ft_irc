/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:43:41 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 20:43:41 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static int respectLMode(std::string mode, Channel *channel, Client *client, Server *server)
{
    if (mode.find('l') != std::string::npos)
    {
        if ((int)channel->getTabJoined().size() >= channel->getLimit())
        {
            E471(client->getFd(), client->getNickname(), server, channel->getName());
            return (0);
        }
    }
    return (1);
}

static int respectIMode(std::string mode, Channel *channel, Client *client, Server *server)
{
    if (mode.find('i') != std::string::npos)
    {
        if (!channel->isInInvited(client->getUsername()))
        {
            E473(client->getFd(), client->getNickname(), server, channel->getName());
            return (0);
        }
    }
    return (1);
}

static int respectKMode(std::string mode, Channel *channel, Client *client, std::vector<std::string>::iterator password, std::vector<std::string> keys, Server *server)
{
    if (mode.find('k') != std::string::npos)
    {
        std::string channel_pass = channel->getPassword();
        if (password == keys.end() || channel_pass != *password)
        {
            E475(client->getFd(), client->getNickname(), server, channel->getName());
            return (0);
        }
    }
    return (1);
}

static std::string    getListofMembers(Channel *channel)
{
    std::map<int, Client *>             clients_joined = channel->getTabJoined();
    std::map<int, Client *>::iterator   it;
    std::string                         list;

    for (it = clients_joined.begin(); it != clients_joined.end(); it++)
    {
        if (channel->isOperator(it->second->getUsername()))
            list += '@';
        list += it->second->getNickname() + ' ';
    }
    return (list);
}

static void    sendChanInfos(Channel *channel, Client *client, Server *server)
{
    std::string                             nick = client->getNickname();
    std::string                             username = client->getUsername();
    int                                     fd = client->getFd();
    std::map<int, Client *>::const_iterator it = channel->getTabJoined().begin();
    std::stringstream	                    output;
    std::string                             members;

	output << clientId(nick, username, client->getHostname()) << " JOIN " << channel->getName();
	send_to_client_things(output.str(), fd, server);
    members = getListofMembers(channel);
    if (!channel->getTopic().empty())
        R332(fd, nick, server, channel->getName(), channel->getTopic());
    output.str("");
    R353(fd, nick, server, '=', channel->getName(), members);
    R366(fd, nick, server, channel->getName());
    while (it != channel->getTabJoined().end())
    {
        nick = it->second->getNickname();
        username = it->second->getUsername();
        fd = it->second->getFd();
        if (fd != client->getFd())
        {
            output.str("");
	        output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " JOIN " << channel->getName();
	        send_to_client_things(output.str(), fd, server);
        }
        it++;
    }
}

void   join(const std::string &message, Client *client, Server *server)
{
    std::vector<std::string>    channels;
    std::vector<std::string>    keys;
    size_t                      i = 0;

    channels = parseChannels(message, &i);
    if (channels.empty())
    {
        E461(client->getFd(), client->getNickname(), server, "JOIN");
        return ;
    }
    keys = parseArgs(message, &i);

    std::vector<std::string>::iterator it_key = keys.begin();
    std::vector<std::string>::iterator it_chan = channels.begin();
    std::map<std::string, Channel *> all_channels = server->getTabChannels();
    while (it_chan != channels.end())
    {
        std::map<std::string, Channel *>::iterator it_all_chan = all_channels.find(*it_chan);
        if (it_all_chan == all_channels.end())
        {
            Channel *new_channel = new (std::nothrow) Channel(*it_chan, client);
            if (!new_channel)
                throw std::runtime_error("Error: operator new");
            server->insertChannel(*it_chan, new_channel);
            sendChanInfos(new_channel, client, server);
            server->displayServerMessage();
            it_chan++;
            if (it_key != keys.end())
                it_key++;
        }
        else
        {
            Channel *this_channel = it_all_chan->second;
            std::string mode = this_channel->getModes();
            if (!respectLMode(mode, this_channel, client, server))
            {
                it_chan++;
                if (it_key != keys.end())
                    it_key++;
                continue ;
            }
            else if (!respectKMode(mode, this_channel, client, it_key, keys, server))
            {
                it_chan++;
                if (it_key != keys.end())
                    it_key++;
                continue ;
            }
            else if (!respectIMode(mode, this_channel, client, server))
            {
                it_chan++;
                if (it_key != keys.end())
                    it_key++;
                continue ;
            }
            this_channel->addJoined(client);
            if (this_channel->isInInvited(client->getUsername()))
                this_channel->delInvited(client->getUsername());
            sendChanInfos(this_channel, client, server);
            it_chan++;
            it_key++;
        }
    }
}
