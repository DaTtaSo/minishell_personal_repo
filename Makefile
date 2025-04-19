# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 13:49:07 by alarroye          #+#    #+#              #
#    Updated: 2025/04/12 14:50:47 by alarroye         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #


NAME 				= 			minishell

LIBFT				=			includes/libft/libft.a

HEAD 				=			minishell.h

OBJ_DIR				=			obj/

CC					=			cc

CFLAGS				=		-Wall -Werror -MMD -MP -g3

SRCS				=			$(addprefix $(SRCS_DIR), $(SRC_ACC))

SRCS_DIR			=			srcs/
PARSING_DIR			=			parsing/

PARSING_SRCS		=			tokenize \
								env


MAIN_SRCS			=			main

SRC_ACC				+=			$(addprefix $(PARSING_DIR), $(addsuffix .c, $(PARSING_SRCS)))
SRC_ACC				+=			$(addsuffix .c, $(MAIN_SRCS))

OBJ			=			$(patsubst $(SRCS_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

DEP			=			$(patsubst $(SRCS_DIR)%.c,$(OBJ_DIR)%.d,$(SRCS))

all:					$(NAME)

$(NAME):				$(OBJ) $(LIBFT)
							$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
						@mkdir -p $(dir $@)
						$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): FORCE
	$(MAKE) -C includes/libft
	$(MAKE) bonus -C includes/libft

clean:
						make clean -C includes/libft
						@rm -rf $(OBJ_DIR)
						@echo "Deleting $(OBJ_DIR)"


fclean:					clean
							make fclean -C includes/libft
							@rm -rf $(NAME)
							@echo "Deleting $(NAME)"


re: 					fclean
						$(MAKE) all

FORCE:

.PHONY: 				re all clean fclean FORCE

-include $(DEP)