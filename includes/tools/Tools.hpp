/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:40:20 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 13:40:20 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../ft_irc.hpp"

std::string					parseFirstWord(std::string str);
int 						invalidCharacters(std::string str);
std::string 				clientId(std::string nickname, std::string username, std::string host);
std::vector<std::string>	parseChannels(const std::string &message, size_t *index);
std::string 				eraseEarlySpaces(std::string str);
std::vector<std::string>	parseArgs(const std::string &message, size_t *index);
std::string					ft_itoa(int n);

#endif
