/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created:                                          #+#    #+#             */
/*   Updated:                                          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_quoted_token(t_data *data, int i)
{
    int		start;
    char	quote;
    char	*token;

    start = i;
    quote = data->prompt[i++];
    while (data->prompt[i] && data->prompt[i] != quote)
        i++;
    
    if (data->prompt[i] == quote)
        i++;
    
    token = ft_substr(data->prompt, start, i - start);
    add_token_list(data, token, give_id_token(token));
    free(token);
    
    return (i);
}

static int	handle_operator_token(t_data *data, int i)
{
    int		start;
    int		len;
    char	*token;

    start = i;
    len = 1;
    
    // Check for double operators (>> or <<)
    if ((data->prompt[i] == '>' || data->prompt[i] == '<') && 
        data->prompt[i + 1] == data->prompt[i])
        len = 2;
        
    token = ft_substr(data->prompt, start, len);
    add_token_list(data, token, give_id_token(token));
    free(token);
    
    return (i + len);
}

static int	handle_word_token(t_data *data, int i)
{
    int		start;
    char	*token;

    start = i;
    
    // Continue until whitespace, quotes, or operator
    while (data->prompt[i] && 
           !ft_strchr(NOPRINTABLE, data->prompt[i]) &&
           data->prompt[i] != '"' && data->prompt[i] != '\'' &&
           data->prompt[i] != '>' && data->prompt[i] != '<' &&
           data->prompt[i] != '|')
        i++;
        
    token = ft_substr(data->prompt, start, i - start);
    add_token_list(data, token, give_id_token(token));
    free(token);
    
    return (i);
}

void	add_token_list(t_data *data, char *token_name, t_type id_token)
{
    t_token	*new_token;
    t_token	*last;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return ;
    
    new_token->value = ft_strdup(token_name);
    if (!new_token->value)
    {
        free(new_token);
        return ;
    }
    
    new_token->type = id_token;
    new_token->quoted = false;
    new_token->next = NULL;
    
    if (!data->token_list)
        data->token_list = new_token;
    else
    {
        last = ft_last(data->token_list);
        last->next = new_token;
    }
}

int	create_token(t_data *data)
{
    int	i;

    i = 0;
    // Skip initial whitespace
    while (data->prompt[i] && ft_strchr(NOPRINTABLE, data->prompt[i]))
        i++;
        
    while (data->prompt[i])
    {
        // Handle quotes ('' or "")
        if (data->prompt[i] == '\'' || data->prompt[i] == '"')
            i = handle_quoted_token(data, i);
            
        // Handle operators (>, <, >>, <<, |)
        else if (data->prompt[i] == '>' || data->prompt[i] == '<' || 
                data->prompt[i] == '|')
            i = handle_operator_token(data, i);
            
        // Handle regular words
        else if (!ft_strchr(NOPRINTABLE, data->prompt[i]))
            i = handle_word_token(data, i);
            
        // Skip whitespace
        else
            i++;
    }
    
    return (i);
}