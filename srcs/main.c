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

volatile sig_atomic_t g_signal_received = 0;

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = SIGINT;
		write(1, "\n", 1);
	}
}

void	handle_sig()
{
	if (g_signal_received == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal_received = 0;
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
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (!name)
		{
			free_env(env_cpy);
			return (NULL);
		}
		if (env[i][j] == '=')
			content = ft_strdup(&env[i][j + 1]);
		else
			content = NULL;
		if (env[i][j] == '=' && !content)
		{
			free(name);
			free_env(env_cpy);
			return (NULL);
		}
		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
		i++;
	}
	return (env_cpy);
}

//t_cmd *create_cmd()
//{
//	t_cmd *cmd = malloc(sizeof(t_cmd));
//	if (!cmd)
//		return NULL;
//	return cmd;
//}

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
			while (cmd->cmd_param && cmd->cmd_param[i])
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

void	print_tokens(t_token *head)
{
	const char	*type_names[] = {
			"REDIR_IN",
			"REDIR_OUT",
			"HEREDOC",
			"APPEND",
			"PIPE",
			"WORD"
	};

	printf("\n--- TOKENS ---\n");
	while (head)
	{
		printf("[%-8s] %s\n", type_names[head->type], head->str);
		head = head->next;
	}
	printf("--------------\n\n");
}

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

void	free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;
	int		i;

	i = 0;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->cmd_param)
		{
			i = 0;
			while (tmp->cmd_param[i])
			{
				free(tmp->cmd_param[i]);
				i++;
			}
			free(tmp->cmd_param);
		}
//		free(tmp->file_out);
//		free(tmp->file_in);
//		free(tmp->eof);
		free(tmp);
	}
}

void	free_iteration_data(t_data *data)
{
	if (data->token)
	{
		free_tokens(data->token);
		data->token = NULL;
	}
	if (data->cmd)
	{
		free_cmd(data->cmd);
		data->cmd = NULL;
	}
}

void	free_all(t_data *data, char *read)
{
	free_iteration_data(data);
	if(data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	if (read)
		free(read);
}

int main(int ac, char **av, char **env)
{
	t_data data;
	char *cwd;
	char *expanded;
	char *read;
	char *prompt;

	init_data(&data, ac, av);
	data.env = cpy_env(env);
	if (!data.env)
	{
		fprintf(stderr, "Error: Failed to copy environment\n");
		return (1);
	}
	signal(SIGINT, sig_handler);
	while (1)
	{
		handle_sig();
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			prompt = ft_strdup("minishell> ");
		}
		else
		{
			prompt = ft_strjoin(cwd, "> ");
			free(cwd);
		}
		if (!prompt)
		{
			fprintf(stderr, "Error: Memory allocation failed\n");
			break;
		}
		read = readline(prompt);
		free(prompt);
		if (!read)
			break;
		if (!read[0])
		{
			free(read);
			continue;
		}
		add_history(read);
		expanded = expand_env_var(data.env, read);
		free(read);
		if (!expanded)
		{
			fprintf(stderr, "Error: Environment expansion failed\n");
			continue;
		}
		data.token = tokenize(&data, expanded);
		free(expanded);
		if (!data.token)
			continue;
		data = cmd_builder(&data);
		print_tokens(data.token);
		print(data.cmd);
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}

//int	main(int ac, char **av, char **env)
//{
//	t_data data;
//	char	*cwd;
//	char	*expanded;
//	char *read;
//
//	cwd = NULL;
//	init_data(&data, ac, av);
//	data.env = cpy_env(env);
//	signal(SIGINT, sig_handler);
//	while (1)
//	{
//		handle_sig();
//		expanded = getcwd(NULL, 0);
//		cwd = ft_strjoin(expanded, ">");
//		free(expanded);
//		read = readline(cwd);
//		free(cwd);
//		if (!read)
//			break ;
//		add_history(read);
//		if (!read[0])
//			continue ;
//		expanded = expand_env_var(data.env ,read);
//		data.token = tokenize(&data, expanded);
//		free(expanded);
//		cmd_builder(&data);
//		print_tokens(data.token);
//		print(data.cmd);
//	}
//	rl_clear_history();
//	free_all(&data);
//	return (0);
//}
