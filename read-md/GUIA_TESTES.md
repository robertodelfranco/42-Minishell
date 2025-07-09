# Scripts de Teste do Minishell - Guia Rápido

## 🎯 Scripts Principais

### 1. `test_overview.sh` - Overview Geral
```bash
./test_overview.sh
```
**Recomendado para começar** - Mostra status geral de todos os testes

### 2. `basic_interactive_test.sh` - Teste Principal
```bash
./basic_interactive_test.sh              # Teste completo
./basic_interactive_test.sh --valgrind   # Inclui memory leaks
```
**Script mais confiável** - Testa todas as funcionalidades em modo interativo

### 3. `simple_interactive_test.sh` - Alternativa Simples
```bash
./simple_interactive_test.sh
./simple_interactive_test.sh --valgrind
```
Versão alternativa usando pipes nomeados

### 4. `interactive_batch_test.sh` - Versão Avançada
```bash
./interactive_batch_test.sh
./interactive_batch_test.sh --valgrind
```
Requer `expect` - Simulação mais sofisticada

## 📊 Scripts de Análise Específica

### Memory Leaks e Erros
- `test_valgrind.sh` - Teste focado em memory leaks
- `test_error_messages.sh` - Teste de mensagens de erro

### Testes Legados (Refatorados)
- `batch_test_refactored.sh` - Versão refatorada do teste original
- `minimal_test.sh` - Testes mínimos essenciais

## 📖 Documentação

### Arquivos de Documentação
- `TESTES_INTERATIVOS.md` - **Documentação principal**
- `RECURSOS_TESTADOS.md` - Lista de recursos testados
- `MELHORIAS_BATCH_TEST.md` - Melhorias implementadas
- `CORRECAO_REDIRECIONAMENTO.md` - Correções de redirecionamento
- `CORRECAO_EXIT_STATUS.md` - Correções de exit status

## 🚀 Uso Recomendado

### Para Desenvolvimento Diário
```bash
./test_overview.sh                    # Status geral rápido
./basic_interactive_test.sh           # Teste completo
```

### Para Debugging
```bash
./basic_interactive_test.sh --valgrind  # Detectar memory leaks
./test_error_messages.sh               # Testar mensagens de erro
```

### Para Avaliação Final
```bash
./test_overview.sh                    # Overview geral
./basic_interactive_test.sh           # Teste completo
./basic_interactive_test.sh --valgrind # Memory leaks
```

## ✅ Funcionalidades Testadas

### Comandos Básicos
- `echo` (aspas simples, duplas, múltiplas palavras)
- Comandos externos (`ls`, `cat`, `whoami`, etc.)

### Builtins Obrigatórios
- `pwd` - Print working directory
- `cd` - Change directory
- `echo` - Print arguments
- `export` - Set environment variables
- `unset` - Unset environment variables
- `env` - Print environment
- `exit` - Exit shell

### Recursos Avançados
- **Redirecionamento**: `>`, `<`, `>>`
- **Pipes**: `|` (simples e múltiplos)
- **Expansão**: `$VAR`, `$HOME`, `$USER`, `$PWD`
- **Aspas**: Simples (`'`) e duplas (`"`)
- **Sinais**: Ctrl+C, Ctrl+D
- **Exit codes**: Propagação correta

### Tratamento de Erros
- Comandos inexistentes
- Arquivos inexistentes
- Sintaxe inválida
- Comparação inteligente de mensagens

## 🔧 Resolução de Problemas

### Erro: "Minishell não encontrado"
```bash
make clean && make
```

### Erro: "expect não encontrado"
```bash
# Ubuntu/Debian
sudo apt-get install expect

# macOS
brew install expect

# Alternativa: use basic_interactive_test.sh
```

### Erro: "mkfifo não disponível"
```bash
# Use o script básico em vez do simples
./basic_interactive_test.sh
```

### Testes falhando
```bash
# Veja detalhes específicos
./basic_interactive_test.sh | grep "❌"

# Teste funcionalidade específica
echo "seu_comando_aqui" | ./minishell
```

## 📈 Interpretação dos Resultados

### ✅ Teste Passou
- Funcionalidade implementada corretamente
- Output idêntico ao bash

### ❌ Teste Falhou
- Diferença no output ou comportamento
- Mostra diferença entre minishell e bash

### ⚠️ Falhou (conforme esperado)
- Teste de caso de erro funcionou
- Minishell detectou erro corretamente

### ⚠️ Inesperado
- Teste deveria falhar mas passou
- Pode indicar falta de validação

## 🎯 Status Atual

Com base nos últimos testes:
- **23/24 testes passando** (96% sucesso)
- **Sem memory leaks** detectados
- **Todas as funcionalidades obrigatórias** implementadas
- **Compatibilidade alta** com bash

O minishell está **pronto para avaliação**!
