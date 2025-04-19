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

#include "../../includes/minishell.h"

void	init_data(t_data *data, int ac, char **av)
{
	(void)ac;
	(void)av;
	data->cmd = NULL;
	data->env = NULL;
	data->fd_out = NULL;
	data->fd_in = NULL;
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char *extract_word(char *str, int *i)
{
	int	start;
	int	quotes;

	start = *i;
	quotes = 0;
	while (str[*i] && (quotes || (!is_operator(str[*i]) && !ft_isspace(str[*i]))))
	{
		if (str[*i] == '\'' && quotes != 2)
			quotes = quotes == 1 ? 0 : 1;
		else if (str[*i] == '\"' && quotes != 1)
			quotes = quotes == 2 ? 0 : 2;
		(*i)++;
	}
	return (ft_substr(str, start, *i - start));
}

t_token_type get_operator_type(char *str, int *i)
{
	if (str[(*i)] == '<' && str[(*i)+ 1] == '<')
	{
		(*i) += 2;
		return (HEREDOC);
	}
	else if (str[(*i)] == '>' && str[(*i)+ 1] == '>')
	{
		(*i) += 2;
		return (APPEND);
	}
	else if (str[(*i)] == '>')
	{
		(*i)++;
		return (REDIR_OUT);
	}
	else if (str[(*i)] == '<')
	{
		(*i)++;
		return (REDIR_IN);
	}
	else if (str[(*i)] == '|')
	{
		(*i)++;
		return (PIPE);
	}
	return (WORD);
}

char	*get_operator_str(t_token_type type)
{
	if (type == HEREDOC)
		return (ft_strdup("<<"));
	if (type == APPEND)
		return (ft_strdup(">>"));
	if (type == REDIR_IN)
		return (ft_strdup("<"));
	if (type == REDIR_OUT)
		return (ft_strdup(">"));
	if (type == PIPE)
		return (ft_strdup("|"));
	return (NULL);
}

t_token	*create_token(char *str, t_token_type type)
{
	t_token *new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	if (!new_token->str)
		return (NULL);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenize(t_data *data, char *str)
{
	t_token *new_token;
	t_token **current;
	t_token_type	type;
	int	i;

	new_token = NULL;
	current = &new_token;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if(!str[i])
			break ;
		if (is_operator(str[i]))
		{
			type = get_operator_type(str, &i);
			*current = create_token(get_operator_str(type), type);
		}
		else
		{
			type = WORD;
			*current = create_token(extract_word( str, &i), type);
		}
		if(!*current)
			return (NULL);
		current = &(*current)->next;
	}
	return (new_token);
}
