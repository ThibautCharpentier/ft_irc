/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeOperator.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 17:38:19 by igeorge           #+#    #+#             */
/*   Updated: 2023/08/01 17:38:19 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/commands/Command.hpp"

static void    setMode(Channel *channel, Client *client, std::string arg, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    while (it_member != member.end())
    {
        if (it_member->second->getNickname() == arg)
            break ;
        it_member++;
    }
    if (it_member == member.end())
    {
        E441(client->getFd(), client->getNickname(), server, channel->getName(), arg);
        return ;
    }
    if (channel->isOperator(it_member->second->getUsername()))
        return ;
    channel->addOperator(it_member->second->getUsername());
    for (it_member = member.begin(); it_member != member.end(); it_member++)
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " +o " << arg;
	    send_to_client_things(output.str(), it_member->first, server);
    }
}

static void deleteMode(Channel *channel, Client *client, std::string arg, Server *server)
{
    std::string                             mode_chan = channel->getModes();
    std::vector<std::string>                operators = channel->getTabOperator();
    std::map<int, Client *>                 member = channel->getTabJoined();
    std::map<int, Client *>::const_iterator it_member = member.begin();
    std::stringstream	                    output;

    while (it_member != member.end())
    {
        if (it_member->second->getNickname() == arg)
            break ;
        it_member++;
    }
    if (it_member == member.end())
    {
        E441(client->getFd(), client->getNickname(), server, channel->getName(), arg);
        return ;
    }
    if (!(channel->isOperator(it_member->second->getUsername())))
        return ;
    if (it_member->second->getUsername() == *(operators.begin()))
        return ;
    channel->deleteOperator(it_member->second->getUsername());
    for (it_member = member.begin(); it_member != member.end(); it_member++)
    {
        output.str("");
	    output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " MODE " << channel->getName() << " -o " << arg;
	    send_to_client_things(output.str(), it_member->first, server);
    }
}

void    modeOperator(const std::string &target, Client *client, Server *server, const std::string &mode, const std::string &arg)
{
    std::map<std::string, Channel *>                  all_channels = server->getTabChannels();
    std::map<std::string, Channel *>::const_iterator  it_chan = all_channels.find(target);
    
    if (arg.empty())
    {
        E461(client->getFd(), client->getNickname(), server, "MODE " + mode);
        return ;
    }
    if (mode[0] == '+')
        setMode(it_chan->second, client, arg, server);
    else if (mode[0] == '-')
        deleteMode(it_chan->second, client, arg, server);
}
