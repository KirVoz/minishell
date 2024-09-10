#include "lexer.h"
#include "minishell.h"

static int  is_builtin(char *cmd)
{
    size_t cmd_len;

    cmd_len = ft_strlen(cmd);
    if (ft_strncmp(cmd, "echo", cmd_len) == 0 || ft_strncmp(cmd, "cd", cmd_len) == 0 || 
        ft_strncmp(cmd, "pwd", cmd_len) == 0 || ft_strncmp(cmd, "export", cmd_len) == 0 || 
        ft_strncmp(cmd, "unset", cmd_len) == 0 || ft_strncmp(cmd, "env", cmd_len) == 0 || 
        ft_strncmp(cmd, "exit", cmd_len) == 0)
        return (1);
    return (0);
}

static void free_pipes(int **pipes, int num_cmd)
{
    int i;

    i = 0;
    while (i < num_cmd - 1)
    {
        free(pipes[i]);
        i++;
    }
    free(pipes);
}

static void open_file(const char *filename, int flags, int std_fd)
{
    int fd;

    fd = open(filename, flags, 0644);
    if (fd == -1)
    {
        perror("open file");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, std_fd) == -1)
    {
        perror("dup2 file");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

static void redirect_input(t_cmd *current, int **pipes, int i)
{
    if (current->infile)
    {
        open_file(current->infile, O_RDONLY, STDIN_FILENO);
    }
    else if (i > 0)
    {
        if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
    }
}

static void redirect_output(t_cmd *current, int **pipes, int i, int num_cmd)
{
    if (current->outfile)
    {
        int flags = O_WRONLY | O_CREAT | (current->append ? O_APPEND : O_TRUNC);
        open_file(current->outfile, flags, STDOUT_FILENO);
    }
    else if (i < num_cmd - 1)
    {
        if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
        {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
    }
}

static void close_pipes(int **pipes, int num_cmd)
{
    int j;

    j = -1;
    while (++j < num_cmd - 1)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }
}

static void execute_child(t_minishell *minishell, t_cmd *current, int **pipes, int i, int num_cmd, char **env)
{
    redirect_input(current, pipes, i);
    redirect_output(current, pipes, i, num_cmd);
    close_pipes(pipes, num_cmd);

    if (is_builtin(current->cmd[0]))
        execute_command(current->cmd[0], minishell);
    else
    {
        if (execve(get_path(minishell, current->cmd[0]), current->cmd, env) == -1)
        {
            not_found(minishell, current->cmd[0]);
            exit(EXIT_FAILURE);
        }
    }
}

static int count_commands(t_cmd *cmd)
{
    int num_cmd = 0;
    while (cmd)
    {
        num_cmd++;
        cmd = cmd->next;
    }
    return num_cmd;
}

static int **setup_pipes(int num_cmd)
{
    int **pipes;
    int i;

    i = -1;
    pipes = malloc(sizeof(int *) * (num_cmd - 1));
    while (++i < num_cmd - 1)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    return pipes;
}

static pid_t *fork_processes(t_minishell *minishell, int num_cmd, int **pipes, char **env)
{
    t_cmd  *cur;
    pid_t *pids;
    int i;

    i = -1;
    cur = minishell->cmd;
    pids = malloc(sizeof(pid_t) * num_cmd);
    while (++i < num_cmd)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0)
        {
            execute_child(minishell, cur, pipes, i, num_cmd, env);
        }
        cur = cur->next;
    }
    return pids;
}

static void wait_for_processes(pid_t *pids, int num_cmd, t_minishell *minishell)
{
    int i;
    int status;
    int exit_code;
    
    exit_code = 0;
    i = 0;
    while (i < num_cmd)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
        {
            exit_code = WEXITSTATUS(status);
            if (exit_code != 0)
            {
                printf("Command failed with exit code: %d\n", exit_code);
            }
        }
        i++;
    }
    minishell->exit_code = exit_code;
}

static void execute_commands(t_minishell *minishell, char **env)
{
    int num_cmd = count_commands(minishell->cmd);
    int **pipes = NULL;
    pid_t *pids = NULL;

    if (num_cmd > 1)
    {
        pipes = setup_pipes(num_cmd);
    }

    pids = fork_processes(minishell, num_cmd, pipes, env);

    if (num_cmd > 1)
    {
        close_pipes(pipes, num_cmd);
        free_pipes(pipes, num_cmd);
    }

    wait_for_processes(pids, num_cmd, minishell);
    free(pids);
}

void execute(t_minishell *minishell, char **env)
{
    execute_commands(minishell, env);
}
