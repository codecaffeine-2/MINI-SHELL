/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/23 10:20:40 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_cd(t_tools *tools)
{
	struct stat	st;

	if (tools->cmd->args[1] == NULL || !ft_strcmp("~", tools->cmd->args[1]))
		return (handle_home(tools));
	else if (!ft_strcmp(tools->cmd->args[1], "-"))
		return (handle_old_path(tools));
	else if (stat(tools->cmd->args[1], &st) != 0)
	{
		cd_error(tools->cmd->args[1]);
		return (1);
	}
	else
	{
		if (chdir(tools->cmd->args[1]) == -1)
		{
			cd_error(tools->cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(tools->env, tools->aloc, tools->cmd->args[1], tools);
		}
	}
	return (0);
}

int	ft_export(t_tools *tools)
{
	int		i;
	int		status;
	char	*value;
	char	*key;

	i = 0;
	while (tools->cmd->args[++i])
	{
		key = ft_split(tools->cmd->args[i], '=', tools->aloc)[0];
		value = ft_strchr(tools->cmd->args[i], '=');
		if (value)
			value++;
		status = treat_variable(tools, key, value, i);
	}
	if (i == 1)
		export_display(tools->env, tools->aloc);
	return (status);
}

int	ft_env(t_malloc **aloc, t_env **env, t_cmd *cmd)
{
	t_env	*ptr;

	(void)aloc;
	ptr = *env;
	if (cmd->args[1])
	{
		ft_putstr_fd("env:", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("too many arguments\n", 2);
		return (1);
	}
	while (ptr)
	{
		if (ptr->value && is_key_valid(ptr->key))
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (cmd->args[++i])
	{
		if (is_var_exist(cmd->args[i], *env))
			remove_variable(cmd->args[i], env, aloc);
		if (!is_key_valid(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
	}
	return (status);
}


void	ft_exit(t_malloc **aloc, t_cmd *cmd, int *status)
{
	char		*s;
	long long	i;

	printf("exit\n");
	if (cmd->args[1])
	{
		if (cmd->args[2])
			failed_exit_args_num(status);
		else
		{
			s = ft_isnum(cmd->args[1], aloc);
			if (s)
			{
				i = ft_atoi(s);
				if (errno == ERANGE)
					exit_numeric_error(s, aloc);
				clean_up(aloc);
				exit(i % 256);
			}
			else
				exit_numeric_error(cmd->args[1], aloc);
		}
	}
	else
		return (clean_up(aloc), exit(*status));
}
