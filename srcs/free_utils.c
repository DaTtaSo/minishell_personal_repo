/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:23:57 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:34 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_list *env)
{
	t_list	*tmp;

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
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
	token = NULL;
}

void	free_file_list(t_file *file)
{
	t_file	*tmp;

	while (file)
	{
		tmp = file;
		file = file->next;
		if (tmp->file)
			free(tmp->file);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
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
		if (tmp->file)
			free_file_list(tmp->file);
		free(tmp);
	}
}

void	free_all(t_data *data, char *read)
{
	free_iteration_data(data);
	if (data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	if (read)
		free(read);
}
