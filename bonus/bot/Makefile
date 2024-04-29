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

NAME = ircbot

CC = c++
RM = rm -f
FLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 #-fsanitize=address

INCLUDES = Bot.hpp

SRCS = main.cpp Bot.cpp ft_itoa.cpp

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
