/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:02:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/03/18 13:12:12 by rafaelherin      ###   ########.fr       */
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

// int main(int argc, char *argv[]) {
//     char *path = getenv("PATH");
//     char *cmd = "ls";
//     char *args[] = {cmd, NULL};
//     char *dir = strtok(path, ":");
//     char full_path[1024];

//     if (argc)
//         cmd = argv[1];
//     while (dir != NULL) {
//         snprintf(full_path, sizeof(full_path), "%s/%s\n", dir, cmd);
//         if (access(full_path, X_OK) == 0) {
//             pid_t pid = fork();
//             printf("fork %d", pid);
//             if (pid == 0) {
//                 execve(full_path, args, NULL);
//                 perror("execve failed");
//                 exit(EXIT_FAILURE);
//             } else if (pid > 0) {
//                 wait(NULL);
//             } else {
//                 perror("fork failed");
//             }
//             break;
//         }
// 		execve(full_path, args, NULL);
//         dir = strtok(NULL, ":");
//     }
//     printf("path: %s\n", full_path);
//     return 0;
// }

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

