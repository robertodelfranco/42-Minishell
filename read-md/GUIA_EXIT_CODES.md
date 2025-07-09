# Guia: Testando Exit Codes em Modo Não Interativo

## Por que precisa do `exit`?

Em modo não interativo, o minishell continua lendo comandos do stdin até encontrar EOF ou um comando `exit`. Sem o `exit` explícito, o processo pode:

1. **Ficar aguardando mais entrada**
2. **Não retornar o exit code correto**
3. **Não terminar adequadamente**

## Métodos para Testar Exit Codes

### 1. Usando Echo com Pipe
```bash
echo "exit 42" | ./minishell
echo "Exit code: $?"
```

### 2. Usando Arquivo de Comandos
```bash
# Criar arquivo
echo "exit 42" > test.txt
./minishell < test.txt
echo "Exit code: $?"
```

### 3. Usando Here Document
```bash
./minishell << EOF
pwd
exit 42
EOF
echo "Exit code: $?"
```

## Exemplos Práticos

### Exit Code 0 (Sucesso)
```bash
echo "exit 0" | ./minishell
echo $?  # Deve mostrar: 0
```

### Exit Code Customizado
```bash
echo "exit 42" | ./minishell
echo $?  # Deve mostrar: 42
```

### Exit Code de Comando Anterior
```bash
# Arquivo: test_false.txt
false
echo $?
exit $?

./minishell < test_false.txt
echo "Exit final: $?"
```

### Comparação com Bash
```bash
# Teste com bash
echo "exit 42" | bash
echo "Bash exit: $?"

# Teste com minishell
echo "exit 42" | ./minishell
echo "Minishell exit: $?"
```

## Scripts de Teste Incluídos

1. **`test_exit_codes.sh`**: Testes sistemáticos de exit codes
2. **`demo_exit_codes.sh`**: Demonstrações práticas
3. **`batch_test.sh`**: Testes automatizados (agora com exits)

## Executar Testes
```bash
# Teste específico de exit codes
./test_exit_codes.sh

# Demonstração completa
./demo_exit_codes.sh

# Batch de testes
./batch_test.sh
```

## Dicas Importantes

1. **Sempre inclua `exit`** nos testes não interativos
2. **Use `exit $?`** para preservar exit codes de comandos anteriores
3. **Exit codes > 255** são reduzidos módulo 256
4. **Sem argumentos**, exit usa o último exit code

## Exemplo de Arquivo de Teste Completo
```bash
# test_complete.txt
echo "Iniciando testes..."
pwd
echo "Diretório atual listado"
true
echo "Comando true executado, exit code: $?"
false
echo "Comando false executado, exit code: $?"
exit $?
```

Execute com:
```bash
./minishell < test_complete.txt
echo "Exit code final: $?"
```
