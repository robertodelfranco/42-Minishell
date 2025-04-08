/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/04/08 17:48:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "../src/token/token.h"
# include "../lib/includes/libft.h"
# include "../lib/includes/ft_printf_bonus.h"

# define SUCCESS 0
# define FAILURE 1

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define RESET "\033[0m"


# define NOPRINTABLE "\t\n\v\f\r "


typedef struct s_token
{
	char				*value;
	t_type				type;
	struct s_token		*next;
}t_token;
typedef struct s_data
{
	char	*prompt;
	int		exit_status;
	t_token	*token_list;
}t_data;

typedef struct s_node
{
	char	*content;
	int		type;
}   t_node;

typedef struct s_tree
{
	t_node          *node;
	struct s_tree   *rigth;
	struct s_tree   *left;
	
}   t_tree;

#endif