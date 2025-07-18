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

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_file
{
	int				type;
	char			*file;
	char			**eof;
	struct s_file	*next;
}	t_file;

typedef struct s_cmd
{
	char			**cmd_param;
	t_file			*file;
	struct s_cmd	*next;
}	t_cmd;

typedef enum e_token_type
{
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	PIPE,
	WORD
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	t_cmd	*cmd;
	t_list	*env;
	t_token	*token;
	int		exit_status;
}	t_data;

extern int	g_signal_received;

/*parsing*/

/*tokenize*/
t_token_type	get_operator_type(char *str, int *i);
t_token			*tokenize(t_data *data, char *str);
t_token			*create_token(char *str, t_token_type type);
char			*extract_word(char *str, int *i);
t_token			*tokenize_bis(int *i, char *str);

/*tokenize_utils*/
char			*get_operator_str(t_token_type type);
int				ft_isspace(char c);
void			init_data(t_data *data, int ac, char **av);
int				is_operator(char c);
int				trickster(int *i);

/*env*/
t_list			*cpy_env(char **env);
char			*expand_env_var(t_data *data, char *str);
t_list			*create_env_node(char *env_var, t_list **env_cpy);
void			handle_quote(int *i, int *quotes, char *str, char **res);
void			expend_env_var_bis(int *i, char *str,
					t_list *env_cpy, char **res);

/*env_utils*/
char			*char_to_str(char c);
char			*join_and_free(char *s1, char *s2);
char			*remove_quotes(const char *src);
char			*get_env_value(t_list *env, char *name);
void			check_unclosed_quotes(int quotes);

/*builder*/
t_data			cmd_builder(t_data *data);
void			cmd_builder_bis(t_token **token, t_cmd **current_cmd, int *param_index);
t_cmd			*cmd_list(t_token *token);
int				cmd_list_bis(t_token **current_token, t_cmd **head, t_cmd **current);
t_cmd			*create_new_cmd(t_token *token);

/*builder_utils*/
void			handle_redirection(t_file **files, t_token **token);
void			cmd_count(t_token *token, int *nb_pipe);
int				get_cmd_size(t_token *token);
int				is_redirection(int type);
t_file			*file_add_back(t_file **lst);
void			copy_filename(t_file *current, t_token **token);

/*utils*/
int				ft_strcmp(char *s1, char *s2);

/*free_utils*/
void			free_tokens(t_token *token);
void			free_env(t_list *env);
void			free_all(t_data *data, char *read);
void			free_file_list(t_file *file);
void			free_cmd(t_cmd *cmd);
void			free_iteration_data(t_data *data);

/*print_utils*/
void			print_tokens(t_token *head);
void			print(t_cmd *cmd);

#endif