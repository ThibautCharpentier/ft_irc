/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 12:17:32 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/09 12:17:32 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#define BUFFER_SIZE 1024

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <csignal>
#include <errno.h>
#include <poll.h>

std::string	ft_itoa(int n);

class Bot
{
	public:
		Bot(void);
		Bot(std::string addr, const char *port, std::string pass, std::string name);
		Bot(const Bot &src);
        ~Bot(void);

        Bot	&operator=(const Bot &src);

		void	run(void);

		static void	handler(int);

	private:
		struct addrinfo     _param_sock;
        struct addrinfo     *_servinfo;
		std::string			_addr;
		std::string			_pass;
		std::string			_name;
		std::string			_msg_rcv;
		std::string			_msg_to_send;
		int					_port;
		int					_fd;
		int					_connected;

		void	connexion(void);
		void	check_msg(void);
		void	send_msg();
		bool	recv_msg(void);
		bool	check_nick_pass(void);
};

#endif
