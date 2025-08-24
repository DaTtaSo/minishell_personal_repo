/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 04:52:07 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/26 05:22:26 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_outer_quotes(char *str, t_quote_type q_type)
{
	int		len;
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((q_type == SINGLE_QUOTES && str[0] == '\'' && str[len - 1] == '\'')
		|| (q_type == DOUBLE_QUOTES && str[0] == '"' && str[len - 1] == '"'))
	{
		i = 1;
		while (i < len - 1)
		{
			if ((str[i] == '"' || str[i] == '\'') && i == 1)
				return (ft_strdup(str));
			i++;
		}
		if (len == 2)
			result = ft_strdup("");
		else
			result = ft_substr(str, 1, len - 2);
	}
	else
		result = ft_strdup(str);
	if (!result)
		return (NULL);
	return (result);
}

t_token	*process_word_token_bis(t_data *data, t_token *current, t_token *next)
{
	char	*expanded;

	if (current->q_type == SINGLE_QUOTES && current->retokenized)
		return (next);
	expanded = expand_env_var(data, current->str, &current);
	if (!expanded)
		return (next);
	if (current->str)
		free(current->str);
	current->str = expanded;
	return (next);
}

static void	process_word_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;

	current = data->token;
	while (current)
	{
		next = current->next;
		if (current->type == WORD && current->str)
			current = process_word_token_bis(data, current, next);
		else
			current = next;
	}
}

static void	process_quoted_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;
	char	*clean;

	current = data->token;
	while (current)
	{
		next = current->next;
		if ((current->q_type != NO_QUOTES || current->in_quote == 3)
			&& current->str && !check_q(&current) && !current->retokenized)
		{
			clean = remove_outer_quotes(current->str, current->q_type);
			if (!clean)
			{
				current = next;
				continue ;
			}
			free(current->str);
			current->str = clean;
			current->q_type = NO_QUOTES;
		}
		current = next;
	}
}

void	expand_tokens(t_data *data)
{
	process_word_tokens(data);
	process_quoted_tokens(data);
}
