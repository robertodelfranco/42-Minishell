# Testes Interativos do Minishell

## Visão Geral

Este documento descreve o sistema de testes interativos desenvolvido para o minishell, que executa comandos em modo interativo e compara os resultados com o bash.

## Scripts Disponíveis

### 1. `basic_interactive_test.sh` (Recomendado)
**Script principal e mais confiável**

- Testa todas as funcionalidades obrigatórias do minishell
- Filtra corretamente o prompt e saídas do sistema
- Comparação inteligente de outputs e mensagens de erro
- Suporte a Valgrind para detecção de memory leaks

**Uso:**
```bash
./basic_interactive_test.sh              # Executa todos os testes
./basic_interactive_test.sh --valgrind   # Inclui teste de memory leaks
./basic_interactive_test.sh --help       # Mostra ajuda
```

### 2. `simple_interactive_test.sh`
**Versão alternativa usando pipes nomeados**

- Não depende do `expect`
- Usa pipes nomeados (mkfifo) para comunicação
- Timeout configurável para comandos

### 3. `interactive_batch_test.sh`
**Versão avançada com expect**

- Requer o utilitário `expect` instalado
- Simulação mais sofisticada de entrada interativa
- Maior controle sobre a sessão do shell

## Funcionalidades Testadas

### ✅ Comandos Básicos
- `echo` com diferentes tipos de aspas
- Comandos com múltiplas palavras

### ✅ Builtins
- `pwd` - Print working directory
- `env` - Listar variáveis de ambiente
- `export` - Exportar variáveis
- `unset` - Remover variáveis
- `cd` - Mudar diretório
- `exit` - Sair do shell

### ✅ Expansão de Variáveis
- `$HOME`, `$USER`, `$PWD`
- Variáveis customizadas após export
- Exit status (`$?`)

### ✅ Redirecionamento
- Output redirect (`>`)
- Input redirect (`<`)
- Append (`>>`)
- Verificação de conteúdo dos arquivos

### ✅ Pipes
- Pipes simples (`comando1 | comando2`)
- Pipes múltiplos (`cmd1 | cmd2 | cmd3`)
- Combinação com comandos externos

### ✅ Comandos Externos
- `whoami`, `ls`, `cat`, `head`
- Integração com pipes

### ✅ Tratamento de Erros
- Comandos inexistentes
- Arquivos inexistentes
- Comparação inteligente de mensagens de erro

## Como Funciona

### Execução de Comandos
1. **Criação de script temporário**: Cada comando é escrito em um arquivo temporário junto com `exit`
2. **Execução**: O minishell é executado com redirecionamento de entrada
3. **Filtragem**: O output é filtrado para remover prompts e comandos de controle
4. **Comparação**: O resultado é comparado com a execução equivalente no bash

### Filtragem de Output
```bash
# Remove prompts "Minihell $" e comandos de controle
output=$(timeout 3 ./minishell < script.sh 2>&1 | \
         sed '/Minihell \$/d' | \
         grep -v "^exit$" | \
         grep -v "^$")
```

### Comparação Inteligente
- **Outputs idênticos**: Comparação exata
- **Mensagens de erro**: Verifica se ambos contêm a essência do erro
- **Comandos variáveis** (`env`, `pwd`): Verifica elementos-chave
- **Comandos sem output** (`export`, `unset`): Verifica se ambos estão vazios

## Vantagens dos Testes Interativos

### ✅ Modo Real de Operação
- Testa o minishell exatamente como será usado
- Garante que prompts e leitura de linha funcionem
- Detecta problemas de interatividade

### ✅ Comparação Precisa
- Compara diretamente com comportamento do bash
- Detecta diferenças sutis em outputs
- Verifica compatibilidade de comandos

### ✅ Detecção de Memory Leaks
- Integração opcional com Valgrind
- Detecta vazamentos de memória em uso real
- Testes com múltiplos comandos

### ✅ Automação Completa
- Execução batch de todos os testes
- Relatórios coloridos e informativos
- Taxa de sucesso e estatísticas

## Exemplos de Output

### Teste Bem-sucedido
```
[1] Echo simples
    Comando: echo hello
    ✅ PASSOU
```

### Teste com Falha
```
[2] Comando inexistente
    Comando: cmd_nao_existe
    ❌ FALHOU
    Minishell: 'cmd_nao_existe: command not found'
    Bash: 'bash: cmd_nao_existe: command not found'
```

### Teste de Erro Esperado
```
[3] Arquivo inexistente
    Comando: cat arquivo_nao_existe
    ⚠️  FALHOU (conforme esperado)
```

### Resultado Final
```
=== RESULTADOS ===
Total: 24
Passou: 23
Falhou: 0
🎉 Todos os testes passaram!
```

## Valgrind Integration

### Teste de Memory Leaks
```bash
./basic_interactive_test.sh --valgrind
```

### Output do Valgrind
```
=== TESTE BÁSICO DE MEMORY LEAKS ===
Executando teste básico com Valgrind...
==523484==    definitely lost: 0 bytes in 0 blocks
==523484==    indirectly lost: 0 bytes in 0 blocks
==523484== ERROR SUMMARY: 0 errors from 0 contexts
```

## Resolução de Problemas

### Problema: "Minishell não encontrado"
```bash
make clean && make  # Recompila o minishell
```

### Problema: "expect não encontrado"
```bash
# Ubuntu/Debian
sudo apt-get install expect

# macOS
brew install expect
```

### Problema: "mkfifo não disponível"
- Use `basic_interactive_test.sh` em vez de `simple_interactive_test.sh`
- mkfifo é padrão em sistemas Unix/Linux

## Conclusão

O sistema de testes interativos oferece uma forma robusta e automatizada de validar o minishell, garantindo que todas as funcionalidades obrigatórias funcionem corretamente em modo interativo, que é como o shell será realmente usado.

O script `basic_interactive_test.sh` é recomendado para uso regular, oferecendo uma boa combinação de simplicidade, eficácia e recursos avançados.
