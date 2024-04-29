/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:58:52 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 13:58:52 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Command.hpp"

bool	pass(const std::string &message, Client *client, const std::string &password, Server *server)
{
	std::string	pass = parseFirstWord(message);

	if (client->getPassword() == true)
	{
		E462(client->getFd(), client->getNickname(), server);
		return (true);
	}
	else if (pass.empty())
	{
		E461(client->getFd(), client->getNickname(), server, "PASS");
		return (false);
	}
	else if (pass != password)
	{
		E464(client->getFd(), client->getNickname(), server);
		return (false);
	}
	return (true);
}
