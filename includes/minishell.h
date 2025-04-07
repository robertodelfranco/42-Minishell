/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/03/21 17:46:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <stdlib.h>
# include "../lib/includes/libft.h"

# define SUCCESS 0
# define FAILURE 1

typedef struct s_data
{
	int	exit_status;
}	t_data;

typedef struct s_node
{
	char	*content;
	int		type;
}	t_node;


typedef struct s_tree
{
	t_node			*node;
	struct s_tree	*rigth;
	struct s_tree	*left;
}	t_tree;

typedef struct s_cmd
{
	char	*cmd;
	char	*flags;
	char	*path;
	char	**args;
	char	**env;
	char	*input;
	char	*output;
	char	*redirect;
	int		exit_status;    
}	t_cmd;

#endif