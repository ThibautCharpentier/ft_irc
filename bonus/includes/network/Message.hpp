/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 17:35:12 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/22 17:35:12 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../ft_irc.hpp"

class Message
{
	public:
		Message(void);
		Message(const Message &src);
		~Message(void);

		Message	&operator=(const Message &src);

		const std::vector<int>			&getTabDest(void) const;
		const std::string				&getCommand(void) const;
		const std::string				&getMsg(void) const;
		const std::string				&getPrefix(void) const;

		void							setTabDest(std::vector<int> &tab_dest);
		void							setCommand(std::string command);
		void							setMsg(std::string msg);
		void							setPrefix(std::string prefix);
		void							addDest(int fd);

	private:
		std::vector<int>			_tab_dest;
		std::string					_command;
		std::string					_msg;
		std::string					_prefix;
};

#endif
