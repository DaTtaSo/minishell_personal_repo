/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temporary_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:00:41 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:57 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include "../includes/minishell.h"

void	print(t_cmd *cmd)
{
	int	i;
	int	cmd_num = 0;

	if (!cmd)
	{
		printf("Command list is empty\n");
		return;
	}

	printf("Starting to print commands:\n");

	while (cmd)
	{
		i = 0;
		printf("Command %d at %p:\n", cmd_num++, (void *)cmd);

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

		// ✅ Itérer sans écraser cmd->file
		t_file *file = cmd->file;
		while (file)
		{
			printf("  file: '%s'\n", file->file ? file->file : "(none)");
			printf("  type: '%d'\n", file->type);
			file = file->next;
		}
		cmd = cmd->next;
	}
	printf("Done printing commands\n");
}

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
