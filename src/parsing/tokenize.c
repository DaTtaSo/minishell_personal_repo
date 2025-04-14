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
	data->token = NULL;
	data->env = NULL;
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

t_token_type get_operator_type(char *str, int *i)
{
	if (str[(*i)++] == '<' && str[(*i) + 2] == '<')
		return (HEREDOC);
	else if (str[(*i)++] == '>' && str[(*i) + 2] == '>')
		return (APPEND);
	else if (str[(*i)++] == '>')
		return (REDIR_IN);
	else if (str[(*i)++] == '<')
		return (REDIR_OUT);
	else if (str[(*i)++] == '|')
		return (HEREDOC);
	return (WORD);
}

char	*get_operator_str(t_token_type type)
{
	if (type == HEREDOC)
		return ("<<");
	if (type == APPEND)
		return (">>");
	if (type == REDIR_OUT)
		return ("<");
	if (type == REDIR_IN)
		return (">");
	if (type == PIPE)
		return ("|");
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

t_token *create_token(char *str, t_token_type type)
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

t_token *tokenize(t_data *data, char *str)
{
	t_token *tokens;
	t_token *current;
	t_token *new_token;
	t_token_type	type;
	int	i;
	char *word;
	char *symbol;

	new_token = NULL;
	tokens = NULL;
	current = NULL;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		type = get_operator_type(str, &i);
		if (is_operator(str[i]))
			new_token = create_token(get_operator_str(type), type);
		else
			word = extract_word();
		if (!word || !new_token)
			return (free(tokens), NULL);
	}
}
