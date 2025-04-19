/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:58:17 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/14 13:58:44 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_env_var(t_list *env_cpy, char *str)
{
	int i;
	int quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && quotes != 2)
			quotes = quotes == 1 ? 0 : 1;
		else if (str[i] == '\"' && quotes != 1)
			quotes = quotes == 2 ? 0 : 2;
		i++;
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')


	}
	return (NULL);
}
