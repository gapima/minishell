#include "../../include/minishell.h"

void shellzin_unset(char **argv, t_shellzin *shell)
{
	size_t idx;
	t_list *node;
	t_list *prev;

	idx = 1;
	prev = NULL;
	while (argv[idx])
	{
		node = shellzin_env_search_node(shell, argv[idx++], &prev, true);
		if (node)
		{
			if (prev)
				prev->next = node->next;
			else if (shell->env)
				shell->env = shell->env->next;
			free(node->content);
			free(node);
		}
	}
	shell->last_status = 0;
}
