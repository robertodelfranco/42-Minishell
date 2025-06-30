/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:21:38 by rheringe          #+#    #+#             */
/*   Updated: 2025/06/30 14:24:29 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		ft_envdup(t_data *data);
void	print_export(t_data *data);
static t_env	*create_env_node(const char *key, const char *value);
static void	append_env_node(t_env **head, t_env *new_node);

static void	append_env_node(t_env **head, t_env *new_node)
{
	t_env	*last;

	if (!*head)
		*head = new_node;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
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

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!((str[0] >= 'a' && str[0] <= 'z') || 
		  (str[0] >= 'A' && str[0] <= 'Z') || 
		  str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') ||
			  (str[i] >= 'A' && str[i] <= 'Z') ||
			  (str[i] >= '0' && str[i] <= '9') ||
			  str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_export_arg(const char *arg, char **key, char **value)
{
	char	*equal_pos;
	int		key_len;

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

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

void	b_export(t_data *data, char **argv)
{
	int		i;
	char	*key;
	char	*value;

	if (!argv[1])
	{
		print_export(data);
		return ;
	}
	
	i = 1;
	while (argv[i])
	{
		printf("DEBUG: Processando '%s'\n", argv[i]); // DEBUG
		
		key = NULL;
		value = NULL;
		
		if (parse_export_arg(argv[i], &key, &value) != 0)
		{
			printf("export: memory allocation error\n");
			data->exit_status = 1;
		}
		else 
		{
			printf("DEBUG: key='%s', value='%s'\n", key, value ? value : "NULL"); // DEBUG
			
			if (!is_valid_identifier(key))
			{
				printf("export: `%s': not a valid identifier\n", argv[i]);
				data->exit_status = 1;
			}
			else
			{
				printf("DEBUG: Chamando update_or_add_env\n"); // DEBUG
				if (update_or_add_env(data, key, value) != 0)
				{
					printf("export: memory allocation error\n");
					data->exit_status = 1;
				}
				else
				{
					printf("DEBUG: Variável adicionada com sucesso!\n"); // DEBUG
				}
			}
		}
		
		free(key);
		free(value);
		i++;
	}
}