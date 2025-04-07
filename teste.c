#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Tipos de nó na AST
typedef enum e_node_type {
    CMD_NODE,
    PIPE_NODE
} t_node_type;

// Estrutura do nó da AST
typedef struct s_ast {
    t_node_type type;
    char **args;           // Para comandos simples (NULL para nós de pipe)
    struct s_ast *left;    // Filho esquerdo: comando antes do pipe
    struct s_ast *right;   // Filho direito: comando depois do pipe
} t_ast;

t_ast *create_cmd_node(char **args) {
    t_ast *node = malloc(sizeof(t_ast));
    if (!node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->type = CMD_NODE;
    node->args = args;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Cria um nó para o operador pipe, ligando dois nós (comandos)
t_ast *create_pipe_node(t_ast *left, t_ast *right) {
    t_ast *node = malloc(sizeof(t_ast));
    if (!node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->type = PIPE_NODE;
    node->left = left;
    node->right = right;
    node->args = NULL;  // Não há argumentos diretamente no nó de pipe
    return node;
}

int execute_ast(t_ast *node) {
    if (!node)
        return -1;
		
	if (node->type == CMD_NODE) {
		pid_t pid = fork();
        if (pid == 0) {
            // no filho, executa o comando
            execvp(node->args[0], node->args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            return status;
        } else {
            perror("fork failed");
            return -1;
        }
    }

    if (node->type == PIPE_NODE) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            return -1;
        }
        // Executa o comando à esquerda (primeiro filho)
        pid_t pid_left = fork();
        if (pid_left == 0) {
            // Redireciona stdout para a escrita do pipe
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]); // Fecha a leitura no filho da esquerda
            close(pipefd[1]);
            exit(execute_ast(node->left));
        }
		
        // Executa o comando à direita (segundo filho)
        pid_t pid_right = fork();
        if (pid_right == 0) {
            // Redireciona stdin para a leitura do pipe
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]); // Fecha a escrita no filho da direita
            close(pipefd[0]);
            exit(execute_ast(node->right));
        }

        // No processo pai, fecha ambos os lados do pipe e espera pelos filhos
        close(pipefd[0]);
        close(pipefd[1]);

        int status_left, status_right;
        waitpid(pid_left, &status_left, 0);
        waitpid(pid_right, &status_right, 0);

        return status_right;
    }

    return 0;
}

#include <stdbool.h>

struct teste
{
	bool a;
	bool b;
};


int main(void) {
    // Monta o nó para o comando "ls -l"
    char **args1 = malloc(3 * sizeof(char *));
    args1[0] = strdup("ls");
    args1[1] = strdup("-l");
    args1[2] = NULL;
    t_ast *cmd1 = create_cmd_node(args1);

    // Monta o nó para o comando "grep txt"
    char **args2 = malloc(3 * sizeof(char *));
    args2[0] = strdup("grep");
    args2[1] = strdup("txt");
    args2[2] = NULL;
    t_ast *cmd2 = create_cmd_node(args2);

    // Cria um nó de pipe que conecta os dois comandos
    t_ast *pipe_node = create_pipe_node(cmd1, cmd2);

    // Executa a árvore
    execute_ast(pipe_node);

    return 0;
}
