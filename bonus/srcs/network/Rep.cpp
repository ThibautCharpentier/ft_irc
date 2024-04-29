/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rep.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:06:11 by tcharpen          #+#    #+#             */
/*   Updated: 2023/07/24 14:06:11 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/Rep.hpp"

void	send_to_client(std::string msg, int const &fd, Server *server)
{
	Client  	*client;
	std::string	message;

	client = server->getTabClients().find(fd)->second;
	message = ":ircserv " + msg +"\r\n";
	client->addMessageToRecv(message);
}

void	send_to_client_things(std::string msg, int const &fd, Server *server)
{
	Client  	*client;
	std::string	message;

	client = server->getTabClients().find(fd)->second;
	message = msg +"\r\n";
	client->addMessageToRecv(message);
}

void 	R001(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "001 " << name << " :Welcome " << name << " to the Internet Chat Relay!";
	send_to_client(output.str(), fd, server);
}

void	R324(REP_ARG, const std::string &channel, const std::string &mode, const std::string &mode_param)
{
	std::stringstream	output;

	output.str("");
	output << "324 " << name << " " << channel << " " << mode << " " << mode_param;
	send_to_client(output.str(), fd, server);
}

void	R331(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "331 " << name << " " << channel << " :No topic is set";
	send_to_client(output.str(), fd, server);
}

void	R332(REP_ARG, const std::string &channel, const std::string &topic)
{
	std::stringstream	output;

	output.str("");
	output << "332 " << name << " " << channel << " :" << topic;
	send_to_client(output.str(), fd, server);
}

void	R341(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << "341 " << name << " " << channel << " " << input_name;
	send_to_client(output.str(), fd, server);
}

void	R353(REP_ARG, const char &symbol, const std::string &channel, const std::string &members)
{
	std::stringstream	output;

	output.str("");
	output << "353 " << name << " " << symbol << " " << channel << " :" << members;
	send_to_client(output.str(), fd, server);
}

void	R366(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "366 " << name << " " << channel << " :End of /NAMES list";
	send_to_client(output.str(), fd, server);
}

void	R372(REP_ARG, const std::string &line)
{
	std::stringstream	output;

	output.str("");
	output << "372 " << name << " : " << line;
	send_to_client(output.str(), fd, server);
}

void	R375(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "375 " << name << " :- ircserv Message of the day - ";
	send_to_client(output.str(), fd, server);
}

void	R376(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "376 " << name << " :End of MOTD command";
	send_to_client(output.str(), fd, server);
}

void	R391(REP_ARG)
{
	std::stringstream	output;
	char 				date_string[50];
	time_t 				curr_time;
	tm 					*curr_tm;

	output.str("");
	time(&curr_time);
	curr_tm = localtime(&curr_time);
	strftime(date_string, 50, "%c", curr_tm);
	output << "391 " << name << " ircserv :" << date_string;
	send_to_client(output.str(), fd, server);
}

void	E401(REP_ARG, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << "401 " << name << " " << input_name << " :No such nick/channel";
	send_to_client(output.str(), fd, server);
}

void	E402(REP_ARG, const std::string &server_name)
{
	std::stringstream	output;

	output.str("");
	output << "402 " << name << " " << server_name << " :No such server";
	send_to_client(output.str(), fd, server);
}

void 	E403(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "403 " << name << " " << channel << " :No such channel";
	send_to_client(output.str(), fd, server);
}

void 	E404(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "404 " << name << " " << channel << " :Cannot send to channel";
	send_to_client(output.str(), fd, server);
}

void	E405(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "405 " << name << " " << channel <<  " :You have joined too many channels";	
	send_to_client(output.str(), fd, server);
}

void	E411(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << "411 " << name << " :No recipient given (" << cmd << ")";
	send_to_client(output.str(), fd, server);
}

void	E412(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "412 " << name << " :No text to send";
	send_to_client(output.str(), fd, server);
}

void	E421(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << "461 " << name << " " << cmd << " :Unknown command";
	send_to_client(output.str(), fd, server);
}

void	E422(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "422 " << name << " :MOTD File is missing";
	send_to_client(output.str(), fd, server);
}

void	E431(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "431 " << name << " :No nickname given";
	send_to_client(output.str(), fd, server);
}

void	E432(REP_ARG, const std::string &bad_name)
{
	std::stringstream	output;

	output.str("");
	output << "432 " << name << " " << bad_name << " :Erroneous nickname";
	send_to_client(output.str(), fd, server);
}

void	E433(REP_ARG, const std::string &bad_name)
{
	std::stringstream	output;

	output.str("");
	output << "433 " << name << " " << bad_name << " :Nickname is already in use";
	send_to_client(output.str(), fd, server);
}

void	E441(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << "441 " << name << " " << input_name << " " << channel << " :They aren't on that channel";
	send_to_client(output.str(), fd, server);
}

void	E442(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "442 " << name << " " << channel << " :You're not on that channel";
	send_to_client(output.str(), fd, server);
}

void	E443(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << "443 " << name << " " << input_name << " " << channel << " :is already on channel";
	send_to_client(output.str(), fd, server);
}

void	E461(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << "461 " << name << " " << cmd << " :Not enough parameters";
	send_to_client(output.str(), fd, server);
}

void	E462(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "462 " << name << " :Unauthorized command (already registered)";
	send_to_client(output.str(), fd, server);
}

void	E464(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << "464 " << name << " :Password incorrect";
	send_to_client(output.str(), fd, server);
}

void	E471(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "471 " << name << " " << channel << " :Cannot join channel (+l)";
	send_to_client(output.str(), fd, server);
}

void 	E472(REP_ARG, const std::string &channel, const char &mode)
{
	std::stringstream	output;

	output.str("");
	output << "472 " << name << " " << mode << " :is unknown mode char to me for "
	<< channel;
	send_to_client(output.str(), fd, server);
}

void	E473(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "473 " << name << " " << channel << " :Cannot join channel (+i)";
	send_to_client(output.str(), fd, server);
}

void	E475(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "475 " << name << " " << channel << " :Cannot join channel (+k)";
	send_to_client(output.str(), fd, server);
}

void	E482(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "482 " << name << " " << channel << " :You're not channel operator";
	send_to_client(output.str(), fd, server);
}

void	E696(REP_ARG, const std::string &channel, const std::string &mode, const std::string &password)
{
	std::stringstream	output;

	output.str("");
	output << "696 " << name << " " << channel << " " << mode << " " << password << " :limit must only contained digit";
	send_to_client(output.str(), fd, server);
}

void	E525(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << "525 " << name << " " << channel << " :Key is not well-formed";
	send_to_client(output.str(), fd, server);
}
