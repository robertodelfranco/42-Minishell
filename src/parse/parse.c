/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/19 19:03:15 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*free_program(t_data *data)
{
	ft_free_token_list(data);
	free(data->prompt);
	free(data);
	return (NULL);
}

void	print_list(t_data *data)
{
	printf("entrei\n");

	t_parse	*print;
	print = data->parse_list;
	while (print)
	{
		ft_printf("type = %d\n", print->node_type);
		int i = 0;
		while (print->cmd[i])
		{
			ft_printf("cmd = %s\n", print->cmd[i]);
			i++;
		}
		if (print->redir)
			ft_printf("target = %s, type = %d\n", print->redir->target, print->redir->type);
		print = print->next;
	}
}

void	*parse(t_data *data)
{
	if (data->nbr_of_quotes % 2 != 0)
		return (free_program(data));
	if (data->nbr_of_parentheses % 2 != 0)
		return (free_program(data));
	if (!validate_tokens(data))
		return (free_program(data));
	if (!parse_args(data))
		return (free_program(data));
	print_list(data);
	// if (!build_tree(data))
	// 	return (free_program(data));
	return (NULL);
}

bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	last = data->token_list;
	if (cur->type == PIPE || cur->type == AND || cur->type == OR)
		return (false);
	while(last->next)
		last = last->next;
	if (last->type == PIPE || last->type == AND || last->type == OR
			|| last->type == REDIR)
		return (false);
	// transform every if into a function
	while (cur)
	{
		if ((cur->type == PIPE && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL) || (cur->type == PIPE
					&& cur->next == NULL))
				{
					printf("Error: pipe with invalid target\n");
					return (false);
				}
		if ((cur->type == AND && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL && cur->next->type != WORD)
			|| (cur->type == AND && cur->next == NULL))
			return (false);
		if ((cur->type == OR && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL && cur->next->type != WORD)
			|| (cur->type == OR && cur->next == NULL))
			return (false);
		if (cur->type == REDIR)
			if (cur->next == NULL || (cur->next->type != WORD && cur
				->next->type != BUILT_IN && cur->next->type != EXTERNAL))
				{
					printf("Error: redirection with invalid target\n");
					return (false);
				}
		cur = cur->next;
	}
	return (true);
}

static t_type	ft_get_type(char *value)
{
	if (strcmp(value, "echo") == 0)
		return (ECHO);
	else if (strcmp(value, "pwd") == 0)
		return (PWD);
	else if (strcmp(value, "cd") == 0)
		return (CD);
	else if (strcmp(value, "env") == 0)
		return (ENV);
	else if (strcmp(value, "unset") == 0)
		return (UNSET);
	else if (strcmp(value, "export") == 0)
		return (EXPORT);
	else if (strcmp(value, "exit") == 0)
		return (EXIT);
	else
		return (EXTERNAL);
}

t_parse	*add_parse_list(t_data *data, char **args, t_type type)
{
	t_parse	*new_node;
	t_parse	*last;

	new_node = ft_calloc(1, sizeof(t_parse));
	if (!new_node)
		return (NULL);
	new_node->cmd = args;
	new_node->node_type = type;
	new_node->redir = NULL;
	new_node->next = NULL;
	if (data->parse_list == NULL)
		data->parse_list = new_node;
	else
	{
		last = data->parse_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	return (new_node);
}

t_type	ft_get_redir_type(char *redir)
{
	if (strcmp(redir, ">") == 0)
		return (IN_REDIR);
	else if (strcmp(redir, "<") == 0)
		return (OUT_REDIR);
	else if (strcmp(redir, ">>") == 0)
		return (APPEND);
	else
		return (HEREDOC);
}

t_redir	*create_redir(char *redir, char *target)
{
	t_redir	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->target = redir;
	new_redir->type = ft_get_redir_type(redir);
	new_redir->subshell = false;
	return (new_redir);
}

char	**get_operations(t_token *cur)
{
	char	**args;
	int		i;

	i = 1;
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cur->value);
	if (!args[0])
		return (NULL);
	return (args);
}

bool	parse_args(t_data *data)
{
	char	**args;
	t_parse	*node;
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (cur->type == PIPE || cur->type == AND || cur->type == OR)
			add_parse_list(data, get_operations(cur), cur->type);
		if ((cur->type == BUILT_IN || cur->type == EXTERNAL))
		{
			cur->type = ft_get_type(cur->value);
			args = get_arguments(cur);
			node = add_parse_list(data, args, cur->type);
		}
		if (cur->type == REDIR)
			if (node)
				node->redir = create_redir(cur->value);
		cur = cur->next;
	}
	// ft_free_token_list(data);
	return (true);
}


char	**get_arguments(t_token *cur)
{
	t_token	*count;
	char	**args;
	int		i;

	i = 1;
	count = cur->next;
	while (count && count->type == WORD)
	{
		i++;
		count = count->next;
	}
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	args[i++] = ft_strdup(cur->value);
	cur = cur->next;
	while (cur && cur->type == WORD)
	{
		args[i++] = ft_strdup(cur->value);
		if (!args[i - 1])
		return (NULL);
		cur = cur->next;
	}
	return (args);
}

// Tokens inesperados no início/fim: ex: começar ou terminar com | ou && ou redirecionamentos (<, >).

// Tokens duplicados sem conteúdo entre eles: ex: ||, && ou | |.

// Redirecionamentos sem target: ex: ls > ou cat <.

// Subshells mal fechados: parênteses abertos sem fechar ( ou ).


// if (current->type == WORD)
// 	ft_printf("WORD: %s\n", current->value);
// else if (current->type == PIPE)
// 	ft_printf("PIPE: %s\n", current->value);
// else if (current->type == IN_REDIR)
// 	ft_printf("IN_REDIR: %s\n", current->value);
// else if (current->type == OUT_REDIR)
// 	ft_printf("OUT_REDIR: %s\n", current->value);
// else if (current->type == AND)
// 	ft_printf("AND: %s\n", current->value);
// else if (current->type == OR)
// 	ft_printf("OR: %s\n", current->value);
// else if (current->type == HEREDOC)
// 	ft_printf("HEREDOC: %s\n", current->value);
// else if (current->type == APPEND)
// 	ft_printf("APPEND: %s\n", current->value);
// else if (current->type == OPEN_PAR)
// 	ft_printf("OPEN_PAR: %s\n", current->value);
// else if (current->type == CLOSE_PAR)
// 	ft_printf("CLOSE_PAR: %s\n", current->value);
// else if (current->type == SING_QUOTE)
// 	ft_printf("SING_QUOTE: %s\n", current->value);
// else if (current->type == DOUB_QUOTE)
// 	ft_printf("DOUB_QUOTE: %s\n", current->value);
// else if (current->type == EXPAND)
// 	ft_printf("EXPAND: %s\n", current->value);
// else if (current->type == BUILT_IN)
// 	ft_printf("BUILT_IN: %s\n", current->value);
// else if (current->type == EXTERNAL)
// 	ft_printf("EXTERNAL: %s\n", current->value);
// else
// 	ft_printf("UNKNOWN TOKEN: %s\n", current->value);
// current = current->next;