/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:44:23 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:28:46 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
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
