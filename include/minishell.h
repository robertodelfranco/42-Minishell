/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/03/19 16:36:51 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

# define SUCCESS 0
# define FAILURE 1

typedef struct s_data
{
    int exit_status;
}   t_data;

typedef struct s_node
{
    char    *content;
    int     type;
}   t_node;


typedef struct s_tree
{
    t_node          *node;
    struct s_tree   *rigth;
    struct s_tree   *left;
    
}   t_tree;

#endif