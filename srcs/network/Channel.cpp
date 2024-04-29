/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:35:52 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/26 12:35:52 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/Channel.hpp"

Channel::Channel(void) { }

Channel::Channel(std::string name, Client *client) : _name(name), _topic("")
	, _modes("t"), _password("")
{ 
	_tab_operator.push_back(client->getUsername());
	_tab_joined.insert(std::pair<int, Client *>(client->getFd(), client));
}

Channel::Channel(const Channel &src) { *this = src; }

Channel::~Channel(void) { }

Channel	&Channel::operator=(const Channel &src)
{
	this->_name = src._name;
	this->_topic = src._topic;
	this->_modes = src._modes;
	this->_password = src._password;
	this->_tab_operator = src._tab_operator;
	this->_tab_joined = src._tab_joined;
	this->_tab_invited = src._tab_invited;
	this->_limit = src._limit;
	return (*this);
}

const std::string				&Channel::getName(void) const { return (this->_name); }
const std::string				&Channel::getTopic(void) const { return (this->_topic); }
const std::string				&Channel::getModes(void) const { return (this->_modes); }
const std::string				&Channel::getPassword(void) const { return (this->_password); }
const std::vector<std::string>	&Channel::getTabOperator(void) const { return (this->_tab_operator); }
const std::map<int, Client *>	&Channel::getTabJoined(void) const { return (this->_tab_joined); }
const std::vector<std::string>	&Channel::getTabInvited(void) const { return (this->_tab_invited); }
const long						&Channel::getLimit(void) const { return (this->_limit); }

void	Channel::setName(std::string name) { this->_name = name; }
void	Channel::setTopic(std::string topic) { this->_topic = topic; }
void	Channel::setModes(std::string modes) { this->_modes = modes; }
void	Channel::setPassword(std::string password) { this->_password = password; }
void	Channel::setLimit(long limit) { this->_limit = limit; }

bool	Channel::isOperator(std::string user) const
{
	for (std::vector<std::string>::const_iterator it = _tab_operator.begin(); it != _tab_operator.end(); it++)
	{
		if (user == *it)
			return (true);
	}
	return (false);
}

bool	Channel::lastClient(Client *client) const
{
	if (_tab_joined.size() == 1 && isInChannel(client))
		return (true);
	return (false);
}

bool	Channel::isInChannel(Client *client) const
{
	for (std::map<int, Client *>::const_iterator it = _tab_joined.begin(); it != _tab_joined.end(); it++)
	{
		if (it->second == client)
			return (true);
	}
	return (false);
}

bool	Channel::isInInvited(std::string user) const
{
	for (std::vector<std::string>::const_iterator it = _tab_invited.begin(); it != _tab_invited.end(); it++)
	{
		if (user == *it)
			return (true);
	}
	return (false);
}

void	Channel::delJoined(Client *client)
{
	_tab_joined.erase(client->getFd());
}

void	Channel::delInvited(std::string user)
{
	for (std::vector<std::string>::iterator it = _tab_invited.begin(); it != _tab_invited.end(); it++)
	{
		if (user == *it)
		{
			_tab_invited.erase(it);
			break ;
		}
	}
}

void	Channel::addJoined(Client *client)
{
	_tab_joined.insert(std::pair<int, Client *>(client->getFd(), client));
}

void	Channel::addInvited(std::string user)
{
	_tab_invited.push_back(user);
}

void	Channel::unsetMode(char mode)
{
	size_t	pos;
	
	pos = _modes.find(mode);
	if (pos != std::string::npos)
		_modes.erase(pos, 1);
}

void	Channel::addMode(char mode)
{
	_modes.push_back(mode);
}

void	Channel::addOperator(std::string user)
{
	_tab_operator.push_back(user);
}

void	Channel::deleteOperator(std::string user)
{
	std::vector<std::string>::iterator	it;

	for (it = _tab_operator.begin(); it != _tab_operator.end(); it++)
	{
		if (*it == user)
		{
			_tab_operator.erase(it);
			break ;
		}
	}
}
