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

#include "../../inc/minishell.h"

char	**parse_env(char **env)
{
	char	*tab;
	char	**path;
	int		i;

	tab = NULL;
	i = 0;
	path = NULL;
	while (env[i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
			break ;
		i++;
	}
	if (env[i])
		tab = ft_strdup(&env[i][4]);
	if (!tab)
		return (NULL);
	path = ft_split(tab, ':');
	free(tab);
	if (!path || !(*path))
		ft_free_dtab(path);
	return (path);
}

