/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:54:08 by nbedouan          #+#    #+#             */
/*   Updated: 2025/08/24 02:54:21 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_token(t_token **current)
{
	int	i;

	i = 0;
	if (!current || !(*current) || !(*current)->str)
		return (1);
	while ((*current)->str[i])
	{
		if ((*current)->str[i] != '"' && (*current)->str[i] != '\'')
			return (1);
		i++;
	}
	return (0);
}

int	check_q(t_token **current)
{
	int	i;

	i = 0;
	if (!(*current) || !(*current)->str)
		return (0);
	while ((*current)->str[i])
	{
		if ((*current)->str[i] != '"' && (*current)->str[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}
