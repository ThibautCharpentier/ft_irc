/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeKey.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 16:07:06 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/02 16:07:06 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/commands/Command.hpp"

static void setMode(Channel *channel, Client *client, std::string password, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find('k') == std::string::npos)
        channel->addMode('k');
    channel->setPassword(password);
    for (it_member = member.begin(); it_member != member.end(); it_member++)
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " +k " << password;
	    send_to_client_things(output.str(), it_member->first, server);
    }
}

static void    deleteMode(Channel *channel, Client *client, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find('k') == std::string::npos)
        return ;
    channel->unsetMode('k');
    channel->setPassword("");
    while (it_member != member.end())
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " -k ";
	    send_to_client_things(output.str(), it_member->first, server);
        it_member++;
    }
}

void    modeKey(const std::string &target, Client *client, Server *server,const std::string &mode,const std::string &arg)
{
    std::map<std::string, Channel *>                    all_channels = server->getTabChannels();
    std::map<std::string, Channel *>::const_iterator    it_chan = all_channels.find(target);
    
    if (mode[0] == '+')
    {
        if (arg.empty())
        {
            E461(client->getFd(), client->getNickname(), server, "MODE " + mode);
            return ;
        }
        for (int i = 0; arg[i] != '\0'; i++)
        {
            if (!isalpha(arg[i]) && !isdigit(arg[i]))
            {
                E525(client->getFd(), client->getNickname(), server, target);
                return ;
            }
        }
        setMode(it_chan->second, client, arg, server);
    }
    else if (mode[0] == '-')
        deleteMode(it_chan->second, client, server);
}
