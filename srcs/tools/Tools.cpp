/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igeorge <igeorge@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 13:42:10 by igeorge           #+#    #+#             */
/*   Updated: 2023/07/25 13:42:10 by igeorge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/tools/Tools.hpp"

std::string clientId(std::string nickname, std::string username, std::string host)
{
    std::string id;

    id = ":" + nickname + "!" + username + "@" + host;
    return (id);
}

std::string eraseEarlySpaces(std::string str)
{
    while (str.c_str()[0] == ' ')
        str.erase(0, 1);
    return (str);
}

std::string parseFirstWord(std::string str)
{
    std::string first_word = "";

    if (str.empty())
        return (first_word);
    str = eraseEarlySpaces(str);
    if (str.find(' ') != std::string::npos)
        first_word = str.substr(0, str.find(' '));
    else
        first_word = str;
    return (first_word);  
}

int invalidCharacters(std::string str)
{
    char    forbidden[] = {'&', ',', '*', '?', '!', '@', '.', '#', '$', ':', '\"', '\''};
    int     i = 0;

    while (i < 12)
    {
        if (str.find(forbidden[i]) != std::string::npos)
            return (1);
        i++;
    }
    return (0);
}

std::vector<std::string>   parseChannels(const std::string &message, size_t *index)
{
    std::vector<std::string>            channels;
    std::string                         name = "";
    size_t                              i = *index;

    while (i < message.length() && message.c_str()[i] == ' ')
        i++;
    while (i < message.length())
    {
        if ((message.c_str()[i]) == '#' || (message.c_str()[i]) == '&')
        {
            while ((message.c_str()[i]) != ',' && (message.c_str()[i]) != ' ' && i < message.length())
            {
                name += (message.c_str()[i]);
                i++;
            }
            channels.push_back(name);
            name.clear();
        }
        else if (message.c_str()[i] == ' ')
            break ;
        else
            i++;
    }
    *index = i;
    return (channels);
}

std::vector<std::string>   parseArgs(const std::string &message, size_t *index)
{
    std::vector<std::string>    keys;
    std::string                 name = "";
    size_t                      i = *index;

    while (i < message.length() && message.c_str()[i] == ' ')
        i++;
    while (i < message.length())
    {
        if ((message.c_str()[i]) != ' ' && (message.c_str()[i]) != ',')
        {
            while ((message.c_str()[i]) != ',' && (message.c_str()[i]) != ' ' && i < message.length())
            {
                name += (message.c_str()[i]);
                i++;
            }
            keys.push_back(name);
            name.clear();
        }
        else if (message.c_str()[i] == ' ')
            break ;
        else
            i++;
    }
    *index = i;
    return (keys);
}
