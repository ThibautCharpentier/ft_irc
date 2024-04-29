/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:14:57 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/29 18:14:57 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static int  errorCommand(std::string target, Client *client, Server *server, std::string mode, std::string param)
{
    std::map<std::string, Channel *>            all_channels = server->getTabChannels();
    std::map<std::string, Channel *>::const_iterator  it_all_chan = all_channels.find(target);
    Channel                                     *channel;
    std::map<int, Client *>::const_iterator     it_client;
    std::vector<std::string> ::const_iterator   it_operator;

    if (it_all_chan == all_channels.end())  
    {
        E403(client->getFd(), client->getNickname(), server, target);
        return (1);
    }
    channel = it_all_chan->second;
    it_client = channel->getTabJoined().find(client->getFd());
    if (it_client == channel->getTabJoined().end())
    {
        E442(client->getFd(), client->getNickname(), server, target);
        return (1);
    }
    it_operator = channel->getTabOperator().begin();
    while (it_operator != channel->getTabOperator().end())
    {
        if (*it_operator == client->getUsername())
            break ;
        else
            it_operator++;
    }
    if (mode.empty() || (mode[0] != '+' && mode[0] != '-'))
    {
        if (!param.empty())
        {
            mode.push_back(' ');
            mode = mode + param;
        }
        R324(client->getFd(), client->getNickname(), server, target, server->getTabChannels().find(target)->second->getModes(), mode);
        return (1);
    }
    if (it_operator == channel->getTabOperator().end()) 
    {
        E482(client->getFd(), client->getNickname(), server, target);
        return (1);
    }
    return (0);
}

static  std::vector<std::string>  parseMode(std::string mode, std::string param)
{
    std::vector<std::string>            tab_modes;
    char                                letter;
    std::string                         arg;
    std::string                         one_mode;
    int                                 i = 0;
    int                                 y = 0;
    char                                sign;

    while (i < (int) mode.length())
    {
        while (mode[i] == '+' || mode[i] == '-')
            sign = mode[i++];
        if (mode[i] == '\0')
            return (tab_modes);
        letter = mode[i];
        if ((sign == '+' && (letter == 'l' || letter == 'k')) || letter == 'o')
        {
            while (param[y] != '\0')
            {
                if (param[y] != ' ')
                    arg.push_back(param[y++]);
                else
                {
                    y++;
                    break ;
                }
            }
        }
        one_mode.push_back(sign);
        one_mode.push_back(letter);
        if (!arg.empty())
        {
            one_mode.push_back(' ');
            one_mode = one_mode + arg;
        }
        tab_modes.push_back(one_mode);
        one_mode.clear();
        arg.clear();
        i++;
    }
    return (tab_modes);
}

static void execMode(std::vector<std::string>  tab_modes, std::string target, Client *client, Server *server)
{
    std::vector<std::string>::iterator              it;
    std::string                                     mode;
    std::string                                     arg;
    
    for (it = tab_modes.begin(); it != tab_modes.end(); it++)
    {   
        mode.clear();
        arg.clear();
        mode.push_back((*it)[0]);
        mode.push_back((*it)[1]);
        if ((*it).length() > 2)
            arg = (*it).substr(3, std::string::npos);
        if (mode == "+i")
            modeInviteOrTopic(target, client, server, mode);
        else if (mode == "-i")
            modeInviteOrTopic(target, client, server, mode);
        else if (mode == "+t")
            modeInviteOrTopic(target, client, server, mode);
        else if (mode == "-t")
            modeInviteOrTopic(target, client, server, mode);
        else if (mode == "+o")
            modeOperator(target, client, server, mode, arg);
        else if (mode == "-o")
            modeOperator(target, client, server, mode, arg);
        else if (mode == "+l")
            modeLimit(target, client, server, mode, arg);
        else if (mode == "-l")
            modeLimit(target, client, server, mode, arg);
        else if (mode == "+k")
            modeKey(target, client, server, mode, arg);
        else if (mode == "-k")
            modeKey(target, client, server, mode, arg);
    }
}

void    mode(const std::string &message, Client *client, Server *server)
{
    int         i = 0;
    std::string target = "";
    std::string mode = "";
    std::string param = "";
    std::vector<std::string>  tab_modes;

    while ((message.c_str()[i]) == ' ')
        i++;
    while ((message.c_str()[i]) != '\0')
    {
        if ((message.c_str()[i]) != ' ')
            target += (message.c_str()[i]);
        else
            break ;
        i++;
    }
    while ((message.c_str()[i]) == ' ')
        i++;
    while ((message.c_str()[i]) != '\0')
    {
        if ((message.c_str()[i]) != ' ')
            mode += (message.c_str()[i]);
        else
            break ;
        i++;
    }
    while ((message.c_str()[i]) == ' ')
        i++;
    while ((message.c_str()[i]) != '\0')
        param += (message.c_str()[i++]);
    if (target.empty())
    {
        E461(client->getFd(), client->getNickname(), server, "MODE");
        return ;
    }
    if (target == client->getNickname() || mode == "b")
        return ;
    if (errorCommand(target, client, server, mode, param))
        return ;
    tab_modes = parseMode(mode, param);
    execMode(tab_modes, target, client, server);
}
