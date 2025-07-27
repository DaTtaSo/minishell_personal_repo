/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/22 06:09:24 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	ft_bzero(data, sizeof(t_data));
	data->stdout_save = -1;
	data->stdin_save = -1;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->prev_fd = -1;
	if (!data->stdin_save || !data->stdout_save)
	{
		printf("save dup failed\n");
		exit(1);
	}
	g_exit_status = 0;
}

int	er_msg_free_tok(char *arg, char *msg, t_token **token)
{
	char	*tmp;
	int		res;

	tmp = NULL;
	if (arg)
		tmp = ft_strdup(arg);
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	res = ft_error_msg(tmp, msg);
	if (tmp && *tmp)
		free(tmp);
	return (res);
}

int	check_synthax(t_data *data)
{
	t_token	*token;

	token = data->token;
	if (!(token->str))
		return (er_msg_free_tok(NULL, "command not found", &data->token));
	if (token->type == PIPE)
		return (er_msg_free_tok(token->str,
				"syntax error near unexpected token", &data->token));
	while (token)
	{
		if ((!token->str || token->str[0] == '\0'))
			return (er_msg_free_tok(token->str, "command not found",
					&data->token));
		if (token->type == PIPE && (!token->next || token->next->type == PIPE))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		else if (!(token->type == PIPE || token->type == WORD) && (!token->next
				|| token->next->type != WORD))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		token = token->next;
	}
	return (0);
}

char	*ft_read_urandom(void)
{
	int		fd;
	int		nb_bytes;
	char	buf[2];
	char	*name;
	int		i;

	name = malloc(sizeof(char) * 21);
	if (!name)
		return (ft_error_msg(NULL, "malloc failed"), NULL);
	name[20] = '\0';
	buf[1] = '\0';
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(name);
		perror("open: urandom");
		return (NULL);
	}
	i = 0;
	while (i < 20)
	{
		nb_bytes = read(fd, buf, 1);
		if (nb_bytes != 1)
		{
			free(name);
			perror("read: urandom");
			return (NULL);
		}
		if (ft_isalpha(buf[0]))
		{
			name[i] = buf[0];
			i++;
		}
	}
	close(fd);
	return (name);
}

int	ft_tmp_file(t_file **file)
{
	int		fd;
	char	*name;
	char	*name_tmp;

	name_tmp = ft_read_urandom();
	if (!name_tmp)
		return (-1);
	name = ft_strjoin("/tmp/", name_tmp);
	free(name_tmp);
	if (!name)
	{
		free(name);
		ft_error_msg("ft_strjoin", "malloc failed");
		return (-1);
	}
	fd = open(name, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
	{
		perror("open:");
		return (-1);
	}
	(*file)->filename = name;
	return (fd);
}

void	handle_heredoc(t_data *data)
{
	t_cmd	*tmp;
	t_file	*tmp_file;

	tmp = data->cmd;
	while (tmp)
	{
		tmp_file = tmp->file;
		while (tmp_file)
		{
			if (tmp_file->type == HEREDOC)
				ft_heredoc(tmp_file);
			tmp_file = tmp_file->next;
		}
		tmp = tmp->next;
	}
}

void	ft_heredoc(t_file *tmp)
{
	char	*read;
	int		fd;

	fd = -1;
	fd = ft_tmp_file(&tmp);
	if (fd == -1)
		return ;
	while (!g_exit_status)
	{
		read = readline("> ");
		if (!read)
		{
			ft_error_msg("warning", "here-document delimited by end-of-file");
			close(fd);
			return ;
		}
		if (!*read)
			continue ;
		if (ft_strcmp(read, tmp->eof))
		{
			write(fd, read, ft_strlen(read));
			write(fd, "\n", 1);
		}
		else
		{
			free(read);
			close(fd);
			break ;
		}
		if (!read[0])
		{
			write(fd, "\n", 1);
			continue ;
		}
	}
	close(fd);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;
	char	*read;
	pid_t	pid;

	init_data(&data, ac, av);
	pid = 0;
	data.env = cpy_env(env);
	rl_event_hook = do_nothing;
	data.stdin_save = dup(STDIN_FILENO);
	data.stdout_save = dup(STDOUT_FILENO);
	if (data.stdin_save == -1 || data.stdout_save == -1)
		return (ft_error_msg("dup", "dup failed"));
	if (!data.env)
		return (ft_error_msg("cpy_env", "Error: Failed to copy environment"));
	set_signals_prompt();
	while (1)
	{
		dup2(data.stdin_save, STDIN_FILENO);
		dup2(data.stdout_save, STDOUT_FILENO);
		read = readline("minishell> ");
		if (g_exit_status)
			data.exit_status = 130;
		g_exit_status = 0;
		if (!read)
		{
			ft_close_save(&data);
			break ;
		}
		if (!read[0])
		{
			free(read);
			continue ;
		}
		add_history(read);
		data.token = tokenize(&data, read);
		free(read);
		if (!data.token || check_synthax(&data))
		{
			data.exit_status = 2;
			free_tokens(&data.token);
			continue ;
		}
		expand_tokens(&data);
		data = cmd_builder(&data);
		 print_tokens(data.token);
//		 print(data.cmd);
		handle_heredoc(&data);
		signal(SIGINT, SIG_IGN);
		data.exit_status = ft_exec(&data, pid);
		if (data.exit_status > 128)
			write(1, "\n", 1);
		set_signals_prompt();
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}
