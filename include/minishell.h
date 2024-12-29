/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 18:16:30 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:37:49 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shellzin
{
	t_list	*env;
}	t_shellzin;

void	shellzin_handle_sigint(int sig);
void	shellzin_init(t_shellzin *shell, char *envp[]);
char	*shellzin_env_search(t_shellzin *shell, const char *key);
void	shellzin_deinit(t_shellzin *shell);

#endif
