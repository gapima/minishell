#include "../../include/minishell.h"

void cd_not_directory(char *path)
{
	ft_putstr_fd("shellzin: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd(": Not a directory", 2);
}

void cd_not_found(char *path)
{
	ft_putstr_fd("shellzin: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd(": No such file or directory", 2);
}

void cd_permission_denied(char *path)
{
	ft_putstr_fd("shellzin: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putendl_fd(": Permission denied", 2);
}

char	*cd_check_path(char *path)
{
	char	*dir;
	int		status;

	dir = NULL;
	status = is_regular_file(path);
	if (status == 0)
		dir = path;
	else if (status == 1)
		cd_not_directory(path);
	else if (status == -1)
		cd_not_found(path);
	else if (status == -2)
		cd_permission_denied(path);
	return (dir);
}

void shellzin_cd(char **argv, t_shellzin *shell)
{
	char	*home;
	char	*dir;
	char	old_pwd[PATH_MAX];

	shell->last_status = 1;
	if (argv[1] && argv[2] != NULL)
	{
		ft_putendl_fd("shellzin: cd: too many arguments", 2);
		return ;
	}
	memcpy(old_pwd, shell->cwd, PATH_MAX);
	home = shellzin_env_search(shell, "HOME");
	dir = NULL;
	if (argv[1] == NULL)
		dir = home;
	else
		dir = cd_check_path(argv[1]); 
	if (dir && chdir(dir) == 0)
	{
		getcwd(shell->cwd, PATH_MAX);
		shellzin_set_or_change_env(shell, "PWD", shell->cwd);
		shellzin_set_or_change_env(shell, "OLDPWD", old_pwd);
		shell->last_status = 0;
	}
}
