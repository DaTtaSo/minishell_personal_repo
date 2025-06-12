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
	int		type;
	char	**eof;
	char	**cmd_param;
	char	*file_in;
	char	*file_out;
	struct	s_cmd	*next;
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
}	t_data;

/*parsing*/
void	init_data(t_data *data, int ac, char **av);
t_token *tokenize(t_data *data, char *str);
t_list *cpy_env(char **env);
char *expand_env_var(t_list *env_cpy, char *str);
int	ft_isspace(char c);
t_data	cmd_builder(t_data *data);
t_cmd *cmd_list(t_token *token);

/*utils*/
int	ft_strcmp(char *s1, char *s2);
void	free_tokens(t_token *token);
void	free_env(t_list *env);
void	free_all(t_data *data);



#endif