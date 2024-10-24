/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_deallocation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvoznese < kvoznese@student.42yerevan.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 06:26:08 by aaleksee          #+#    #+#             */
/*   Updated: 2024/09/29 01:34:51 by kvoznese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (cmd == NULL)
		return ;
	free_array(cmd->cmd);
	ft_lstclear(&cmd->files, free);
	free(cmd->whitespace);
	free_cmd(cmd->next);
	free(cmd);
}

void	free_tmp(t_mem *tmp)
{
	if (tmp->line)
	{
		free(tmp->line);
		tmp->line = NULL;
	}
	if (tmp->tokens)
	{
		free_array(tmp->tokens);
		tmp->tokens = NULL;
	}
	if (tmp->heredoc_tmp)
	{
		free_heredoc_tmp(tmp->heredoc_tmp);
		tmp->heredoc_tmp = NULL;
	}
	if (tmp->ws_tmp)
	{
		free(tmp->ws_tmp);
		tmp->ws_tmp = NULL;
	}
	if (tmp->ws_pipe_tmp)
	{
		free(tmp->ws_pipe_tmp);
		tmp->ws_pipe_tmp = NULL;
	}
}

void	free_minishell(t_minishell *minishell)
{
	if (!minishell)
		return ;
	if (minishell->cmd)
	{
		free_cmd(minishell->cmd);
		minishell->cmd = NULL;
	}
	if (minishell->tmp)
		free_tmp(minishell->tmp);
	if (minishell->buildins)
	{
		free(minishell->buildins);
		minishell->buildins = NULL;
	}
	if (minishell->path)
	{
		free(minishell->path);
		minishell->path = NULL;
	}
	if (minishell->tmp)
	{
		minishell->tmp->current_heredoc = 0;
		minishell->tmp->is_child = -1;
	}
}

void	exit_free(t_minishell *minishell, int exit_code)
{
	free_minishell(minishell);
	free(minishell->tmp);
	minishell->tmp = NULL;
	free_array(minishell->env);
	minishell->env = NULL;
	minishell = NULL;
	rl_clear_history();
	exit(exit_code);
}
