/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:01:14 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/09 18:01:14 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

static bool check_password(char *pass)
{
    if (pass[0] == '\0')
        return (false);
    for (int i = 0; pass[i] != '\0'; i++)
    {
        if (pass[i] == ' ')
            return (false);
    }
    return (true);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error usage: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    if (!check_password(argv[2]))
    {
        std::cerr << "Error usage: <password> must not contain spaces or be empty" << std::endl;
        return (1);
    }

    Server  server(argv[1], argv[2]);

    signal(SIGINT, &Server::handler);
    signal(SIGQUIT, &Server::handler);
    try
    {   
        server.launchServer(argv[1]);
        server.loopServer();
        server.freeServer();
        return (0);
    }
    catch (const std::exception& e)
    {
        server.freeServer();
        std::cerr << e.what() << std::endl;
        return (1);
    }
}
