/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:39:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/04/14 14:39:41 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	data->cmd = NULL;
	data->type = NULL;
}

t_tokens *tokenize(char *str)
{

}
