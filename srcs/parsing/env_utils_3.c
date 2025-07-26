/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbedouan <nbedouan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 22:57:35 by nbedouan          #+#    #+#             */
/*   Updated: 2025/07/16 23:33:03 by nbedouan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_exit_status(t_data **data, int *i, char *str, char **res)
{
	char	*value;

	value = ft_itoa((*data)->exit_status);
	*res = join_and_free(*res, ft_strdup(value));
	free(value);
	(*i) += 2;
}

int	needs_retokenization(char *str)
{
	int i = 0;
	int quotes = 0;

	while (str[i])
	{
		if (str[i] == '\'' && quotes != 2)
		{
			if (quotes == 1)
				quotes = 0;
			else
				quotes = 1;
		}
		else if (str[i] == '\"' && quotes != 1)
		{
			if (quotes == 2)
				quotes = 0;
			else
				quotes = 2;
		}
		else if (!quotes && (is_operator(str[i]) || ft_isspace(str[i])))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	replace_token_with_list(t_token **token_list, t_token *to_replace, t_token *new_tokens)
{
	t_token *prev = NULL;
	t_token *current = *token_list;

	while (current && current != to_replace)
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return;
	t_token *last_new = new_tokens;
	while (last_new && last_new->next)
		last_new = last_new->next;
	if (prev)
		prev->next = new_tokens;
	else
		*token_list = new_tokens;
	if (last_new)
		last_new->next = current->next;
	free(current->str);
	free(current);
}

int	token_contains_quotes(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

char	*remove_outer_quotes(char *str)
{
	int len = ft_strlen(str);

	if (len < 2)
		return (ft_strdup(str));

	if ((str[0] == '"' && str[len-1] == '"') ||
		(str[0] == '\'' && str[len-1] == '\''))
	{
		if (len == 2)
			return (ft_strdup(""));
		return (ft_substr(str, 1, len - 2));
	}

	return (ft_strdup(str));
}
