/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:06:27 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/18 19:06:27 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/Client.hpp"

Client::Client(void) { }

Client::Client(const Client &src) : _nickname(src._nickname)
	, _username(src._username) , _hostname(src._hostname), _message(src._message)
	, _message_to_recv(src._message_to_recv), _parsed_msg(src._parsed_msg), _status(src._status)
	, _has_all_info(false), _fd(src._fd), _password(src._password) { }

Client::Client(int fd, struct sockaddr_in address) :
	_nickname(""), _username(""), _message(""), _message_to_recv("")
	, _parsed_msg(NULL), _status(NOT_CONNECTED)
	, _has_all_info(false), _fd(fd), _password(false)
{
	char	hostname[BUFFER_SIZE];

	fcntl(this->_fd, F_SETFL, O_NONBLOCK);
	if (getnameinfo((const struct sockaddr *)&address, sizeof(address), hostname, BUFFER_SIZE, NULL, 0, NI_NUMERICSERV))
		throw std::runtime_error("Error: getnameinfo()");
	this->_hostname = hostname;
}

Client::~Client(void) { }

Client	&Client::operator=(const Client &src)
{
	this->_nickname = src._nickname;
	this->_username = src._username;
	this->_hostname = src._hostname;
	this->_message = src._message;
	this->_message_to_recv = src._message_to_recv;
	this->_parsed_msg = src._parsed_msg;
	this->_status = src._status;
	this->_has_all_info = src._has_all_info;
	this->_fd = src._fd;
	this->_password = src._password;
	return (*this);
}

const std::string	&Client::getNickname(void) const { return (this->_nickname); }
const std::string	&Client::getUsername(void) const { return (this->_username); }
const std::string	&Client::getHostname(void) const { return (this->_hostname); }
const std::string	&Client::getMessage(void) const { return (this->_message); }
const std::string	&Client::getMessageToRecv(void) const { return (this->_message_to_recv); }
Message				*Client::getParsedMsg(void) const { return (this->_parsed_msg); }
const bool			&Client::getStatus(void) const { return (this->_status); }
const bool			&Client::getHasAllInfo(void) const { return (this->_has_all_info); }
const int			&Client::getFd(void) const { return (this->_fd); }
const bool			&Client::getPassword(void) const { return (this->_password); }

void	Client::setNickname(std::string nickname) { this->_nickname = nickname; }
void	Client::setUsername(std::string username) { this->_username = username; }
void	Client::setStatus(bool status) { this->_status = status; }
void	Client::setFd(int fd) { this->_fd = fd; }
void	Client::setMessage(std::string str) { this->_message = str; }
void	Client::setParsedMsg(Message *msg) { this->_parsed_msg = msg; }
void	Client::setHasAllInfo(bool has_all_info) { this->_has_all_info = has_all_info; }
void	Client::setMessage(char *buffer, int read_count)
{
	if (_message.find("\r\n") != std::string::npos)
		_message = "";
	for (int i = 0; i < read_count ; i++)
		_message += buffer[i];
}

void	Client::addMessageToRecv(std::string msg) 
{	
	this->_message_to_recv += msg;
}

void	Client::clearMessageToRecv(void)
{
	_message_to_recv.clear();
}

std::vector<std::string>    Client::splitMessage(void)
{
    int 		                pos = 0;
	std::string                 substr;
    std::vector<std::string>	tab_line;

	while ((pos = this->_message.find("\r\n")) != static_cast<int>(std::string::npos))
	{
		substr = this->_message.substr(0, pos);
		tab_line.push_back(substr);
		this->_message.erase(0, pos + 2);
	}
    return (tab_line);
}

void    Client::fillClient(std::string line, Server *server)
{
    if (!parseCommand(_parsed_msg, line))
	{
        return ;
	}
	if (_parsed_msg->getCommand() == "NICK")
		nick(_parsed_msg->getMsg(), this, server);
	else if (_parsed_msg->getCommand() == "USER")
		user(_parsed_msg->getMsg(), this, server);
	else if (_parsed_msg->getCommand() == "PASS")
	{
		if (pass(_parsed_msg->getMsg(), this, server->getPassword(), server))
			_password = true;
	}
}

