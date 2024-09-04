#include "minishell.h"

static void ft_free_split(char **split)
{
    int i;

    i = 0;
    if (!split)
        return;

    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

static char **get_paths(t_list *envp_var)
{
    t_list *tmp = envp_var;
    char **paths = NULL;

    while (tmp)
    {
        if (ft_strncmp(tmp->content, "PATH=", 5) == 0)
        {
            paths = ft_split(tmp->content + 5, ':');
            break;
        }
        tmp = tmp->next;
    }
    return paths;
}

static char *find_executable(char **paths, char *cmd)
{
    char *part_path;
    char *path;
    int i;

    i = 0;
    while (paths[i])
    {
        part_path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(part_path, cmd);
        free(part_path);
        if (access(path, F_OK) == 0)
            return path;
        free(path);
        i++;
    }
    return NULL;
}

char *get_path(t_minishell *minishell, char *cmd)
{
    char **paths;
    char *path;

    if (*cmd == '\0')
        return (NULL);
    paths = get_paths(minishell->env->envp_var);
    if (paths == NULL)
        return (NULL);
    path = find_executable(paths, cmd);
    ft_free_split(paths);
    return (path);
}

