/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 05:22:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 05:22:37 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quote(int *i, int *quotes, char *str)
{
	if (str[(*i)] == '\'' && (*quotes) != 2)
	{
		if ((*quotes) == 1)
			(*quotes) = 0;
		else
			(*quotes) = 1;
		(*i)++;
		return (1);
	}
	else if (str[(*i)] == '\"' && (*quotes) != 1)
	{
		if ((*quotes) == 2)
			(*quotes) = 0;
		else
			(*quotes) = 2;
		(*i)++;
		return (1);
	}
	return (0);
}
