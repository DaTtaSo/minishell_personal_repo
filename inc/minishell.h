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

# include "../libft/libft.h"
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

typedef struct s_tokens
{
	char	*str;
	t_token_type type;
	struct s_tokens *next;
}	t_tokens ;

typedef struct s_data
{
	t_tokens type; // genre si c un file une cmd un < | > >> << t a capter k
	t_cmd *cmd;
	struct s_data *next ;
}	t_data;

int		ft_free_dtab(char **tab);
void	ft_error_msg(char *arg, char *msg);
int		ft_error(char *msg, char **path, char **dtab, int status);
char	**parse_env(char **env);
char	*search_path(char *cmd, char **path, int *error);
int		ft_is_access(char *path_cmd, int *error);

//parsing
void	init_data(t_data *data, int ac, char **av);

#endif