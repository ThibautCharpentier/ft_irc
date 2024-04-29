/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:23:16 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/09 18:23:16 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/Server.hpp"

bool    sig = false;

Server::Server(void) { }

Server::Server(const Server &src) { *this = src; }

Server::Server(std::string port, std::string password) : _socket_fd(-1)
    , _password(password), _port(port)
{   
    this->displayServerMessage();
}

Server::~Server(void) { }

Server	&Server::operator=(const Server &src)
{
	this->_param_sock = src._param_sock;
	this->_servinfo = src._servinfo;
	this->_socket_fd = src._socket_fd;
    this->_max_fd = src._max_fd;
    this->_password = src._password;
    this->_port = src._port;
    this->_poll_fds = src._poll_fds;
    this->_tab_clients = src._tab_clients;
    this->_tab_channels = src._tab_channels;
	return (*this);
}

const std::string                       &Server::getPort(void) const { return (_port); }
const std::string                       &Server::getPassword(void) const { return (_password); }
const std::map<int, Client *>           &Server::getTabClients(void) const { return (_tab_clients); }
const std::map<std::string, Channel *>  &Server::getTabChannels(void) const { return (_tab_channels); }

void    Server::initEnv()
{
    struct  rlimit  rlp;

    if (getrlimit(RLIMIT_NOFILE, &rlp))
        throw std::runtime_error("Error: getrlimit()");
    _max_fd = rlp.rlim_cur;
}

void    Server::setSocketInfo(char *port)
{
    _param_sock.ai_family = AF_INET;
    _param_sock.ai_socktype = SOCK_STREAM;
    _param_sock.ai_flags = AI_PASSIVE;
    _param_sock.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(NULL, port, &_param_sock, &_servinfo) < 0)
        throw std::runtime_error("Error: getaddrinfo()");
}

void     Server::createSocket(void)
{
    _socket_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
    if (_socket_fd == -1)
    {
        freeaddrinfo(_servinfo);
        throw std::runtime_error("Error: socket()");
    }
    int optvalue = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) < 0)
    {
        freeaddrinfo(_servinfo);
        throw std::runtime_error("Error: setsockopt()");
    }
    if (bind(_socket_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) < 0)
    {
        freeaddrinfo(_servinfo);
        throw   std::runtime_error("Error: bind()");
    }
    if (listen(_socket_fd, SOMAXCONN) < 0)
    {
        freeaddrinfo(_servinfo);
        throw   std::runtime_error("Error: listen()");
    }
    freeaddrinfo(_servinfo);
    _servinfo = NULL;
}

void    Server::addClient(void)
{
    int                 fd_new_client;
    struct sockaddr_in  addr_new_client;
    socklen_t           addr_new_client_len;
    Client              *new_client;

    if (_tab_clients.size() == (unsigned long)_max_fd)
    {
        if (shutdown(_socket_fd, SHUT_RD) < 0)
            throw std::runtime_error("Error: shutdown()");
    }
    addr_new_client_len = sizeof(addr_new_client);
    fd_new_client = accept(_socket_fd, (struct sockaddr *)&addr_new_client,  &addr_new_client_len);
    if (fd_new_client < 0)
        throw std::runtime_error("Error: accept()");
    new_client = new (std::nothrow) Client(fd_new_client, addr_new_client);
    if (!new_client)
        throw std::runtime_error("Error: operator new");
    _tab_clients.insert(std::pair<int, Client *>(fd_new_client, new_client));
    _poll_fds.push_back(pollfd());
	_poll_fds.back().fd = fd_new_client;
	_poll_fds.back().events = POLLIN | POLLOUT;
}

bool    Server::clientMessage(int fd, std::vector<struct pollfd>::iterator it_poll)
{
    Client  *client;
    char    buffer[BUFFER_SIZE];
    int     read_count;

    client = _tab_clients.find(fd)->second;

    memset(buffer, 0, sizeof(buffer));
    read_count = recv(fd, buffer, BUFFER_SIZE, 0);
    if (read_count == -1)
        throw std::runtime_error("Error: recv()");
    else if (read_count == 0)
    {
        delClientInAllChannels(client);
        delClient(fd, client, it_poll);
        this->displayServerMessage();
        return (false);
    }
    else
    {
        client->setMessage(buffer, read_count);
        if (client->getMessage().find("\r\n") != std::string::npos)
        {
            if (!client->parseMessage(this))
            {
                delClient(fd, client, it_poll);
                this->displayServerMessage();
                return (false);
            }
        }
    }
    return (true);
}

