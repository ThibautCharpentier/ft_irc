# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 18:14:59 by tcharpen          #+#    #+#              #
#    Updated: 2023/07/18 18:14:59 by tcharpen         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
RM = rm -f
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 #-fsanitize=address

INCLUDES =  ft_irc.hpp \
network/Client.hpp network/Server.hpp network/Message.hpp network/Rep.hpp \
network/Channel.hpp \
tools/Tools.hpp \
commands/Command.hpp

SRCS = main.cpp \
network/Client.cpp network/Server.cpp network/Message.cpp network/Rep.cpp \
network/Channel.cpp \
tools/Tools.cpp tools/ft_itoa.cpp \
commands/nick.cpp commands/pass.cpp commands/user.cpp commands/pong.cpp \
commands/join.cpp commands/quit.cpp commands/part.cpp commands/topic.cpp \
commands/motd.cpp commands/time.cpp commands/mode.cpp commands/invite.cpp \
commands/kick.cpp commands/privmsg.cpp commands/notice.cpp \
commands/mode_channel/modeInviteOrTopic.cpp commands/mode_channel/modeOperator.cpp \
commands/mode_channel/modeKey.cpp commands/mode_channel/modeLimit.cpp

INCS = $(addprefix includes/,${INCLUDES})
OBJ = $(addprefix srcs/,$(SRCS:.cpp=.o))

%.o: %.cpp ${INCS} Makefile
		${CC} ${FLAGS} -c $< -o $@

all: ${NAME}

$(NAME): $(OBJ)
		$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
