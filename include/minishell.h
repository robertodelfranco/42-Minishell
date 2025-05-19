/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/05/19 18:23:52 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../lib/includes/libft.h"
# include "../lib/includes/ft_printf_bonus.h"

# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127

# define NOPRINTABLE "\t\n\v\f\r "

typedef enum e_token_type
{
	AND = 1,
	OR,
	PIPE,
	REDIR,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
	COMMAND,
	EXPAND,
	SING_QUOTE,
	DOUB_QUOTE,
	OPEN_PAR,
	CLOSE_PAR,
	BUILT_IN,
	EXTERNAL,
	WORD,
	ECHO,
	PWD,
	CD,
	ENV,
	UNSET,
	EXPORT,
	EXIT
}	t_type;

typedef enum e_tree_type
{
	OP_NODE = 1, // Operation -> and, or
	PIPE_NODE,
	CMD_NODE,
	SUBSHELL,
}	t_tree_type;

typedef struct s_redir
{
	t_type	type; // -> in_redir, out_redir, append, heredoc
	char	*target;
	bool	subshell; // -> if it is a subshell
}	t_redir;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_parse
{
	t_type			node_type;
	char			**cmd;
	t_redir			*redir;
	struct s_parse	*next;
}	t_parse;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char	*prompt;
	int		nbr_of_quotes;
	int		nbr_of_parentheses;
	int		exit_status;
	bool	exit;
	t_token	*token_list;
	t_parse	*parse_list;
	t_env	*env_list;
}	t_data;

typedef struct s_node
{
	char			**cmd;
	t_tree_type		node_type;
	t_type			command_type;
	bool			subshell;
	t_redir			*redir;
	struct s_node	*right;
	struct s_node	*left;
}	t_node;

//read
char	*ft_readline(void);

//inits
t_redir	*create_redir_node(t_type type, char *target, bool subshell);
t_node	*create_pipe_node(t_node *cmd1, t_node *cmd2, bool subshell);
t_node	*create_cmd_node(char **prompt, t_redir *redir, bool subshell);

// tokens
int		create_token(t_data *data);
t_type	get_command(char *token_name);
t_type	external_command(char *token_name);

// parsing
void	*parse(t_data *data);
bool	validate_tokens(t_data *data);
bool	parse_args(t_data *data);
char	**get_arguments(t_token *cur);

// clean
void	ft_free_token_list(t_data *data);
void	ft_free_matrix(char **ptr_matrix);

#endif