/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alarroye <alarroye@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:04:32 by alarroye          #+#    #+#             */
/*   Updated: 2025/07/22 04:38:13 by alarroye         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env(t_list **env, char *pwd, char *export, int error)
{
	char	**pwd_exported;
	char	*path_pwd;

	pwd_exported = malloc(sizeof(char *) * 3);
	pwd_exported[2] = NULL;
	pwd_exported[0] = strdup("export");
	if (!pwd_exported[0])
	{
		ft_free_dtab(pwd_exported);
		return (ft_error_msg("cd", "malloc failed in pwd export"));
	}
	path_pwd = ft_strjoin(export, pwd);
	if (!path_pwd)
	{
		ft_free_dtab(pwd_exported);
		return (ft_error_msg("cd", "ft_strjoin : malloc failed path_pwd"));
	}
	pwd_exported[1] = path_pwd;
	error = ft_export(env, pwd_exported);
	ft_free_dtab(pwd_exported);
	if (error)
		return (error);
	return (0);
}

int	fd_cd_not_arg(t_list **env, char *buf, int error_update)
{
	char	*pwd;
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env && ft_strcmp(tmp_env->name, "HOME"))
		tmp_env = tmp_env->next;
	if (!tmp_env)
		return (ft_error_msg("cd", "HOME not set"));
	pwd = getcwd(buf, PATH_MAX);
	if (!pwd)
		return (ft_perror_msg("cd: getcwd", 1));
	if (chdir(tmp_env->content) == -1)
		return (ft_perror_msg("cd: chdir", 1));
	error_update = update_env(env, tmp_env->content, "PWD=", error_update);
	if (error_update)
		return (error_update);
	error_update = update_env(env, pwd, "OLDPWD=", error_update);
	if (error_update)
		return (error_update);
	return (0);
}

int	ft_cd(t_list **env, char **cmd)
{
	char	*pwd;
	char	buf[PATH_MAX];
	int		error_update;

	error_update = 0;
	if (cmd && !cmd[1])
		return (fd_cd_not_arg(env, buf, error_update));
	else if (cmd && cmd[2])
		return (ft_error_msg("cd", "too many arguments"));
	pwd = getcwd(buf, PATH_MAX);
	if (!pwd)
		return (ft_perror_msg("cd: getcwd", 1));
	if (chdir(cmd[1]) == -1)
		return (ft_perror_msg("cd: chdir", 1));
	error_update = update_env(env, pwd, "OLDPWD=", error_update);
	if (error_update)
		return (error_update);
	pwd = getcwd(buf, PATH_MAX);
	if (!pwd)
		return (ft_perror_msg("cd: getcwd", 1));
	error_update = update_env(env, pwd, "PWD=", error_update);
	if (error_update)
		return (error_update);
	return (error_update);
}
