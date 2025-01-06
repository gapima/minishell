#include "../../include/minishell.h"

void	shellzin_exit(char **argv, t_shellzin *shell)
{
	int	status;

	status = 0;
	if (argv[1])
	{
		status = ft_atoi(argv[1]);
		if (status == 0 && argv[1][0] != '0')
		{
			ft_printf_fd(2, "shellzin: exit: numeric argument required\n");
			status = 2;
		}
		if (argv[2])
		{
			ft_printf_fd(2, "shellzin: exit: too many arguments\n");
			shell->last_status = 1;
			return ;
		}
	}
	ast_deinit(shell->ast);
	parser_deinit(&shell->parser);
	shellzin_deinit(shell);
	ft_putendl_fd("exit", 2);
	exit(status);
}
