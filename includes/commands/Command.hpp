/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 23:28:01 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/24 23:28:01 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../ft_irc.hpp"

void    nick(const std::string &message, Client *client, Server *server);
void    user(const std::string &message, Client *client, Server *server);
bool	pass(const std::string &message, Client *client, const std::string &password, Server *server);
void    pong(const std::string &message, Client *client, Server *server);
void    join(const std::string &message, Client *client, Server *server);
void	part(const std::string &message, Client *client, Server *server);
void	quit(const std::string &message, Client *client, Server *server);
void	topic(const std::string &message, Client *client, Server *server);
void	motd(const std::string &message, Client *client, Server *server);
void	time(const std::string &message, Client *client, Server *server);
void    mode(const std::string &message, Client *client, Server *server);
void    invite(const std::string &message, Client *client, Server *server);
void    kick(const std::string &message, Client *client, Server *server);
void    privmsg(const std::string &message, Client *client, Server *server);
void    notice(const std::string &message, Client *client, Server *server);

void    modeInviteOrTopic(const std::string &target, Client *client, Server *server, const std::string &mode);
void    modeOperator(const std::string &target, Client *client, Server *server, const std::string &mode, const std::string &arg);
void    modeLimit(const std::string &target, Client *client, Server *server,const std::string &mode,const std::string &arg);
void    modeKey(const std::string &target, Client *client, Server *server,const std::string &mode,const std::string &arg);

#endif
