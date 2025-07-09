# Recursos Testados no Minishell - Versão Refatorada

## Visão Geral

Esta versão refatorada dos testes foi criada para focar **exclusivamente** nos recursos que seu minishell implementa, removendo todos os testes de funcionalidades não suportadas ou bonus.

## Recursos Implementados e Testados

### 1. Builtins Obrigatórios (7)

#### echo
- `echo hello` - Output simples
- `echo 'hello world'` - Com aspas simples
- `echo "hello world"` - Com aspas duplas  
- `echo -n test` - Flag -n (sem quebra de linha)

#### cd
- `cd /tmp` - Mudar para diretório existente
- `cd /nonexistent` - Erro para diretório inexistente
- `cd` - Voltar para HOME (sem argumentos)

#### pwd
- `pwd` - Mostrar diretório atual

#### export
- `export TEST_VAR=123` - Criar nova variável
- `export` - Listar todas as variáveis exportadas

#### unset
- `unset PATH` - Remover variável existente
- `unset NONEXISTENT` - Tentar remover variável inexistente

#### env
- `env` - Listar todas as variáveis de ambiente

#### exit
- `exit 0` - Exit code 0
- `exit 1` - Exit code 1
- `exit 42` - Exit code 42
- `exit 255` - Exit code 255

### 2. Comandos Externos

#### Comandos Básicos
- `ls` - Listagem de arquivos
- `/bin/ls` - Comando com path absoluto
- `cat /etc/passwd` - Leitura de arquivo
- `wc -l /etc/passwd` - Comando com argumentos

### 3. Pipes

#### Pipes Simples
- `ls | head -3` - Comando para comando
- `cat /etc/passwd | grep root` - Filtrar output
- `echo hello | cat` - Pipe simples
- `env | grep USER` - Filtrar variáveis
- `ls | wc -l` - Contar linhas

### 4. Redirecionamentos

#### Output Redirect (>)
- `echo test > /tmp/minishell_test.txt` - Criar arquivo
- `ls > /tmp/ls_output.txt` - Comando para arquivo

#### Input Redirect (<)
- `cat < /etc/passwd` - Ler de arquivo

#### Append Redirect (>>)
- `echo append >> /tmp/minishell_test.txt` - Adicionar ao arquivo

### Heredoc Redirect (<<)
- `cat << hi` - Lê até o delimitador ou Ctrl-D, suporta delimitador entre aspas pra não expansão de variavéis e vice-versa

### 5. Expansão de Variáveis

#### Variáveis de Ambiente
- `echo $HOME` - Variável HOME
- `echo $USER` - Variável USER
- `echo $PATH` - Variável PATH
- `echo $?` - Exit code do último comando
- `echo $NONEXISTENT` - Variável inexistente (deve resultar em string vazia)

### 6. Tratamento de Aspas

#### Aspas Simples
- `echo 'hello world'` - Texto literal
- `echo '$HOME'` - Variável não expandida
- `echo 'it'\''s working'` - Escape de aspas simples

#### Aspas Duplas
- `echo "hello world"` - Texto simples
- `echo "$HOME"` - Variável expandida dentro de aspas duplas

### 7. Tratamento de Erros

#### Comandos Inexistentes
- `nonexistent_command` - Comando não encontrado
- `/bin/nonexistent` - Executável inexistente

#### Arquivos/Diretórios Inexistentes
- `ls /nonexistent_directory` - Diretório inexistente
- `cat /nonexistent_file` - Arquivo inexistente

### 8. Combinações de Recursos

#### Múltiplas Funcionalidades
- Histórico de comandos
- `echo $USER | cat` - Expansão + pipe
- `ls | head -3 > /tmp/output.txt` - Pipe + redirect
- `echo 'hello' | cat > /tmp/hello.txt` - Aspas + pipe + redirect

## Recursos NÃO Testados (Não Implementados)

### Operadores Lógicos
- `&&` (AND lógico)
- `||` (OR lógico)
- `;` (separador de comandos)

### Variáveis Especiais
- `$$` (PID do shell)
- `$!` (PID do último processo em background)

### Recursos Bonus/Avançados
- Pipes não fechados
- Aspas não fechadas
- Wildcards (`*`)
- Expansão de chaves (`{1..10}`)
- Background processes (`&`)
- Job control

### Redirecionamentos Avançados
- `2>` (redirect stderr)
- `&>` (redirect stdout e stderr)

## Como Usar os Testes

### Script Principal (Completo)
```bash
./batch_test_refactored.sh          # Execução normal
./batch_test_refactored.sh -v       # Com Valgrind (memory leaks)
./batch_test_refactored.sh --help   # Mostrar ajuda
```

### Script Mínimo (Verificação Rápida)
```bash
./minimal_test.sh                   # Testes essenciais apenas
```

## Benefícios da Refatoração

1. **Foco nos Recursos Implementados**: Remove testes de funcionalidades não implementadas
2. **Melhor Accuracy**: Compara apenas o que seu minishell deveria fazer
3. **Debugging Mais Fácil**: Falhas indicam problemas reais, não recursos não implementados
4. **Performance**: Execução mais rápida com menos testes irrelevantes
5. **Clarity**: Cada teste tem propósito específico e conhecido

## Estrutura dos Testes

Os testes estão organizados em seções claras:

1. **Builtins** - Todos os 7 comandos internos obrigatórios
2. **Comandos Externos** - Execução de programas externos
3. **Pipes** - Comunicação entre processos
4. **Redirecionamentos** - Manipulação de I/O
5. **Expansão** - Substituição de variáveis
6. **Aspas** - Processamento de strings
7. **Exit Codes** - Códigos de saída corretos
8. **Erros** - Tratamento adequado de erros
9. **Combinações** - Múltiplos recursos juntos

Cada seção testa aspectos específicos e essenciais do funcionamento do minishell, garantindo que todos os recursos obrigatórios estejam funcionando corretamente.
