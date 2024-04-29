/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 12:17:35 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/09 12:17:35 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

bool	sig = false;

Bot::Bot(void) { }

Bot::Bot(const Bot &src) { *this = src; }

Bot::Bot(std::string addr, const char *port, std::string pass, std::string name) :
	_addr(addr), _pass(pass), _name(name), _msg_rcv(""), _connected(0)
{
	_param_sock.ai_family = AF_INET;
    _param_sock.ai_socktype = SOCK_STREAM;
    _param_sock.ai_flags = AI_PASSIVE;
    _param_sock.ai_protocol = IPPROTO_TCP;
	_port = atoi(port);
	if (getaddrinfo(_addr.c_str(), port, &_param_sock, &_servinfo) < 0)
        throw std::runtime_error("Error: getaddrinfo()");
	_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
    if (_fd == -1)
    {
        freeaddrinfo(_servinfo);
        throw std::runtime_error("Error: socket()");
    }
	int optvalue = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) < 0)
    {
        freeaddrinfo(_servinfo);
		close(_fd);
        throw std::runtime_error("Error: setsockopt()");
    }
	while (connect(_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) < 0)
    {
		if (sig == true)
		{
			freeaddrinfo(_servinfo);
			close(_fd);
			throw std::runtime_error("Error: connect()");
		}
        std::cerr << "connect() failed" << std::endl;
		std::cerr << "Retrying..." << std::endl;
		sleep(3);
    }
	freeaddrinfo(_servinfo);
    _servinfo = NULL;
}

Bot::~Bot(void) { }

Bot	&Bot::operator=(const Bot &src)
{
	this->_param_sock = src._param_sock;
	this->_servinfo = src._servinfo;
	this->_addr = src._addr;
	this->_pass = src._pass;
	this->_name = src._name;
	this->_port = src._port;
	this->_fd = src._fd;
	this->_msg_rcv = src._msg_rcv;
	this->_connected = src._connected;
	return (*this);
}

void    Bot::run(void)
{
	struct pollfd   pfd;
	
	pfd.fd = _fd;
	pfd.events = POLLIN | POLLOUT;
	while (!sig)
    {
        if (poll(&pfd, 1, -1) <= 0)
		{	
			close(_fd);
			if (errno == EINTR)
        	    return ;
        	else
            	throw std::runtime_error("Error: poll()");
		}
		if ((pfd.revents & POLLIN) && _connected == 1)
		{	
			if (!recv_msg())
				return ;
			if (!check_nick_pass())
				return ;
			_connected = 2;
			continue ;
		}
		if ((pfd.revents & POLLOUT) && (_connected == 0 || _connected == 2))
		{
            connexion();
			if (_connected == 0)
				_connected = 1;
			else if (_connected == 2)
				_connected = 3;
		}
		if ((pfd.revents & POLLIN) && _connected == 3)
		{
			if (!recv_msg())
				return ;
			check_msg();
		}
		if ((pfd.revents & POLLOUT) && _connected == 3)
            send_msg();
    }
	close(_fd);
}

void	Bot::connexion()
{
	static int pass = 0;
	
	if (pass == 0)
	{
		_msg_to_send = "PASS " + _pass + "\r\n" + "NICK " + _name + "\r\n";
		send_msg();
		pass = 1;
	}
	else
	{
		_msg_to_send = "USER " + _name + " 0 * :" + _name + "\r\n";
		send_msg();
		std::cout << "Connected as " << _name << std::endl;
		std::cout << "Joined server " << _addr << std::endl;
	}
}

void	Bot::check_msg(void)
{
	size_t		first_notice = _msg_rcv.find(" NOTICE ");
	size_t		i;
	std::string	tmp;
	std::string	dest;
	std::string	cmd;

	if (first_notice == std::string::npos
		|| (_msg_rcv.find(" INVITE ") != std::string::npos && first_notice > _msg_rcv.find(" INVITE "))
		|| (_msg_rcv.find(" PRIVMSG ") != std::string::npos && first_notice > _msg_rcv.find(" PRIVMSG ")))
		return ;
	dest = _msg_rcv.substr(1, _msg_rcv.find('!') - 1);
	tmp = _msg_rcv.substr(first_notice + 8);
	i = tmp.find(' ');
	cmd = tmp.substr(i + 1);
	if (cmd == "!roll")
		_msg_to_send += "NOTICE " + dest + " :" + ft_itoa(rand() % 6 + 1) + "\r\n";
	else if (cmd == "!nick")
		_msg_to_send += "NOTICE " + dest + " :Your nickname is " + dest + "\r\n";
	else if (cmd == "!ping")
		_msg_to_send += "NOTICE " + dest + " :PONG" + "\r\n";
	else if (cmd == "!youtube")
		_msg_to_send += "NOTICE " + dest + " :https://www.youtube.com/watch?v=dQw4w9WgXcQ" + "\r\n";
	else if (cmd == "!help")
		_msg_to_send += "NOTICE " + dest + " :!roll - !nick - !ping - !youtube" + "\r\n";
}

void	Bot::send_msg()
{
	std::string	message = _msg_to_send;

	if (send(_fd, message.c_str(), message.size(), 0) == -1)
	{
		close(_fd);
		throw std::runtime_error("Error: send()");
	}
	_msg_to_send.clear();
}

bool	Bot::recv_msg(void)
{
    char            buf[BUFFER_SIZE];
    int             read_count;

	_msg_rcv = "";
	read_count = recv(_fd, buf, BUFFER_SIZE, 0);
	if (read_count < 0)
	{
		close(_fd);
		throw std::runtime_error("Error: recv()");
	}
	else if (read_count == 0)
    {
		close(_fd);
        std::cout << "Connexion closed !" << std::endl;
        return (false);
    }
	for (int i = 0; i < read_count ; i++)
		_msg_rcv += buf[i];
	while (_msg_rcv.find("\r\n") != std::string::npos)
		_msg_rcv.erase(_msg_rcv.find("\r\n"), 2);
	return (true);
}

bool	Bot::check_nick_pass(void)
{
    size_t      pos;
    std::string nickname;

    if (_msg_rcv.find(" 432 ") != std::string::npos
		|| _msg_rcv.find(" 464 ") != std::string::npos
		|| _msg_rcv.find(" 431 ") != std::string::npos
		|| _msg_rcv.find(" 461 ") != std::string::npos)
    {
		close(_fd);
		if (_msg_rcv.find(" 461 ") != std::string::npos
			|| _msg_rcv.find(" 464 ") != std::string::npos)
			std::cerr << "Password incorrect" << std::endl;
		else if (_msg_rcv.find(" 431 ") != std::string::npos
			|| _msg_rcv.find(" 432 ") != std::string::npos)
        	std::cerr << "Erroneous name" << std::endl;
        return (false);
    }
    pos = _msg_rcv.find("NICK");
    nickname = _msg_rcv.substr(pos + 5, std::string::npos);
    if (nickname != _name)
    {
        close(_fd);
        std::cerr << "Nickname already in use" << std::endl;
        return (false);
    }
	return (true);
}

void	Bot::handler(int)
{
	sig = true;
}
