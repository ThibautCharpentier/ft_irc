/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 18:16:11 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/18 18:16:11 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <poll.h>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <cstdio>
#include <sstream>
#include <ctime>
#include <cctype>
#include <fstream>
#include <csignal>

#include "./network/Message.hpp"
#include "./network/Client.hpp"
#include "./network/Channel.hpp"
#include "./network/Server.hpp"
#include "./network/Rep.hpp"
#include "./commands/Command.hpp"
#include "./tools/Tools.hpp"

#define BUFFER_SIZE 1024

#endif
