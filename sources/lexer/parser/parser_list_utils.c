#include "lexer.h"
#include "minishell.h"

void	process_node(t_cmd **current, t_cmd **cmd_list, char *token)
{
	t_cmd	*command_node;
	t_cmd	*last;

	if (!*current || if_pipe(token))
	{
		command_node = init_cmd_node();
		if (!*cmd_list)
			*cmd_list = command_node;
		else
		{
			last = *cmd_list;
			while (last->next)
				last = last->next;
			last->next = command_node;
			command_node->inpipe = 1;
			last->outpipe = 1;
		}
		*current = command_node;
	}
}

void	add_command(t_cmd *current, char *token)
{
	int		command_len;
	char	**command;

	if (!current)
		exit_fail("Empty current node in add_command");
	command_len = 0;
	if (current->cmd)
	{
		while (current->cmd[command_len])
			command_len++;
	}
	command = (char **)ft_realloc(current->cmd, command_len * sizeof(char *),
	(command_len + 2) * sizeof(char *));
	if (!command)
		exit_fail("Failed to allocate memory for command");
	current->cmd = command;
	current->cmd[command_len] = ft_strdup(token);
	if (!current->cmd[command_len])
		exit_fail("Failed to duplicate token in add_command");
	current->cmd[command_len + 1] = NULL;
}