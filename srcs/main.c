/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:03:00 by alarroye          #+#    #+#             */
/*   Updated: 2025/04/14 12:19:36 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


//int	main(int ac, char **av, char **env)
//{
//	t_data data;
//	char	*cwd;
//	char *read;
//	t_token_type
//
//	init_data(&data, ac, av);
//	cpy_env(env);
//	while (1)
//	{
//		getcwd(cwd, 1024);
//		read = readline("cwd>");
//		if (!read)
//			break ;
//		add_history(read);
//		if (!read[0])
//			continue ;
////		token = tokenize(read);
//		}
//	}
//}

t_list *cpy_env(char **env)
{
	int	i;
	int	j;
	char *name;
	char *content;
	t_list *env_cpy;

	env_cpy = NULL;
	i = 0;
	j = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		name = ft_substr(env[i], 0, j);
		if (env[i][j] == '=')
			content = ft_strdup(&env[i][j + 1]);
		ft_lstadd_back(&env_cpy, ft_lstnew(name, content));
		i++;
	}
	return (env_cpy);
}

void	print_env_list(t_list *env)
{
	while (env)
	{
		printf("name: %-15s | value: %s\n", env->name, env->content);
		env = env->next;
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_list *env_cpy = cpy_env(env);
	print_env_list(env_cpy);
	return (0);
}