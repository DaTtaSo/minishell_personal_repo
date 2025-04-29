/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 22:02:16 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/25 22:02:22 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_data	cmd_builder(t_data data)
{
	int	i;
	t_token *token;
	t_cmd *cmd;

	if (!data.token || !data.cmd)
		return data;
	i = 0;
	token = data.token;
	cmd = data.cmd;
	while (token)
	{
		if (token->type != PIPE)
		{
			cmd->cmd_param[i] = ft_strdup(token->str);
			token = token->next;
			i++;
		}
		else
		{
			cmd->cmd_param[i] = NULL;
			cmd = cmd->next;
			token = token->next;
			i = 0;
		}
	}
	if (cmd)
		cmd->cmd_param[i] = NULL;
	return (data);
}