/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 20:50:14 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 20:50:14 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void   pong(const std::string &message, Client *client, Server *server)
{
    std::stringstream	output;

	if (message == "ircserv")
	{
		output.str("");
		output << clientId(client->getNickname(), client->getUsername(), client->getHostname()) << " PONG " << message;
		send_to_client_things(output.str(), client->getFd(), server);
	}
	else
		E402(client->getFd(), client->getNickname(), server, message);
}
 