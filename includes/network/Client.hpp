/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 18:19:54 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/18 18:19:54 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define	CLIENT_HPP

#include "../ft_irc.hpp"

#define NOT_CONNECTED 0
#define CONNECTED 1

class Message;
class Server;

class Client
{    
    public:
		Client(void);
		Client(const Client &src);
        Client(int fd, struct sockaddr_in address);
		~Client(void);

		Client	&operator=(const Client &src);

		const std::string			&getNickname(void) const;
		const std::string			&getUsername(void) const;
		const std::string			&getHostname(void) const;
		const std::string			&getMessage(void) const;
		const std::string			&getMessageToRecv(void) const;
		Message						*getParsedMsg(void) const;
		const bool					&getStatus(void) const;
		const bool					&getHasAllInfo(void) const;
		const int					&getFd(void) const;
		const bool					&getPassword(void) const;

		void						setNickname(std::string nickname);
		void						setUsername(std::string username);
		void						setStatus(bool status);
		void						setFd(int fd);
		void						setMessage(std::string str);
		void						setMessage(char *buffer, int read_count);
		void						setParsedMsg(Message *msg);
		void						setHasAllInfo(bool has_all_info);

		bool						parseMessage(Server *server);
		void						addMessageToRecv(std::string msg);
		void						clearMessageToRecv(void);

    private:
        std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_message;
		std::string _message_to_recv;
		Message		*_parsed_msg;
		bool		_status;
		bool		_has_all_info;
		int			_fd;
		bool		_password;

		std::vector<std::string>	splitMessage(void);
		void						fillClient(std::string line, Server *server);
		int							parseCommand(Message *msg, std::string line);
		bool						allInfoIsSet();
		void						sendClientRegistration(Server *server);
		bool						execCommand(std::string line, Server *server);
};

#endif
