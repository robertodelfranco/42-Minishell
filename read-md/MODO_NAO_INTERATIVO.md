# Implementação: Modo Não Interativo no Minishell

## Mudanças Realizadas

### 1. Estrutura t_data (include/minishell.h)
- Adicionado campo `bool interactive_mode` para detectar se está em modo interativo
- Incluído header `get_next_line_bonus.h` para leitura de stdin

### 2. Função main (src/main.c)
- Adicionada detecção de modo interativo usando `isatty(STDIN_FILENO)`
- Configuração de sinais apenas em modo interativo
- Inicialização do campo `interactive_mode`

### 3. Função ft_readline (src/read.c)
- Modo interativo: usa `readline()` com prompt colorido
- Modo não interativo: usa `get_next_line()` para ler do stdin
- Remove quebras de linha automaticamente em modo não interativo
- Adiciona ao histórico apenas em modo interativo
- Não imprime "exit" ao sair em modo não interativo

### 4. Builtin exit (src/builtin/exit.c)
- Não imprime "exit" quando em modo não interativo
- Mantém comportamento normal em modo interativo

## Como Funciona

### Detecção de Modo
```c
data->interactive_mode = isatty(STDIN_FILENO);
```

A função `isatty()` retorna:
- `true`: quando stdin é um terminal (modo interativo)
- `false`: quando stdin é um pipe ou arquivo (modo não interativo)

### Exemplos de Uso

**Modo Interativo:**
```bash
./minishell
Minihell $ echo "hello"
hello
Minihell $ exit
exit
```

**Modo Não Interativo:**
```bash
echo "echo hello" | ./minishell
hello

./minishell < commands.txt
# executa comandos sem prompt ou mensagens extras
```

## Benefícios

1. **Compatibilidade com scripts**: Pode ser usado em pipelines e scripts
2. **Saída limpa**: Não imprime prompts ou mensagens desnecessárias
3. **Automação**: Facilita testes automatizados
4. **Compatibilidade bash**: Comportamento similar ao bash

## Testes Incluídos

- `test_interactive.sh`: Compara modo interativo vs não interativo
- `batch_test.sh`: Testes automatizados
- `compare_test.sh`: Comparação com bash
- `commands.txt`: Arquivo de exemplo para testes
