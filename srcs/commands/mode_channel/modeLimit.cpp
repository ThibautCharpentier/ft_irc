/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeLimit.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 20:20:10 by igeorge           #+#    #+#             */
/*   Updated: 2023/08/01 20:20:10 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/commands/Command.hpp"

static void setMode(Channel *channel, Client *client, long limit, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find('l') == std::string::npos)
        channel->addMode('l');
    channel->setLimit(limit);
    for (it_member = member.begin(); it_member != member.end(); it_member++)
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " +l " << limit;
	    send_to_client_things(output.str(), it_member->first, server);
    }
}

void    deleteMode(Channel *channel, Client *client, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find('l') == std::string::npos)
        return ;
    channel->unsetMode('l');
    channel->setLimit(0);
    while (it_member != member.end())
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " -l ";
	    send_to_client_things(output.str(), it_member->first, server);
        it_member++;
    }
}

void    modeLimit(const std::string &target, Client *client, Server *server,const std::string &mode, const std::string &arg)
{
    std::map<std::string, Channel *>                    all_channels = server->getTabChannels();
    std::map<std::string, Channel *>::const_iterator    it_chan = all_channels.find(target);
    long                                                 limit;
    
    if (mode[0] == '+')
    {
        if (arg.empty())
        {
            E461(client->getFd(), client->getNickname(), server, "MODE " + mode);
            return ;
        }
        for (int i = 0; arg[i] != '\0'; i++)
        {
            if (!isdigit(arg[i]))
            {
                E696(client->getFd(), client->getNickname(), server, target, mode, arg);
                return ;
            }
        }
        limit = atol(arg.c_str());
        setMode(it_chan->second, client, limit, server);
    }
    else if (mode[0] == '-')
        deleteMode(it_chan->second, client, server);
}
