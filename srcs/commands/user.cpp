/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:28:03 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 13:28:03 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

static bool	isAlreadyUsed(std::string username, Server *server, int client_fd)
{
    const std::map<int, Client *> all_clients = server->getTabClients();

    for (std::map<int, Client *>::const_iterator it = all_clients.begin(); it != all_clients.end(); it++)
    {
        if (it->second->getFd() != client_fd && it->second->getUsername() == username)
            return (true);
    }
    return (false);
}

void    user(const std::string &message, Client *client, Server *server)
{
	std::string	user = parseFirstWord(message);
	int			i = 2;
	size_t		size_user;

	if (client->getUsername() != "")
		E462(client->getFd(), client->getNickname(), server);
	else if (user.empty())
		E461(client->getFd(), client->getNickname(), server, "USER");
	else if (!invalidCharacters(user))
	{
		if (user.size() > 13)
			user = user.substr(0, 13);
		size_user = user.size();
		while (isAlreadyUsed(user, server, client->getFd()))
		{
			if (i == 2)
				user += ft_itoa(i);
			else
				user.replace(size_user, std::string::npos, ft_itoa(i));
			i++;
		}
		client->setUsername(user);
	}
}
