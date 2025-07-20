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
	data->error = -1;
	if (!data->stdin_save || !data->stdout_save)
	{
		perror("save dup failed\n");
		exit(1);
	}
	g_signal_received = 0;
}

int	count_heredoc(t_token *cmd_file)
{
	int		res;
	t_token	*file;

	file = cmd_file;
	res = 0;
	while (file)
	{
		if (file->type == HEREDOC)
			res++;
		file = file->next;
	}
	return (res);
}

void	ft_heredoc(t_data *data)
{
	char	*read;
	int		fd[2];

	pipe(fd);
	while (1)
	{
		read = readline("> ");
		if (!read)
		{
			perror("minishell : warning: here-document"
				" delimited by end-of-file\n");
			close(fd[1]);
			close(fd[0]);
			return ;
		}
		if (ft_strcmp(read, data->cmd->file->eof))
		{
			write(fd[1], read, ft_strlen(read));
			write(fd[1], "\n", 1);
		}
		else
		{
			free(read);
			close(fd[1]);
			break ;
		}
		if (!read[0])
		{
			write(fd[1], "\n", 1);
			continue ;
		}
	}
	if (data->cmd && data->cmd->cmd_param && data->cmd->cmd_param[0])
	{
		dup2(fd[0], STDIN_FILENO);
	}
	close(fd[0]);
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
	free(tmp);
	return (res);
}

int	check_synthax(t_data *data)
{
	t_token *token;

	token = data->token;
	if (!(token->str))
		return (er_msg_free_tok(NULL, "command not found", &data->token));
	if (token->type == PIPE)
		return (er_msg_free_tok(token->str,
				"syntax error near unexpected token", &data->token));
	while (token)
	{
		if ((!token->str || token->str[0] == '\0'))
			return (er_msg_free_tok(token->str, "command not found", &data->token));
		if (token->type == PIPE && (!token->next || token->next->type == PIPE))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		else if (!(token->type == PIPE || token->type == WORD)
			&& (!token->next || token->next->type != WORD))
			return (er_msg_free_tok(token->str,
					"syntax error near unexpected token", &data->token));
		token = token->next;
	}
	return (0);
}

int	ft_child_builtins(t_cmd *cmd, t_data *data)
{
	if (data->prev_fd != -1)
	{
		dup2(data->prev_fd, STDIN_FILENO);
		close(data->prev_fd);
	}
	if (cmd->next)
	{
		close(data->fd[0]);
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
	if (handle_redir(cmd))
		exit(1);
	if (!cmd->cmd_param[0])
		exit(0);
	return (builtins(cmd->cmd_param, &data->env));
}

int	ft_child(t_cmd *cmd, char *path_cmd, t_data *data)
{
	char	**env_exec;

	env_exec = NULL;
	if (data->prev_fd != -1)
	{
		dup2(data->prev_fd, STDIN_FILENO);
		close(data->prev_fd);
	}
	if (cmd->next)
	{
		close(data->fd[0]);
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
	if (handle_redir(cmd))
		exit(1);
	if (!cmd->cmd_param[0])
		exit(0);
	env_exec = lst_in_tab(data->env);
	if (!env_exec)
		return (printf("malloc failed\n"), 1);
	execve(path_cmd, cmd->cmd_param, env_exec);
	ft_failed_execve(data, cmd->cmd_param, env_exec, path_cmd);
	perror("execve");
	exit(errno);
	return (0);
}

int	ft_failed_execve(t_data *data, char **cmd, char **env, char *path_cmd)
{
	free_all(data, NULL);
	ft_free_dtab(env);
	if (path_cmd)
		free(path_cmd);
	return (0);
}

pid_t	handle_children(pid_t pid, t_cmd *cmd, t_data *data, char *path_cmd)
{
	pid = fork();
	if (pid == -1)
		return (printf("pid error\n"), 1);
	if (pid == 0)
	{
		ft_close_save(data);
		if (cmd->cmd_param[0] && is_builtins(cmd->cmd_param[0]))
		{
			ft_child_builtins(cmd, data);
			free_all(data, NULL);
			exit(close(data->fd[0]));
		}
		ft_child(cmd, path_cmd, data);
	}
	if (data->prev_fd != -1)
		close(data->prev_fd);
	if (cmd->next)
	{
		close(data->fd[1]);
		data->prev_fd = data->fd[0];
	}
	else if (data->fd[0] != -1)
		close(data->fd[0]);
	return (pid);
}

int	is_builtins(char *cmd)
{
	if (cmd && (!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export")
			|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "cd")
			|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "echo")
			|| !ft_strcmp(cmd, "exit")))
		return (1);
	return (0);
}

int	builtins(char **cmd, t_list **env)
{
	if (!ft_strcmp(cmd[0], "env"))
		ft_env(*env);
	else if (!ft_strcmp(cmd[0], "unset"))
		ft_unset(env, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		ft_export(env, cmd);
	else if (!ft_strcmp(cmd[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(env, cmd);
	else
		return (1);
	return (0);
}

int	handle_redir(t_cmd *cmd)
{
	t_file	*tmp;

	tmp = cmd->file;
	while (tmp)
	{
		if (tmp->type == REDIR_IN && redirect_infile(tmp->filename))
			return (printf("error redir in\n"), 1);
		else if (tmp->type == REDIR_OUT && redirect_outfile(tmp->filename))
			return (1);
		else if (tmp->type == APPEND && redirect_outfile_append(tmp->filename))
			return (printf("error redir append\n"), 1);
		tmp = tmp->next;
	}
	return (0);
}


int	ft_exec(t_data *data, pid_t pid)
{
	t_cmd	*cmd;
	char	*path_cmd;

	cmd = data->cmd;
	while (cmd)
	{
		if (cmd->next && pipe(data->fd) == -1)
			return (printf("pipe error\n"), 1);
		path_cmd = ft_path(cmd->cmd_param[0], data->env, &data->error);
		if (data->error == 127 || data->error == 126)
		{
			cmd = cmd->next;
			continue ;
		}
		if (ft_cmdlen(data->cmd) == 1 && cmd->cmd_param[0] && !handle_redir(data->cmd)
			&& !builtins(cmd->cmd_param, &data->env))
			break ;
		if (cmd->cmd_param[0])
			pid = handle_children(pid, cmd, data, path_cmd);
		if (path_cmd && *path_cmd)
			free(path_cmd);
		cmd = cmd->next;
	}
	data->prev_fd = -1;
	return (ft_wait(data->cmd, pid, &data->error));
}

int	ft_wait(t_cmd *head, pid_t pid, int *error)
{
	while (head)
	{
		waitpid(-1, error, 0);
		head = head->next;
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
		perror("Error: Failed to copy environment\n");
		return (1);
	}
	data.stdin_save = dup(STDIN_FILENO);
	data.stdout_save = dup(STDOUT_FILENO);
	if (data.stdin_save == -1 || data.stdout_save == -1)
	{
		perror("error dup save");
		return (1);
	}
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		dup2(data.stdin_save, STDIN_FILENO);
		dup2(data.stdout_save, STDOUT_FILENO);
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
			perror("Error: Memory allocation failed\n");
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
			perror("Error: Environment expansion failed\n");
			continue ;
		}
		data.token = tokenize(&data, expanded);
		free(expanded);
		if (!data.token || check_synthax(&data))
			continue;
		data = cmd_builder(&data);
//		print_tokens(data.token);
//		print(data.cmd);
		ft_exec(&data, data.pid);
		free_iteration_data(&data);
	}
	rl_clear_history();
	free_all(&data, read);
	return (0);
}
