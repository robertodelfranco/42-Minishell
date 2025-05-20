/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:48:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/05/20 10:11:38 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_token(t_data *data)
{
	int	i;

	i = 0;
	while (ft_strchr(NOPRINTABLE, data->prompt[i]) != NULL
		&& data->prompt[i] != '\0')
		i++;
	while (data->prompt[i] != '\0')
	{
		// if (data->prompt[i] == '"')
		// 	get_quotes(data, i);
		if (!ft_strchr(NOPRINTABLE, data->prompt[i]))
		{
			get_token(data, i);
			while (data->prompt[i] && !ft_strchr(NOPRINTABLE, data->prompt[i]))
				i++;
		}
		else
			i++;
	}
	return (i);
}

void	get_token(t_data *data, int start)
{
	int		end;
	char	*token_name;
	t_type	id_token;

	end = start;
	while (data->prompt[end] && !ft_strchr(NOPRINTABLE, data->prompt[end]))
		end++;
	token_name = ft_substr(data->prompt, start, end - start);
	id_token = give_id_token(token_name);
	add_token_list(data, token_name, id_token);
	free(token_name);
}

t_type	give_id_token(char *str)
{
	if (strcmp(str, "&&") == 0)
		return (AND);
	if (strcmp(str, "||") == 0)
		return (OR);
	if (strcmp(str, "|") == 0)
		return (PIPE);
	if (strcmp(str, "<<") == 0)
		return (REDIR);
	if (strcmp(str, ">>") == 0)
		return (REDIR);
	if (strcmp(str, "<") == 0)
		return (REDIR);
	if (strcmp(str, ">") == 0)
		return (REDIR);
	if (strcmp(str, "(") == 0)
		return (OPEN_PAR);
	if (strcmp(str, ")") == 0)
		return (CLOSE_PAR);
	if (str[0] == '\'' && str[ft_strlen(str)-1] == '\'')
		return (SING_QUOTE);
	if (str[0] == '"' && str[ft_strlen(str)-1] == '"')
		return (DOUB_QUOTE);
	if (str[0] == '$')
		return (EXPAND);
	return (get_command(str));
}

void	add_token_list(t_data *data, char *token_name, t_type id_token)
{
	t_token	*new_token;
	t_token	*last;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return ;
	if (token_name)
	{
		new_token->value = ft_strdup(token_name);
		if (!new_token->value)
			return (free(new_token));
	}
	else
		new_token->value = NULL;
	new_token->type = id_token;
	new_token->next = NULL;
	if (data->token_list == NULL)
		data->token_list = new_token;
	else
	{
		last = data->token_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
}

t_type	get_command(char *token_name)
{
	if (strcmp(token_name, "echo") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "cd") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "pwd") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "export") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "unset") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "env") == 0)
		return (BUILT_IN);
	else if (strcmp(token_name, "exit") == 0)
		return (BUILT_IN);
	else if (external_command(token_name) == 1)
		return (EXTERNAL);
	else
		return (WORD);
}

t_type	external_command(char *token_name)
{
	int		i;
	char	*full_cmd;
	char	*full_path;
	char	**new_path;

	new_path = ft_split(getenv("PATH"), ':');
	if (new_path == NULL)
		return (0);
	i = 0;
	while (new_path[i])
	{
		full_path = ft_strjoin(new_path[i++], "/");
		full_cmd = ft_strjoin(full_path, token_name);
		if (access(full_cmd, X_OK) == 0)
			return (free(full_cmd), free(full_path), ft_free_matrix(new_path), 1); // this will change to a function because of annoying people
		free(full_cmd);
		free(full_path);
	}
	ft_free_matrix(new_path);
	return (0);
}
