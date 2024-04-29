/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 23:27:31 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/24 23:27:31 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static bool isAlreadyUsed(std::string nickname, Server *server, int client_fd)
{
    const std::map<int, Client *> all_clients = server->getTabClients();

    for (std::map<int, Client *>::const_iterator it = all_clients.begin(); it != all_clients.end(); it++)
    {
        if (it->second->getFd() != client_fd && it->second->getNickname() == nickname)
            return (true);
    }
    return (false);
}

void    nick(const std::string &message, Client *client, Server *server)
{
    std::string         nickname;
    std::string         old_nickname = client->getNickname();
    std::stringstream	output;
    size_t		        size_nick;
    int			        i = 2;

    nickname = parseFirstWord(message);
    if (nickname.empty())
        E431(client->getFd(), client->getNickname(), server);
    else if (invalidCharacters(nickname))
        E432(client->getFd(), client->getNickname(), server, nickname);
    else
    {
        if (nickname.size() > 13)
            nickname = nickname.substr(0, 13);
        size_nick = nickname.size();
        while (isAlreadyUsed(nickname, server, client->getFd()))
        {
            if (i == 2)
				nickname += ft_itoa(i);
			else
				nickname.replace(size_nick, std::string::npos, ft_itoa(i));
			i++;
        }
        client->setNickname(nickname);
	    output.str("");
	    output << clientId(old_nickname, client->getUsername(), client->getHostname()) << " NICK " << nickname;
        send_to_client_things(output.str(), client->getFd(), server);
        if (client->getStatus() == CONNECTED)
            server->displayServerMessage();
    }
}
