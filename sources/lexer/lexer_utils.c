#include "lexer.h"
#include "minishell.h"

char	*quote_counter(char *line)
{
	int		double_q;
	int		single_q;
	int		in_double_q;

	double_q = 0;
	single_q = 0;
	in_double_q = 0;
	while (*line)
	{
		if (ft_strchr("\"", *line))
		{
			double_q++;
			if (in_double_q)
				in_double_q = 0;
			else
				in_double_q = 1;
		}
		else if (ft_strchr("\'", *line) && !in_double_q)
			single_q++;
		line++;
	}
	if (double_q % 2 != 0)
		return ("\"");
	else if (single_q % 2 != 0)
		return ("'");
	return (NULL);
}

char	hanging_pipe_heredoc(char *line)
{
	if (ft_strchr(line, '<')
			&& ft_strncmp(ft_strchr(line, '<'), "<<", 2) == 0
			&& ft_strncmp(ft_strchr(line, '<') + 2, "<", 1) != 0)
		return ('h');
	else if (ft_strrchr(line, '|')
			&& !(*(ft_strrchr(line, '|') + 1)))
		return ('p');
	return (0);
}