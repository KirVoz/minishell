/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvoznese <kvoznese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 05:10:15 by aaleksee          #+#    #+#             */
/*   Updated: 2024/09/30 19:29:49 by kvoznese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	not_found(t_minishell *minishell, char *cmd)
{
	char	*error;

	error = construct_error(cmd, ": command not found\n", NULL, 0);
	ft_putstr_fd(error, 2);
	free(error);
	minishell->exit_code = 127;
	if (minishell->tmp->is_child != 0)
		exit(minishell->exit_code);
}

void	no_file(t_minishell *minishell, char *filename)
{
	char	*error;

	error = construct_error(filename,
			": No such file or directory\n", NULL, 's');
	ft_putstr_fd(error, 2);
	free(error);
	minishell->exit_code = 1;
	if (minishell->tmp->is_child != 0)
		exit(minishell->exit_code);
}

void	no_path_file(t_minishell *minishell, char *filename)
{
	char	*error;

	error = construct_error(filename, ": No such file or directory\n", NULL, 0);
	ft_putstr_fd(error, 2);
	free(error);
	minishell->exit_code = 127;
	if (minishell->tmp->is_child != 0)
		exit(minishell->exit_code);
}

void	not_valid(t_minishell *minishell, char *cmd)
{
	char	*error;

	error = construct_error("export: ", cmd, ": not a valid identifier\n", 0);
	ft_putstr_fd(error, 2);
	free(error);
	minishell->exit_code = 1;
	if (minishell->tmp->is_child != 0)
		exit(minishell->exit_code);
}
