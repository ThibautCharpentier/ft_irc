/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeInvite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 13:09:02 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/01 13:09:02 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/commands/Command.hpp"

static void    setMode(Channel *channel, Client *client, char letter_mode, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find(letter_mode) != std::string::npos)
        return ;
    channel->addMode(letter_mode);
    while (it_member != member.end())
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " +" << letter_mode;
	    send_to_client_things(output.str(), it_member->first, server);
        it_member++;
    }
}

static void    deleteMode(Channel *channel, Client *client, char letter_mode, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    if (mode_chan.find(letter_mode) == std::string::npos)
        return ;
    channel->unsetMode(letter_mode);
    while (it_member != member.end())
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " -" << letter_mode;
	    send_to_client_things(output.str(), it_member->first, server);
        it_member++;
    }
}

void    modeInviteOrTopic(const std::string &target, Client *client, Server *server, const std::string &mode)
{
    std::map<std::string, Channel *>                  all_channels = server->getTabChannels();
    std::map<std::string, Channel *>::const_iterator  it_chan = all_channels.find(target);
    
    if (mode[0] == '+')
        setMode(it_chan->second, client, mode[1], server);
    else if (mode[0] == '-')
        deleteMode(it_chan->second, client, mode[1], server);
}
