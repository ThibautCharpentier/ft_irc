/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:21:33 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/29 18:21:33 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void	motd(const std::string &message, Client *client, Server *server)
{
	std::ifstream	file;
	std::string		line;

	if (!message.empty() && message != "ircserv")
	{
		E402(client->getFd(), client->getNickname(), server, message);
		return ;
	}
	if (file.is_open())
		file.close();
	file.open("./config/motd.config", std::ifstream::in);
	if (file.bad() || file.fail())
	{
		E422(client->getFd(), client->getNickname(), server);
		return ;
	}
	R375(client->getFd(), client->getNickname(), server);
	while (getline(file, line))
		R372(client->getFd(), client->getNickname(), server, line);
	R376(client->getFd(), client->getNickname(), server);
	file.close();
}
