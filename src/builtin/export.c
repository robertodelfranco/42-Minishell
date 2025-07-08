/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:21:38 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/08 16:16:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	update_or_add_env(t_data *data, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*new_node;

	cur = data->env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			if (value)
				cur->value = ft_strdup(value);
			else
				cur->value = NULL;
			return (0);
		}
		cur = cur->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (1);
	append_env_node(&data->env_list, new_node);
	return (0);
}

static int	parse_export_arg(const char *arg, char **key, char **value)
{
	char	*equal_pos;
	int		key_len;

	*key = NULL;
	*value = NULL;
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		key_len = equal_pos - arg;
		*key = ft_substr(arg, 0, key_len);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (equal_pos && !*value))
	{
		free(*key);
		free(*value);
		return (1);
	}
	return (0);
}

static bool	process_export_arg(t_data *data, char *arg)
{
	char	*key;
	char	*value;
	bool	result;

	result = true;
	if (parse_export_arg(arg, &key, &value) != 0)
	{
		ft_printf_fd(2, "export: key error\n");
		data->exit_status = 1;
		return (true);
	}
	if (!is_valid_identifier(key))
	{
		ft_printf_fd(2, "export: `%s': not a valid identifier\n", arg);
		data->exit_status = 1;
		result = false;
	}
	else if (update_or_add_env(data, key, value) != 0)
	{
		ft_printf_fd(2, "export: update or add error\n");
		data->exit_status = 1;
	}
	free(key);
	free(value);
	return (result);
}

bool	b_export(t_data *data, char **argv)
{
	int	i;

	if (!argv[1])
	{
		print_export(data);
		return (true);
	}
	i = 1;
	while (argv[i])
	{
		if (!process_export_arg(data, argv[i]))
			return (false);
		i++;
	}
	return (true);
}
