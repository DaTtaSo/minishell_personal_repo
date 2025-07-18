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

#include "minishell.h"

int	g_signal_received;

void	sig_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_signal_received = 1;
}

int	check_synthax(t_token *token)
{
	if (token->type == PIPE)
	{
		perror("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	while (token)
	{
		if (token->type == PIPE && (!token->next || token->next->type == PIPE))
		{
			perror("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
		else if (!(token->type == PIPE || token->type == WORD)
			&& (!token->next || token->next->type != WORD))
		{
			perror("minishell: syntax error near unexpected token"
				" '< > << >>'\n");
			return (1);
		}
		token = token->next;
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_data				data;
	char				*cwd;
	char				*expanded;
	char				*read;
	char				*prompt;

	init_data(&data, ac, av);
	data.env = cpy_env(env);
	if (!data.env)
	{
		fprintf(stderr, "Error: Failed to copy environment\n");
		return (1);
	}
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
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
			break ;
		}
		read = readline(prompt);
		if (g_signal_received)
			data.exit_status = 130;
		g_signal_received = 0;
		free(prompt);
		if (!read)
			break ;
		if (!read[0])
		{
			free(read);
			continue ;
		}
		add_history(read);
		expanded = expand_env_var(&data, read);
		free(read);
		if (!expanded)
		{
			fprintf(stderr, "Error: Environment expansion failed\n");
			continue ;
		}
		data.token = tokenize(&data, expanded);
		free(expanded);
		if (!data.token || check_synthax(data.token))
			continue ;
		data = cmd_builder(&data);
		print_tokens(data.token);
		print(data.cmd);
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}
