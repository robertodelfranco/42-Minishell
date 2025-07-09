/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:57:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/09 14:55:27 by rheringe         ###   ########.fr       */
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
# include <signal.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../lib/includes/libft.h"
# include "../lib/includes/ft_printf_bonus.h"

# define SUCCESS 0
# define FAILURE 1
# define CMD_NOT_FOUND 127
# define COLOR "\001\033[1;36m\002"
# define RESET "\001\033[0m\002"
# define NOPRINTABLE "\t\n\v\f\r "
# define PATH_MAX 4096

extern int	g_sig;
typedef enum e_token_type
{
	PIPE = 1,
	REDIR,
	IN_REDIR,
	OUT_REDIR,
	APPEND,
	HEREDOC,
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
}	t_type;

typedef struct s_redir
{
	t_type			type;
	char			*target;
	bool			quoted;
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
	bool			quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char			*prompt;
	int				exit_status;
	int				fd[2];
	bool			unclosed_quote;
	t_token			*token_list;
	t_parse			*parse_list;
	t_redir			*redir_list;
	struct s_node	*exec_list;
	t_env			*env_list;
	t_env			*env_copy;
}	t_data;

typedef struct s_node
{
	char			**cmd;
	t_type			node_type;
	t_redir			*redir;
	int				fd_in;
	int				fd_out;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

//read
char	*ft_readline(t_data *data);

// Tokenizer
	// token
int		create_token(t_data *data);
void	add_token_list(t_data *data, char *value, t_type id_token);
	// token_utils
t_type	give_id_token(char *str);
t_type	get_command(char *token_name);
t_type	external_command(char *token_name);
t_token	*ft_last(t_token *lst);
	// token_handle_quotes
int		handle_word_token(t_data *data, int i);

// Parser
	// parse
bool	parse(t_data *data);
	// parse_list
bool	parse_args_list(t_data *data);
char	**get_arguments(t_token *cur);
	//parse_list_redirs
void	append_redir(t_redir **redir_list, t_token *cur);
void	verify_pipeline(t_data *data, t_token **cur);
	// parse_stack
bool	build_stack(t_data *data);
	// parse_types
t_type	ft_get_redir_type(char *redir);
t_type	ft_get_cmd_type(char *value);
bool	get_new_types(t_data *data);
int		ft_count_tokens(t_token *cur);
t_parse	*ft_last_parse(t_parse *lst);
	// parse_quotes
bool	handle_quotes(t_token *cur);

// Expansion
	//init_env
void	ft_init_env(t_data *data, char **env);
	//expand
int		get_exp_size(t_data *data, const char *str, bool heredoc);
char	*get_str_expand(t_data *data, char *input, char *expand, bool heredoc);
bool	ft_expand(t_data *data);
	//expand_utils
int		ft_ptr_len(char **str);
void	ft_free_key_and_value(char *key, char *value);
void	copy_value(char *str_expand, char *value, int *j);
char	*get_variable_value(t_data *data, char *str);
char	*get_variable_key(const char *str);
	// expand_quotes
int		jump_quotes(t_data *data, const char *str, int *size);
int		copy_quotes(t_data *data, char *input, char *str_expand, int *j);

// Executor
	// executor
bool	executor(t_data *data);
int		handle_redir_pipe_error(t_data *data, int fd[2],
			t_node *cur, int *prev);
	// exec_cmd
void	ft_dup_and_close(int fd, int dup, int clos);
bool	execute_built_in(t_data *data, t_node *cur);
bool	execute_external(t_data *data, t_node *cur);
int		get_execve_exit_code(char *cmd, char *full_path);
t_node	*get_last_command_node(t_node *cur);
	// exec_pipes
bool	execute_command(t_data *data, t_node *cur, char **env_array);
bool	exec_multiple_cmd(t_data *data, t_node *cur, int fd[2], int prev_fd);
	// exec_utils
void	dup_fds(t_node *cur);
int		ft_listsize(t_env *list);
bool	fd_restore(t_data *data, t_node *cur);
char	**get_env_array(t_env *env_list);
char	*ft_get_external_path(char *token_name);

// Built_ins
	// echo
bool	echo(t_data *data, char **args);
	// env
bool	env(t_data *data, char **args);
int		ft_envdup(t_data *data);
	// pwd
bool	pwd(t_data *data);
	// cd
bool	cd(t_data *data, char **argv);
	// exit
bool	b_exit(t_data *data, char **argv);
	// export
bool	b_export(t_data *data, char **argv);
void	print_export(t_data *data);
void	append_env_node(t_env **head, t_env *new_node);
t_env	*create_env_node(const char *key, const char *value);
	// unset
bool	b_unset(t_data *data, char **args);

// Redirects
	// redirs
bool	identify_redirs(t_redir *redir, t_node *node, t_data *data);
	// heredoc
bool	init_heredoc(t_redir *redir, t_node *node, t_data *data);
void	read_heredoc(t_redir *redir, char *delimiter, t_data *data, int fd);

// Signals
void	sigint_handler_prompt(int sig);
void	sigint_handler_exec(int sig);
void	sigpipe_handler(int sig);
void	signal_setup_prompt(void);
void	setup_signals_exec(void);
void	handle_heredoc(int sig);
void	heredoc_signal(void);

// Clear Program
	// free_list
void	ft_free_node_list(t_data *data);
void	ft_free_parse_list(t_data *data);
void	ft_free_token_list(t_data *data);
void	ft_free_env_list(t_env *env_list);
// void	ft_free_pids(pid_t *pids);
	// clean
bool	free_program(t_data *data, char *message);
bool	shutdown_program(t_data *data);
void	error_message(char *message);
void	ft_free_matrix(char **ptr_matrix);

#endif