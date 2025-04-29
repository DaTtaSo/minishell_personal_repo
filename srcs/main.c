/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/14 12:19:36 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

t_list *cpy_env(char **env)
{
	int	i;
	int	j;
	char *name;
	char *content;
	t_list *env_cpy;

	env_cpy = NULL;
	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (env[i][j] == '=')
			content = ft_strdup(&env[i][j + 1]);
		else
			content = NULL;
		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
		i++;
	}
	return (env_cpy);
}

t_cmd *create_cmd()
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;
	return cmd;
}

void print(t_cmd *cmd)
{
	int i;

	if (!cmd)
	{
		printf("Command list is empty\n");
		return;
	}

	printf("Starting to print commands:\n");  // Debug message

	while (cmd)
	{
		i = 0;
		printf("Command at %p:\n", (void*)cmd);  // Print command address for debugging

		if (!cmd->cmd_param)
		{
			printf("  cmd_param array is NULL\n");
		}
		else
		{
			while (cmd->cmd_param[i])
			{
				printf("  param[%d]: '%s'\n", i, cmd->cmd_param[i]);
				i++;
			}
			if (i == 0)
				printf("  No parameters found\n");
		}

		cmd = cmd->next;
	}
	printf("Done printing commands\n");  // Debug message
}

//void	print(t_cmd *cmd)
//{
//	int i;
//
//	while (cmd)
//	{
//		i = 0;
//		while (cmd->cmd_param[i])
//		{
//			printf("name: %-15s\n", cmd->cmd_param[i]);
//			i++;
//		}
//		cmd = cmd->next;
//	}
//}

//int	main(int ac, char **av, char **env)
//{
//	(void)ac;
//	(void)av;
//	t_list *env_cpy = cpy_env(env);
//	print_env_list(env_cpy);
//	return (0);
//}

//void	print_tokens(t_token *head)
//{
//	const char	*type_names[] = {
//			"REDIR_IN",
//			"REDIR_OUT",
//			"HEREDOC",
//			"APPEND",
//			"PIPE",
//			"WORD"
//	};
//
//	printf("\n--- TOKENS ---\n");
//	while (head)
//	{
//		printf("[%-8s] %s\n", type_names[head->type], head->str);
//		head = head->next;
//	}
//	printf("--------------\n\n");
//}

void	free_env(t_list *env)
{
	t_list *tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->content);
		free(tmp);
	}
}

void	free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
	token = NULL;
}

void	free_all(t_data data, char *read)
{
	if (data.token)
		free_tokens(data.token);
	if (data.env)
		free_env(data.env);
	free(read);
}


int	main(int ac, char **av, char **env)
{
	t_data data;
	char	*cwd;
	char	*expended;
	char *read;

	cwd = NULL;
	init_data(&data, ac, av);
	data.env = cpy_env(env);
	signal(SIGINT, sig_handler);
	while (1)
	{
		expended = getcwd(NULL, 0);
		cwd = ft_strjoin(expended, ">");
		free(expended);
		read = readline(cwd);
		free(cwd);
		if (!read)
			break ;
		add_history(read);
		if (!read[0])
			continue ;
		expended = expand_env_var(data.env ,read);
		data.token = tokenize(&data, expended);
		free(expended);
		cmd_builder(data);
//		print_tokens(data.token);
		print(data.cmd);
		free_tokens(data.token);
	}
	rl_clear_history();
	free_all(data, read);
	return (0);
}
