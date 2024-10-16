/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvoznese <kvoznese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:21:17 by kvoznese          #+#    #+#             */
/*   Updated: 2024/10/09 17:22:00 by kvoznese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_n(char *str, int ws, int cur_i)
{
	int	i;

	if (str[0] != '-' && cur_i == 1)
		return (3);
	if (ws == 1)
	{
		i = 1;
		if (str[0] == '-' || str[1] == 'n')
		{
			while (str[i++] == 'n')
				if (str[i] == '\0')
					return (1);
			return (0);
		}
	}
	else if (ws == 0)
	{
		i = 0;
		while (str[i++] == 'n')
			if (str[i] == '\0')
				return (1);
		return (0);
	}
	return (2);
}

int	check_n(t_minishell *minishell, char **cmd, int *i)
{
	int	is_valid;
	int	previous_is_valid;
	int	ws;
	int	j;

	is_valid = -1;
	ws = 1;
	j = *i;
	while (cmd[j])
	{
		previous_is_valid = is_valid;
		is_valid = is_valid_n(cmd[j], ws, j);
		if (is_valid == 1)
		{
			ws = minishell->cmd->whitespace[j];
			j++;
			continue ;
		}
		else if (((is_valid == 2 && is_valid != 3) || (is_valid == 0
					&& previous_is_valid == 1 && ws != 0)))
			break ;
		return (1);
	}
	*i = j;
	return (0);
}

void	execute_echo(t_minishell *minishell, int fd, t_cmd *cur)
{
	int	i;
	int	newline;
	int	file_fd;

	i = 1;
	(void)file_fd;
	if (cur->cmd[1])
		newline = check_n(minishell, cur->cmd, &i);
	else
		newline = 1;
	while (cur->cmd[i] != NULL)
	{
		ft_putstr_fd(cur->cmd[i], fd);
		while (cur->whitespace[i] > 0)
		{
			ft_putstr_fd(" ", fd);
			cur->whitespace[i]--;
		}
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", fd);
	minishell->exit_code = 0;
}
