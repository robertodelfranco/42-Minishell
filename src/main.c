/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:02:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/03/18 15:41:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *input;

    while (1)
    {
        // Exibe o prompt e lê a entrada do usuário
        input = readline("$> ");
        
        // Se o usuário apertar Ctrl+D (EOF), encerra o shell
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // Se o usuário digitou algo, adiciona ao histórico
        if (*input)
            add_history(input);

        // Por enquanto, apenas imprime o comando digitado
        printf("Comando digitado: %s\n", input);

        // Libera a memória alocada por readline
        free(input);
    }
    return 0;
}

