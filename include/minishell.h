/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/05/26 17:55:26 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/wait.h>
// # include <signal.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../lib/includes/libft.h"
# include "../lib/includes/ft_printf_bonus.h"

# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127
# define COLOR "\033[1;36m"
# define RESET "\033[0m"
# define NOPRINTABLE "\t\n\v\f\r "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef enum e_token_type
{
	PIPE = 1,
	REDIR,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
	EXPAND,
	SING_QUOTE,
	DOUB_QUOTE,
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

typedef struct s_redir
{
	t_type	type;
	char	*target;
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
	char			*prompt;
	int				double_quotes;
	int				single_quotes;
	int				exit_status;
	bool			exit;
	t_token			*token_list;
	t_parse			*parse_list;
	struct s_node	*exec_list;
	t_env			*env_list;
	t_env			*env_copy;
}	t_data;

typedef struct s_node
{
	char			**cmd;
	t_type			node_type;
	t_redir			*redir;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

//read
char	*ft_readline(void);

// token
int		create_token(t_data *data);
void	get_token(t_data *data, int start);
void	add_token_list(t_data *data, char *token_name, t_type id_token);

// token_utils
t_type	external_command(char *token_name);
t_type	give_id_token(char *str);
t_type	get_command(char *token_name);
t_token	*ft_last(t_token *lst);

// executor
bool	executor(t_data *data);

// parse
bool	parse(t_data *data);
bool	validate_tokens(t_data *data);

// parse_utils
t_type	ft_get_redir_type(char *redir);
t_type	ft_get_cmd_type(char *value);

// parse_list
bool	parse_args(t_data *data);
t_parse	*add_parse_list(t_data *data, char **args, t_type type);
t_redir	*create_redir(char *redir, char *target);
char	**get_operations(t_token *cur);
char	**get_arguments(t_token **cur);

// parse_stack
bool	build_stack(t_data *data);
void	add_stack_node(t_data *data, t_node *node);
t_node	*create_pipe_node(t_type type);
t_node	*create_cmd_node(char **prompt, t_redir *redir, t_type type);

// expansion
void	ft_init_env(t_data *data, char **env);
void	add_env_list(t_data *data, t_env *new_node);

// pipes
void	ft_pipes(t_data *data);

// built_ins
	// echo
int		echo(char **args);
	// env
void	env(t_data *data, char **args);
	// pwd
void	pwd(t_data *data);
	// cd
void	cd(t_data *data, char **argv);
	// exit
void	b_exit(t_data *data, char **argv, pid_t *pids);

// free_list
void	ft_free_env_list(t_data *data);
void	ft_free_node_list(t_data *data);
void	ft_free_parse_list(t_data *data);
void	ft_free_token_list(t_data *data);
void	ft_free_pids(pid_t *pids);

// clean
bool	free_program(t_data *data, char *message);
void	error_message(char *message);
void	ft_free_matrix(char **ptr_matrix);

#endif