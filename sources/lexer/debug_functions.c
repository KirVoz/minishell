#include "lexer.h"
#include "minishell.h"
#include <stdio.h>

void	print_list_state(t_minishell *minishell, char *name)
{
	t_cmd	*current;
	int		cmd_index;
	t_list	*skipped_in;
	t_list	*skipped_out;

	current = minishell->cmd;
	cmd_index = 0;
	printf("\n*LIST STATE %s:\n", name);
	while (current != NULL)
	{
		printf("Command %d:\n", cmd_index);
		for (int arg_index = 0; current->cmd[arg_index] != NULL; arg_index++)
		{
			printf(" Arg %d:\n   cmd:        %s\n   infile:     %s\n   outfile:    %s\n",
			arg_index, current->cmd[arg_index], current->infile, current->outfile);
			printf("   s_infiles:  ");
			skipped_in = current->skipped_in;
			while (skipped_in != NULL)
			{
				printf("%s ", (char *)skipped_in->content);
				skipped_in = skipped_in->next;
			}
			printf("\n");
			printf("   s_oufiles:  ");
			skipped_out = current->skipped_out;
			while (skipped_out != NULL)
			{
				printf("%s, ", (char *)skipped_out->content);
				skipped_out = skipped_out->next;
			}
			printf("\n");
			printf("   is_append: %d, inpipe: %d, outpipe: %d\n",
				current->append, current->inpipe, current->outpipe);
		}
		for (int i = 0; current->heredoc && current->heredoc[i]; i++)
		{
			printf(" Heredoc: %s\n", current->heredoc[i]);
		}
		current = current->next;
		cmd_index++;
	}
	printf("\n");
}

void	print_tokens_state(char **tokens, char *name)
{
	int	i;

	i = 0;
	printf("\n*TOKENS STATE %s:\n", name);
	while (*tokens)
		printf("  Token %d: %s\n", i++, *(tokens++));
	printf("\n");
}
