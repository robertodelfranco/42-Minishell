/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/04/09 15:27:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include "../lib/includes/libft.h"
# include "../lib/includes/ft_printf_bonus.h"

# define SUCCESS 0
# define FAILURE 1

# define NOPRINTABLE "\t\n\v\f\r "

typedef enum e_token_type
{
	AND = 1,
	OR,
	PIPE,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
	COMMAND,
	WORD,
	EXPAND,
	SING_QUOTE,
	DOUB_QUOTE,
	OPEN_PAR,
	CLOSE_PAR,
}	t_type;

typedef enum e_tree_type
{
	OP_NODE = 1, // Operation -> and, or
	PIPE_NODE,
	CMD_NODE,
	SUBSHELL,
}	t_tree_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_type	type; // -> in_redir, out_redir, append, heredoc
	char	*target;
}	t_redir;

typedef struct s_data
{
	char	*prompt;
	int		exit_status;
	t_token	*token_list;
}	t_data;

typedef struct s_node
{
	char			**command;
	t_tree_type		type;
	bool			subshell;
	t_redir			*redir;
	struct s_node	*right;
	struct s_node	*left;
}	t_node;

#endif