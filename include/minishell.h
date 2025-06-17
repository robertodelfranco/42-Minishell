/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/06/17 16:15:48 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
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
	EXIT,
	DOC_WORD,
}	t_type;

typedef struct s_redir
{
	t_type			type;
	char			*target;
	struct s_redir	*next;
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
	int				fd[2];
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
char	*ft_readline(t_data *data, char **env);

// Tokenizer
	// token
int		create_token(t_data *data);
void	get_token(t_data *data, int start);
void	add_token_list(t_data *data, char *token_name, t_type id_token);
	// token_utils
t_type	external_command(char *token_name);
t_type	give_id_token(char *str);
t_type	get_command(char *token_name);
t_token	*ft_last(t_token *lst);

// Parser
	// parse
bool	parse(t_data *data);
	// parse_utils
t_type	ft_get_redir_type(char *redir);
t_type	ft_get_cmd_type(char *value);
int		ft_count_tokens(t_token *cur);
void	get_redirs(t_parse *node, t_token **cur);
void	get_redir_node(t_data *data, t_token **tokens);
	// parse_list
bool	parse_args_list(t_data *data);
t_parse	*add_parse_list(t_data *data, char **args, t_type type);
void	append_redir(t_redir **redir_list, t_token *cur);
char	**get_operations(t_token *cur);
char	**get_arguments(t_token *cur);
	// parse_stack
bool	build_stack(t_data *data);
void	add_stack_node(t_data *data, t_node *node);
t_node	*create_pipe_node(t_type type);
t_node	*create_cmd_node(char **prompt, t_redir *redir, t_type type);

// Expansion
	//init_env
void	ft_init_env(t_data *data, char **env);
void	add_env_list(t_data *data, t_env *new_node);
	//expand
char	*get_variable_value(t_data *data, char *str);
char	*get_variable_key(const char *str, int *len);
int		get_expand_size(t_data *data, const char *str);
char	*get_str_expanded(t_data *data, t_token *cur, char *expanded);
bool	ft_expand(t_data *data);
	//expand_utils
int		ft_ptr_len(char **str);
int		ft_strchr_count(char const *str, char c);
void	ft_free_key_and_value(char *key, char *value);
void	copy_value(char *str_expand, char *value, int *j);

// Executor
bool	execute_one_command(t_data *data, t_node *cur);
bool	executor(t_data *data);
	// exec_cmd
bool	execute_built_in(t_data *data, t_node *cur);
bool	execute_external(t_data *data, t_node *cur);
	// exec_utils
int		ft_listsize(t_env *list);
char	**get_env_array(t_env *env_list);
char	*ft_get_external_path(char *token_name);
	// exec_pipes
void	ft_dup_and_close(int fd, int dup, int clos);
bool	execute_first_command(t_data *data, t_node *cur);
bool	execute_last_command(t_data *data, t_node *cur);
bool	execute_middle_command(t_data *data, t_node *cur);

// Built_ins
	// echo
int		echo(char **args);
	// env
void	env(t_data *data, char **args);
	// pwd
void	pwd(t_data *data);
	// cd
void	cd(t_data *data, char **argv);
	// exit
void	b_exit(t_data *data, char **argv);
	//
void	b_export(t_data *data, char **argv);

// Redirects
bool	identify_redirs(t_redir *redir, t_data *data);

// Clear Program
	// free_list
void	ft_free_env_list(t_data *data);
void	ft_free_node_list(t_data *data);
void	ft_free_parse_list(t_data *data);
void	ft_free_token_list(t_data *data);
// void	ft_free_pids(pid_t *pids);
	// clean
bool	free_program(t_data *data, char *message);
void	error_message(char *message);
void	ft_free_matrix(char **ptr_matrix);

void	print_list(t_data *data);
void	print_command(t_parse *print);

#endif