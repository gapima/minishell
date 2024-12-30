/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:37:26 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 17:38:19 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
#include "../include/minishell.h"

int	ft_min(int a, int b);
void ft_lst_destroy(t_list *list);
void *ft_realloc(void *m, size_t prev_size, size_t new_size);
#endif
