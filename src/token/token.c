/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:48:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/04/08 15:49:43 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// #include <stddef.h> // para size_t
// #include <string.h> // para strlen, strcmp
// #include <stddef.h> // para size_t

// const char *token_type_str(t_type type) {
//     switch (type) {
//         case AND: return "AND";
//         case OR: return "OR";
//         case PIPE: return "PIPE";
//         case IN_REDIR: return "IN_REDIR";
//         case OUT_REDIR: return "OUT_REDIR";
//         case APPEND: return "APPEND";
//         case HEREDOC: return "HEREDOC";
//         case COMMAND: return "COMMAND";
//         case WORD: return "WORD";
//         case EXPAND: return "EXPAND";
//         case SING_QUOTE: return "SING_QUOTE";
//         case DOUB_QUOTE: return "DOUB_QUOTE";
//         case OPEN_PAR: return "OPEN_PAR";
//         case CLOSE_PAR: return "CLOSE_PAR";
//         default: return "UNKNOWN";
//     }
// }

// size_t ft_strlen(const char *str)
// {
// 	size_t len = 0;

// 	while (str[len] != '\0')
// 		len++;
// 	return len;
// }


// // Prototypes das funções usadas
// void    get_token(t_data *data, int start);
// t_type  give_id_token(char *str);
// void    add_token_list(t_data *data, char *token_name, t_type id_token);
// size_t  ft_strlen(const char *s); // para evitar erro no ft_strlen

// char *ft_strchr(const char *s, int c) {
//     while (*s) {
//         if (*s == (char)c)
//             return (char *)s;
//         s++;
//     }
//     return NULL;
// }

// int ft_stchr(const char *set, char c) {
//     while (*set) {
//         if (*set == c)
//             return 1;
//         set++;
//     }
//     return 0;
// }

// char *ft_substr(const char *s, unsigned int start, size_t len) {
//     char *sub = malloc(len + 1);
//     if (!sub) return NULL;
//     strncpy(sub, s + start, len);
//     sub[len] = '\0';
//     return sub;
// }

// char *ft_strdup(const char *s) {
//     char *dup = malloc(strlen(s) + 1);
//     if (dup)
//         strcpy(dup, s);
//     return dup;
// }

// void *ft_calloc(size_t count, size_t size) {
//     void *ptr = malloc(count * size);
//     if (ptr)
//         memset(ptr, 0, count * size);
//     return ptr;
// }

/* TESTE TESTE TESTE */


int	create_token(t_data *data)
{
	int	i;

	i = 0;
	while (ft_strchr(NOPRINTABLE, data->prompt[i]) != NULL
		&& data->prompt[i] != '\0')
		i++;
	while (data->prompt[i] != '\0')
	{
		if (!ft_stchr(NOPRINTABLE, data->prompt[i]))
		{
			get_token(data, i);
			while (data->prompt[i] && !ft_strchr(NOPRINTABLE, data->prompt[i]))
                i++;
		}
		else
			i++;
	}
	return i;
}

void	get_token(t_data *data, int start)
{
	int			end;
	char		*token_name;
	t_type		id_token;

	end = start;
	while (data->prompt[end] && !ft_stchr(NOPRINTABLE, data->prompt[end]))
		end++;
	token_name = ft_substr(data->prompt, start, end - start); //we have an malloc here, that come from ft_substr
	id_token = give_id_token(token_name);
	add_token_list(data, token_name, id_token);
	free(token_name); //free of the ft_substr
}

t_type	give_id_token(char *str)
{
    if (strcmp(str, "&&") == 0)
        return AND;
    if (strcmp(str, "||") == 0)
        return OR;
    if (strcmp(str, "|") == 0)
        return PIPE;
    if (strcmp(str, "<<") == 0)
        return HEREDOC;
    if (strcmp(str, ">>") == 0)
        return APPEND;
    if (strcmp(str, "<") == 0)
        return IN_REDIR;
    if (strcmp(str, ">") == 0)
        return OUT_REDIR;
    if (strcmp(str, "(") == 0)
        return OPEN_PAR;
    if (strcmp(str, ")") == 0)
        return CLOSE_PAR;
    if (str[0] == '\'' && str[ft_strlen(str)-1] == '\'')
        return SING_QUOTE;
    if (str[0] == '"' && str[ft_strlen(str)-1] == '"')
        return DOUB_QUOTE;
    if (str[0] == '$')
        return EXPAND;
    return WORD;
}

void add_token_list(t_data *data, char *token_name, t_type id_token)
{
	t_token	*new_token;
	t_token	*last;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if(!new_token)
		return ;
	if (token_name)
	{
		new_token->value = ft_strdup(token_name);
		if (!new_token->value)
			free(new_token); return;
	}
	else
		new_token->value =NULL;
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

/*TESTE TESTE TESTE TESTE TESTE TESTE TESTE*/

// void print_tokens(t_token *list) {
//     while (list) {
//         printf("Token: %-10s | Type: %-2d\n", list->value, list->type);
//         list = list->next;
//     }
// }

// int main(void)
// {
// 	t_data *data = calloc(1, sizeof(t_data));
// 	data->prompt = strdup("echo hello && ls < << | || > >> ( ) $ $$ * .*");

// 	create_token(data);

// 	// só print pra ver se funcionou:
// 	t_token *tok = data->token_list;
// 	while (tok)
// 	{
// 		print_tokens(data->token_list);
// 		tok = tok->next;
// 	}

// 	return 0;
// }

/*TESTE TESTE TESTE TESTE TESTE TESTE TESTE*/


// void run_test(char *input, int expected, char *test_name) {
//     int result = create_token(input);
    
//     printf("┌───────────────────────────────────────────────┐\n");
//     printf("│ %s%-40s%s │\n", YELLOW, test_name, RESET);
//     printf("├───────────────────────────────────────────────┤\n");
//     printf("│ Input: \"%-35s\" │\n", input);
//     printf("│ Expected: %-4d  Got: %-4d               │\n", expected, result);
    
//     if (result == expected) {
//         printf("│ Status: %s%-29s%s │\n", GREEN, "SUCESSO", RESET);
//     } else {
//         printf("│ Status: %s%-29s%s │\n", RED, "FALHA", RESET);
//     }
//     printf("└───────────────────────────────────────────────┘\n\n");
// }

// int main(void) {
//     printf("\n=== TESTES PARA create_token() ===\n\n");
    
//     // Testes básicos
//     run_test("echo", 0, "Sem espaços no início");
//     run_test("  echo", 2, "Dois espaços no início");
//     run_test("\techo", 1, "Tab no início");
//     run_test("\n\necho", 2, "Dois newlines no início");
    
//     // Casos extremos
//     run_test("", 0, "String vazia");
//     run_test("    ", 4, "Apenas espaços");
//     run_test("  \t\n  ", 6, "Mistura de espaços/tabs/newlines");
    
//     // Casos especiais
//     run_test(" \t\v\f\r\nx", 6, "Todos caracteres não imprimíveis");
//     run_test("a b c", 0, "Caractere imprimível no início");
    
//     return 0;
// }