void    Server::delClientInAllChannels(Client *client)
{
    std::vector<std::string>	name_chan;

    for (std::map<std::string, Channel *>::const_iterator it = this->getTabChannels().begin(); it != this->getTabChannels().end(); it++)
		name_chan.push_back(it->first);
    for (std::vector<std::string>::iterator it_chan = name_chan.begin(); it_chan != name_chan.end(); it_chan++)
	{
		Channel	*channel = this->isChannel(*it_chan);
		if (channel->lastClient(client))
			this->delChannel(*it_chan, channel);
		else if (channel->isInChannel(client))
			channel->delJoined(client);
	}
}

void    Server::delClient(int fd, Client *client, std::vector<struct pollfd>::iterator it_poll)
{
    _tab_clients.erase(fd);
    if (client->getParsedMsg())
        delete client->getParsedMsg();
    delete client;
    _poll_fds.erase(it_poll);
    close(fd);
}

void    Server::delChannel(std::string name, Channel *channel)
{
    _tab_channels.erase(name);
    delete channel;
}

void    Server::displayServerMessage(void) const
{
    for (int i = 0; i < 10; i++)
        std::cout << std::endl;
    std::cout << "Welcome to IRC Server" << std::endl;
    std::cout << std::endl;
    std::cout << "Channels: " << this->_tab_channels.size() << std::endl;
    for (std::map<std::string, Channel *>::const_iterator it = _tab_channels.begin(); it != _tab_channels.end(); it++)
        std::cout << it->second->getName() << std::endl;
    std::cout << std::endl;
    std::cout << "FD        Nickname        Username        Host        Status" << std::endl;
    for (std::map<int, Client *>::const_iterator it = _tab_clients.begin(); it != _tab_clients.end(); it++)
    {
        Client  *client = it->second;

        std::cout << it->first;
        if (it->first < 10)
            std::cout << "         ";
        else
            std::cout << "        ";
        std::cout << client->getNickname();
        for (unsigned long i = 0; i < 16 - client->getNickname().size(); i++)
            std::cout << " ";
        std::cout << client->getUsername();
        for (unsigned long i = 0; i < 16 - client->getUsername().size(); i++)
            std::cout << " ";
        std::cout << client->getHostname();
        for (unsigned long i = 0; i < 12 - client->getHostname().size(); i++)
            std::cout << " ";
        if (client->getStatus())
            std::cout << "CONNECTED" << std::endl;
        else
            std::cout << "NOT CONNECTED" << std::endl;
    }
}

void    Server::launchServer(char  *port)
{
    initEnv();
    setSocketInfo(port);
    createSocket();
}

void  Server::sentMessage(int fd)
{
    Client      *client;
    std::string message;

    client = _tab_clients.find(fd)->second;
    message = client->getMessageToRecv();
    if (!message.empty())
    {   
        if (send(fd, message.c_str(), message.size(), 0) == -1)
		    throw std::runtime_error("Error: send()");
        client->clearMessageToRecv();
    }
}

void    Server::loopServer()
{
    struct pollfd   server_poll_fd;

    server_poll_fd.fd = _socket_fd;
    server_poll_fd.events = POLLIN;
    _poll_fds.push_back(server_poll_fd);
    while (!sig)
    {
        if (poll(&_poll_fds[0], (unsigned int)_poll_fds.size(), -1) <= 0)
        {
            if (errno == EINTR)
                break ;
            else
                throw std::runtime_error("Error server: poll");
        }
        if (_poll_fds[0].revents & POLLIN)
            addClient();
        else
        {
            for (std::vector<struct pollfd>::iterator it = _poll_fds.begin() + 1; it != _poll_fds.end(); it++)
            {
                if ((it->revents & POLLIN) && !clientMessage(it->fd, it))
                    break ;
                if ((it->revents & POLLOUT))
                    sentMessage(it->fd);
            }
        }
    }
}

void    Server::freeServer(void)
{
    for (std::map<int, Client *>::iterator it = _tab_clients.begin(); it != _tab_clients.end(); it++)
    {
        close(it->first);
        delete it->second;
    }
    for (std::map<std::string, Channel *>::iterator it = _tab_channels.begin(); it != _tab_channels.end(); it++)
        delete it->second;
    if (_socket_fd > 2)
        close(_socket_fd);
}

void    Server::insertChannel(std::string name, Channel *channel)
{
    _tab_channels.insert(std::pair<std::string, Channel *>(name, channel));
}

Channel *Server::isChannel(std::string channel) const
{
    for (std::map<std::string, Channel *>::const_iterator it = _tab_channels.begin(); it != _tab_channels.end(); it++)
    {
        if (it->first == channel)
            return (it->second);
    }
    return (NULL);
}

Client *Server::isClient(std::string client) const
{
    for (std::map<int, Client *>::const_iterator it = _tab_clients.begin(); it != _tab_clients.end(); it++)
    {
        if (it->second->getNickname() == client)
            return (it->second);
    }
    return (NULL);
}

void    Server::handler(int)
{
    sig = true;
}
