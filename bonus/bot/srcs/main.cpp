/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 12:17:33 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/09 12:17:33 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

int main(int argc, char **argv)
{
    int i = 0;

    if (argc != 5)
    {
        std::cerr << "Error usage: ./ircbot <name> <host> [port] [password]" << std::endl;
        return (1);
    }
    while (argv[3][i])
    {
        if (!isdigit(argv[3][i]))
        {
            std::cerr << "Error usage: [port] must contains only digit" << std::endl;
            return (1);
        }
        i++;
    }
	signal(SIGINT, &Bot::handler);
    signal(SIGQUIT, &Bot::handler);
    try 
    {
        Bot bot(argv[2], argv[3], argv[4], argv[1]);
        bot.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
