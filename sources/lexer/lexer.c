/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvoznese < kvoznese@student.42yerevan.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 06:22:25 by aaleksee          #+#    #+#             */
/*   Updated: 2024/10/08 18:31:29 by kvoznese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

char	**tokenizator(int **ws_array, char *line)
{
	int		i;
	int		token_flag;
	int		token_count;
	char	**result;

	i = 0;
	token_flag = 0;
	token_count = count_tokens(line);
	*ws_array = allocate_whitespaces(token_count, "Ws_array in tokenizator");
	result = allocate_array(token_count, "Result in tokenizator");
	while (*line && ft_isspace(*line))
		line++;
	while (i < token_count)
	{
		result[i] = extract_token(&line, i, ws_array, &token_flag);
		if (!result[i])
			error_array_allocation(result, i, "Result[i] in tokenizator");
		if (ft_strchr("|", result[i][0]) && i > 0)
			(*ws_array)[i - 1] = 0;
		i++;
	}
	result[token_count] = NULL;
	return (result);
}

int	lexer_main(t_minishell *minishell, char *line)
{
	if (!quote_counter(minishell, line))
		return (0);
	if (hanging_pipe_heredoc(line))
		minishell->tmp->tokens = pipe_heredoc_main(minishell, line);
	else
		minishell->tmp->tokens = tokenizator(&(minishell->tmp->ws_tmp), line);
	if (!validator_main(minishell, &minishell->tmp->tokens))
	{
		free_minishell(minishell);
		return (0);
	}
	expander_main(minishell, minishell->tmp->tokens);
	merger_main(minishell, minishell->tmp->tokens);
	parser_main(&minishell, &minishell->tmp->tokens);
	return (1);
}
