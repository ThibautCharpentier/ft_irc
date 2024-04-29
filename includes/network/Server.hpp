/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 18:02:29 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/09 18:02:29 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define	SERVER_HPP

#include "../ft_irc.hpp"

class Client;
class Channel;

class Server
{
    public:
        Server(void);
		Server(const Server &src);
        Server(std::string port, std::string password);
        ~Server(void);

        Server	&operator=(const Server &src);

        const std::string                       &getPort(void) const;
        const std::string                       &getPassword(void) const;
        const std::map<int, Client *>           &getTabClients(void) const;
        const std::map<std::string, Channel *>  &getTabChannels(void) const;

        void        launchServer(char *port);
        void        loopServer(void);
        void        freeServer(void);
        void        delClient(int fd, Client *client, std::vector<struct pollfd>::iterator it_poll);
        void        delChannel(std::string name, Channel *channel);
        void        displayServerMessage(void) const;
        void        insertChannel(std::string name, Channel *channel);
        Channel *   isChannel(std::string channel) const;
        Client *    isClient(std::string client) const;

        static void handler(int);

    private:
        struct addrinfo                     _param_sock;
        struct addrinfo                     *_servinfo;
        int                                 _socket_fd;
        int                                 _max_fd;
        std::string                         _password;
        std::string                         _port;
        std::vector<struct pollfd>          _poll_fds;
        std::map<int, Client *>             _tab_clients;
        std::map<std::string, Channel *>    _tab_channels;

        void    initEnv(void);
        void    setSocketInfo(char *port);
        void    createSocket(void);
        void    addClient(void);
        bool    clientMessage(int fd, std::vector<struct pollfd>::iterator it_poll);
        void    delClientInAllChannels(Client *client);
        void    sentMessage(int fd);
};

#endif
