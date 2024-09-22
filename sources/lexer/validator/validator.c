#include "lexer.h"
#include "minishell.h"

int	pipe_redirections_mistake(t_minishell *minishell, char **tokens)
{
	int	pipes;
	int	right_redirs;
	int	left_redirs;

	while (*tokens)
	{
		delimiter_counter(tokens, &pipes, &right_redirs, &left_redirs);
		if (pipes > 1)
			return (syntax_error(minishell, "|"));
		else if (right_redirs == 3)
			return (syntax_error(minishell, ">"));
		else if (right_redirs > 3)
			return (syntax_error(minishell, ">>"));
		else if (left_redirs == 3 || left_redirs == 4)
			return (syntax_error(minishell, "<"));
		else if (left_redirs == 5)
			return (syntax_error(minishell, "<<"));
		else if (left_redirs > 5)
			return (syntax_error(minishell, "<<<"));
		tokens++;
	}
	return (1);
}
int	checek_two_tokens(char *token, char *next_token)
{
	if (ft_strncmp(token, ">", 1) == 0 && ft_strncmp(next_token, ">", 1) == 0)
		return (1);
	if (ft_strncmp(token, "<", 1) == 0 && ft_strncmp(next_token, "<", 1) == 0)
		return (1);
	return (0);
}

int	redirections_unification(char **tokens)
{
	int		unified;
	char	a;
	int		i;

	unified = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i + 1] && checek_two_tokens(tokens[i], tokens[i + 1]))
		{
			a =	tokens[i][0];
			free(tokens[i]);
			if (a == '>')
				tokens[i] = ft_strdup(">>");
			else
				tokens[i] = ft_strdup("<<");
			free(tokens[i + 1]);
			tokens[i + 1] = ft_strdup("");
			unified = 1;
		}
		i++;
	}
	return (unified);
}

int	hanging_redirections(char **tokens)
{
	size_t	token_len;

	while (*tokens)
	{
		token_len = ft_strlen(*tokens);
		if ((ft_strncmp(*tokens, "<", token_len) == 0
				|| ft_strncmp(*tokens, ">", token_len) == 0
				|| ft_strncmp(*tokens, "<<", token_len) == 0
				|| ft_strncmp(*tokens, ">>", token_len) == 0) && !*(tokens + 1)
				&& token_len != 0)
			return (0);
		tokens++;
	}
	return (1);
}

int	validator_main(t_minishell *minishell, char ***tokens)
{
	if (!pipe_redirections_mistake(minishell, *tokens))
		return (0);
	if (redirections_unification(*tokens))
		*tokens = tokens_realloc(tokens);
	if (!hanging_redirections(*tokens))
		return (syntax_error(minishell, "newline"));
	return (1);
}