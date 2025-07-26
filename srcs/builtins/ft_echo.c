/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:19:09 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/24 18:19:15 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **cmd_param)
{
	int	i;
	int	j;
	int	nl;

	i = 1;
	j = 0;
	nl = 1;
	while (cmd_param[i] && strncmp(cmd_param[i], "-n", 2) == 0)
	{
		j = 1;
		while (cmd_param[i][j] == 'n')
			j++;
		if (cmd_param[i][j])
			break ;
		nl = 0;
		i++;
	}
	while (cmd_param[i])
	{
		printf("%s", cmd_param[i]);
		if (cmd_param[i + 1])
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}