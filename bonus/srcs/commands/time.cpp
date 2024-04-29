/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 18:21:33 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/29 18:21:33 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

void	time(const std::string &message, Client *client, Server *server)
{
	if (!message.empty() && message != "ircserv")
	{
		E402(client->getFd(), client->getNickname(), server, message);
		return ;
	}
	R391(client->getFd(), client->getNickname(), server);
}
