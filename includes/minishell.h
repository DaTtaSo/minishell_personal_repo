/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 13:48:52 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/14 11:51:03 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>


//#define REDIR_IN	1	//>
//#define REDIR_OUT	2	//<
//#define HEREDOC		3	//<<
//#define APPEND		4	//>>
//#define PIPE		5	//|
//#define WORD			6

typedef struct s_cmd
{
	char	**cmd_param;
	struct s_cmd	*next;
}	t_cmd ;

typedef enum e_token_type
{
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	PIPE,
	WORD
} t_token_type;

typedef struct s_token
{
	char	*str;
	t_token_type type;
	struct s_token *next;
}	t_token ;

typedef struct s_data
{
	t_cmd *cmd;
	t_list *env;
	t_token *token;
	char *fd_in;
	char *fd_out;
	struct s_data *next ;
}	t_data;

char	**parse_env(char **env);

//parsing
void	init_data(t_data *data, int ac, char **av);
t_token *tokenize(t_data *data, char *str);
t_list *cpy_env(char **env);
char *expand_env_var(t_list *env_cpy, char *str);
int	ft_isspace(char c);






#endif