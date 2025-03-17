/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:02:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/03/14 18:53:00 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char *path = getenv("PATH");
    char *cmd = "ls";
    char *args[] = {cmd, NULL};
    char *dir = strtok(path, ":");
    char full_path[1024];

    if (argc)
        cmd = argv[1];
    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s\n", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            pid_t pid = fork();
            printf("fork %d", pid);
            if (pid == 0) {
                execve(full_path, args, NULL);
                perror("execve failed");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                perror("fork failed");
            }
            break;
        }
		execve(full_path, args, NULL);
        dir = strtok(NULL, ":");
    }
    printf("path: %s\n", full_path);
    return 0;
}
