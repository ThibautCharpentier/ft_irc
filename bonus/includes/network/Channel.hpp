/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:49:59 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/26 11:49:59 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define	CHANNEL_HPP

#include "../ft_irc.hpp"

class Client;
class Server;

class Channel
{
	public:
		Channel(void);
		Channel(std::string name, Client *client);
		Channel(const Channel &src);
		~Channel(void);

		Channel	&operator=(const Channel &src);

		const std::string				&getName(void) const;
		const std::string				&getTopic(void) const;
		const std::string				&getModes(void) const;
		const std::string				&getPassword(void) const;
		const std::vector<std::string>	&getTabOperator(void) const;
		const std::map<int, Client *>	&getTabJoined(void) const;
		const std::vector<std::string>	&getTabInvited(void) const;
		const long						&getLimit(void) const;

		void							setName(std::string name);
		void							setTopic(std::string topic);
		void							setModes(std::string modes);
		void							setPassword(std::string password);
		void							setLimit(long limit);

		bool							isOperator(std::string user) const;
		bool							lastClient(Client *client) const;
		bool							isInChannel(Client *client) const;
		bool							isInInvited(std::string user) const;
		void							delJoined(Client *client);
		void							delInvited(std::string user);
		void							addJoined(Client *client);
		void							addInvited(std::string user);
		void							unsetMode(char mode);
		void							addMode(char mode);
		void							addOperator(std::string user);
		void							deleteOperator(std::string user);

	private:
		std::string					_name;
		std::string					_topic;
		std::string					_modes;
		std::string					_password;
		std::vector<std::string>	_tab_operator;
		std::map<int, Client *>		_tab_joined;
		std::vector<std::string>	_tab_invited;
		long						_limit;
};

#endif
