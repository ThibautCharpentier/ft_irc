/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 18:03:07 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/22 18:03:07 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/Message.hpp"

Message::Message(void) { }

Message::Message(const Message &src) : _tab_dest(src._tab_dest)
	, _command(src._command), _msg(src._msg), _prefix(src._prefix) { }

Message::~Message(void) { }

Message	&Message::operator=(const Message &src)
{
	this->_tab_dest = src._tab_dest;
	this->_command = src._command;
	this->_msg = src._msg;
	this->_prefix = src._prefix;
	return (*this);
}

const std::vector<int>			&Message::getTabDest(void) const { return (this->_tab_dest); }
const std::string				&Message::getCommand(void) const { return (this->_command); }
const std::string				&Message::getMsg(void) const { return (this->_msg); }
const std::string				&Message::getPrefix(void) const { return (this->_prefix); }

void	Message::setTabDest(std::vector<int> &tab_dest) { this->_tab_dest = tab_dest; }
void	Message::setCommand(std::string command) { this->_command = command; }
void	Message::setMsg(std::string msg) { this->_msg = msg; }
void	Message::setPrefix(std::string prefix) { this->_prefix = prefix; }

void	Message::addDest(int fd) { this->_tab_dest.push_back(fd); }
