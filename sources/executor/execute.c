#include "lexer.h"
#include "minishell.h"

static void	exe_solo(t_minishell *minishell, char *line)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_fail("Failed to fork");
	if (pid == 0)
		execute_command(line, minishell);
	else
		waitpid(pid, &minishell->exit_code, 0);
}

static void	exe_from_env(t_minishell *minishell, char *path, char **res,
		char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_fail("Failed to fork");
	if (pid == 0)
		execve(path, res, env);
	else
		waitpid(pid, &minishell->exit_code, 0);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 8) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute(t_minishell *minishell, char **env)
{
	t_cmd	*current;
	char	*path;
	char	**res;

	res = NULL;
	current = minishell->cmd;
	while (current != NULL)
	{
		res = array_init();
		*res = ft_strdup(current->cmd[0]);
		if (!res)
			exit_fail("Failed to allocate memory for result in execute");
		current = current->next;
		path = get_path(minishell, *res);
		if (is_builtin(*res))
			exe_solo(minishell, *res);
		else if (!path)
			not_found(minishell, *res);
		else if (path)
			exe_from_env(minishell, path, res, env);
	}
	//		free array of char *;
}