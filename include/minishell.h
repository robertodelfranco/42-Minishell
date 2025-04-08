/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/04/08 13:43:44 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

# define SUCCESS 0
# define FAILURE 1

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define RESET "\033[0m"


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
}t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
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