int Client::parseCommand(Message *msg, std::string line)
{
    std::string command = line;
	std::string	prefix;
	std::string	message;

    if (line.empty())
        return (0);
    if (line[0] == ':')
    {
        if (line.find(' ') != std::string::npos)
            command.erase(0, command.find(' ') + 1);
		prefix = line.substr(0, line.length() - line.find(' '));
		msg->setPrefix(prefix);
    }
	if (command.find(' ') == std::string::npos)
	{
		if (command.find("\r\n") != std::string::npos)
			command.erase(command.find("\r\n"), 2);
	}
	else
	{
		message = command.substr(command.find(' ') + 1, command.length() - command.find(' ') + 1);
		command.erase(command.find(' '), command.length() - command.find(' ') + 1);
	}
	for (long unsigned int i = 0; i < command.length(); i++)
		command[i] = std::toupper(command[i]);
	msg->setCommand(command);
	if (message.find("\r\n") != std::string::npos)
		message.erase(message.find("\r\n"), 2);
	msg->setMsg(message);
	return (1);
}

bool	Client::allInfoIsSet()
{
	if (_username.empty() || _nickname.empty() || _password == false)
		return (false);
	else
		return (true);
}

void	Client::sendClientRegistration(Server *server)
{
	std::ifstream	file;
	std::string		line;

	R001(_fd, _nickname, server);

	if (file.is_open())
		file.close();
	file.open("./config/motd.config", std::ifstream::in);
	if (file.bad() || file.fail())
	{
		E422(_fd, _nickname, server);
		return ;
	}
	R375(_fd, _nickname, server);
	while (getline(file, line))
		R372(_fd, _nickname, server, line);
	R376(_fd, _nickname, server);
	file.close();
}

bool	Client::execCommand(std::string line, Server *server)
{
	std::string	all_cmds[] = {
		"NICK",
		"USER",
		"PASS",
		"PING",
		"JOIN",
		"QUIT",
		"PART",
		"TOPIC",
		"MOTD",
		"TIME",
		"MODE",
		"INVITE",
		"KICK",
		"PRIVMSG",
		"NOTICE"
	};
	int			i = 0;

	if (!parseCommand(_parsed_msg, line))
        return (true);
	while (i < 15)
	{
		if (all_cmds[i] == _parsed_msg->getCommand())
			break;
		i++;
	}
	switch (i + 1)
	{
		case 1: 
			nick(_parsed_msg->getMsg(), this, server); 
			break ;
		case 2:
			user(_parsed_msg->getMsg(), this, server);
			break ;
		case 3: 
			pass(_parsed_msg->getMsg(), this, server->getPassword(), server);
			break ;
		case 4:
			pong(_parsed_msg->getMsg(), this, server);
			break ;
		case 5:
			join(_parsed_msg->getMsg(), this, server);
			break ;
		case 6:
			quit(_parsed_msg->getMsg(), this, server);
			break ;
		case 7:
			part(_parsed_msg->getMsg(), this, server);
			break ;
		case 8:
			topic(_parsed_msg->getMsg(), this, server);
			break ;
		case 9:
			motd(_parsed_msg->getMsg(), this, server);
			break ;
		case 10:
			time(_parsed_msg->getMsg(), this, server);
			break ;
		case 11:
			mode(_parsed_msg->getMsg(), this, server);
			break ;
		case 12:
			invite(_parsed_msg->getMsg(), this, server);
			break ;
		case 13:
			kick(_parsed_msg->getMsg(), this, server);
			break ;
		case 14:
			privmsg(_parsed_msg->getMsg(), this, server);
			break ;
		case 15:
			notice(_parsed_msg->getMsg(), this, server);
			break ;
		default: 
			E421(_fd, _nickname, server, _parsed_msg->getCommand());
	}
	if ((i + 1) == 6)
		return (false);
	return (true);
}

bool	Client::parseMessage(Server *server)
{
    std::vector<std::string>	tab_line = splitMessage();

	_parsed_msg = new (std::nothrow) Message();
	if (!_parsed_msg)
        throw std::runtime_error("Error: operator new");
    for (long unsigned int i = 0; i < tab_line.size(); i++)
    {
        if (this->getStatus() == NOT_CONNECTED)
        {
            if (_has_all_info == false)
            {
                fillClient(tab_line[i], server);
                if (allInfoIsSet() == true)
                    _has_all_info = true;
            }
			if (_has_all_info == true)
			{
				sendClientRegistration(server);
				_status = CONNECTED;
				server->displayServerMessage();
			}
        }
		else if (!execCommand(tab_line[i], server))
		{
			delete _parsed_msg;
			_parsed_msg = NULL;
			return (false);
		}
    }
	delete _parsed_msg;
	_parsed_msg = NULL;
	return (true);
}
