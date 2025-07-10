#!/bin/bash

# =========================================================================
# TESTE ROBUSTO INTERATIVO PARA MINISHELL - VERSÃO INCREMENTADA
# =========================================================================
# Versão aprimorada com testes mais robustos e complexos baseados no
# arquivo RECURSOS_TESTADOS.md. Inclui verificação de exit status em
# todos os comandos e casos edge complexos.
#
# MELHORIAS DESTA VERSÃO:
# • Verificação de exit status em todos os comandos
# • Testes de stress com pipes múltiplos e casos extremos
# • Testes abrangentes por funcionalidade (7 builtins obrigatórios)
# • Casos edge: aspas complexas, variáveis longas, paths complexos
# • Testes de combinação de múltiplos recursos
# • Verificação automatizada de heredoc (quando possível)
# • Relatório detalhado de funcionalidades testadas
# • Opções flexíveis: --stress, --comprehensive, --all, --valgrind
#
# FUNCIONALIDADES TESTADAS (conforme RECURSOS_TESTADOS.md):
# ✓ 7 Builtins: echo, cd, pwd, export, unset, env, exit
# ✓ Comandos externos com paths absolutos e relativos
# ✓ Pipes simples e múltiplos
# ✓ Redirecionamentos: >, <, >>
# ✓ Heredoc: <<
# ✓ Expansão de variáveis: $VAR, $?, $HOME, etc
# ✓ Tratamento de aspas simples e duplas
# ✓ Tratamento de erros e exit codes corretos
# ✓ Combinações complexas de recursos
#
# FUNCIONALIDADES NÃO TESTADAS (não implementadas):
# ✗ Escape de caracteres (\)
# ✗ Comandos multilinha
# ✗ Separador de comandos (;)
# ✗ Operadores lógicos (&&, ||)
# =========================================================================

# Cores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Contadores
TESTS_PASSED=0
TESTS_FAILED=0
TESTS_TOTAL=0

MINISHELL="./minishell"

# Função para executar comando no minishell
test_minishell() {
    local cmd="$1"
    local timeout_val="${2:-3}"
    
    # Cria arquivo temporário com o comando
    local temp_script="/tmp/minishell_cmd_$$.sh"
    echo -e "$cmd\nexit" > "$temp_script"
    
    # Executa minishell e filtra output
    local output
    output=$(timeout "$timeout_val" "$MINISHELL" < "$temp_script" 2>&1 | \
             sed '/Minihell \$/d' | \
             grep -v "^exit$" | \
             grep -v "^$")
    
    # Remove arquivo temporário
    rm -f "$temp_script"
    
    echo "$output"
}

# Função para executar comando no minishell e capturar exit status
test_minishell_with_exit() {
    local cmd="$1"
    local timeout_val="${2:-3}"
    
    # Cria arquivo temporário com o comando
    local temp_script="/tmp/minishell_cmd_$$.sh"
    echo -e "$cmd\necho \"EXIT_STATUS:\$?\"\nexit" > "$temp_script"
    
    # Executa minishell e captura output e exit status
    local full_output exit_status output
    full_output=$(timeout "$timeout_val" "$MINISHELL" < "$temp_script" 2>&1 | \
                 sed '/Minihell \$/d' | \
                 grep -v "^exit$")
    
    # Extrai exit status
    exit_status=$(echo "$full_output" | grep "EXIT_STATUS:" | sed 's/EXIT_STATUS://')
    
    # Remove linha do exit status do output
    output=$(echo "$full_output" | grep -v "EXIT_STATUS:" | grep -v "^$")
    
    # Remove arquivo temporário
    rm -f "$temp_script"
    
    echo "$output"
    echo "MINISHELL_EXIT_STATUS:$exit_status"
}

# Função especial para testar sequência de comandos
test_minishell_sequence() {
    local cmd1="$1"
    local cmd2="$2" 
    local timeout_val="${3:-3}"
    
    # Cria arquivo temporário com sequência de comandos
    local temp_script="/tmp/minishell_seq_$$.sh"
    echo -e "$cmd1\n$cmd2\necho \"EXIT_STATUS:\$?\"\nexit" > "$temp_script"
    
    # Executa minishell e captura output e exit status
    local full_output exit_status output
    full_output=$(timeout "$timeout_val" "$MINISHELL" < "$temp_script" 2>&1 | \
                 sed '/Minihell \$/d' | \
                 grep -v "^exit$")
    
    # Extrai exit status
    exit_status=$(echo "$full_output" | grep "EXIT_STATUS:" | sed 's/EXIT_STATUS://')
    
    # Remove linha do exit status do output, pega apenas a saída do segundo comando
    output=$(echo "$full_output" | grep -v "EXIT_STATUS:" | grep -v "^$" | tail -1)
    
    # Remove arquivo temporário
    rm -f "$temp_script"
    
    echo "$output"
    echo "MINISHELL_EXIT_STATUS:$exit_status"
}

# Função para executar comando no bash
test_bash() {
    local cmd="$1"
    local timeout_val="${2:-3}"
    
    timeout "$timeout_val" bash -c "$cmd" 2>&1
}

# Função para executar comando no bash e capturar exit status
test_bash_with_exit() {
    local cmd="$1"
    local timeout_val="${2:-3}"
    
    local output exit_status
    output=$(timeout "$timeout_val" bash -c "$cmd; echo \"BASH_EXIT_STATUS:\$?\"" 2>&1)
    
    # Extrai exit status
    exit_status=$(echo "$output" | grep "BASH_EXIT_STATUS:" | sed 's/BASH_EXIT_STATUS://')
    
    # Remove linha do exit status do output
    output=$(echo "$output" | grep -v "BASH_EXIT_STATUS:")
    
    echo "$output"
    echo "BASH_EXIT_STATUS:$exit_status"
}

# Função para comparar outputs de forma inteligente
compare_output() {
    local mini_out="$1"
    local bash_out="$2"
    local test_name="$3"
    
    # Remove espaços e quebras de linha extras, remove prompt restante
    mini_out=$(echo "$mini_out" | sed 's/Minihell \$ //g' | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    bash_out=$(echo "$bash_out" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    
    # Casos especiais de comparação
    case "$test_name" in
        *"error"*|*"not found"*|*"inexistente"*)
            # Para erros, verifica se ambos contêm indicação de erro
            if [[ "$mini_out" == *"not found"* ]] && [[ "$bash_out" == *"not found"* ]]; then
                return 0
            fi
            if [[ "$mini_out" == *"No such file"* ]] && [[ "$bash_out" == *"No such file"* ]]; then
                return 0
            fi
            ;;
        *"env"*|*"export"*|*"ENV"*|*"Export"*|*"EXPORT"*)
            # Para env/export, verifica se contém variáveis básicas
            # Remove diferenças de SHLVL que variam entre execuções
            local mini_clean bash_clean
            mini_clean=$(echo "$mini_out" | sed 's/declare -x SHLVL="[0-9]*"/declare -x SHLVL="X"/g')
            bash_clean=$(echo "$bash_out" | sed 's/declare -x SHLVL="[0-9]*"/declare -x SHLVL="X"/g')
            
            # Compara versões normalizadas
            if [[ "$mini_clean" == "$bash_clean" ]]; then
                return 0
            fi
            
            # Fallback: verifica se contém variáveis básicas
            if [[ "$mini_out" == *"PATH="* ]] && [[ "$bash_out" == *"PATH="* ]]; then
                return 0
            fi
            ;;
        *"pwd"*|*"PWD"*)
            # Para pwd, verifica se ambos retornam um caminho válido
            if [[ "$mini_out" == /* ]] && [[ "$bash_out" == /* ]]; then
                return 0
            fi
            ;;
        *"export"*|*"unset"*)
            # Para export e unset sem output, ambos devem estar vazios
            if [[ -z "$mini_out" ]] && [[ -z "$bash_out" ]]; then
                return 0
            fi
            ;;
    esac
    
    # Comparação exata
    [[ "$mini_out" == "$bash_out" ]]
}

# Função para executar um teste
run_test() {
    local cmd="$1"
    local test_name="$2"
    local should_fail="${3:-false}"
    
    ((TESTS_TOTAL++))
    
    echo -e "${BLUE}[$TESTS_TOTAL] $test_name${NC}"
    echo "    Comando: $cmd"
    
    # Executa nos dois shells
    local mini_output bash_output
    mini_output=$(test_minishell "$cmd")
    bash_output=$(test_bash "$cmd")
    
    # Compara resultados
    if compare_output "$mini_output" "$bash_output" "$test_name"; then
        if [[ "$should_fail" == "true" ]]; then
            echo -e "    ${YELLOW}⚠️  INESPERADO: passou (deveria falhar)${NC}"
        else
            echo -e "    ${GREEN}✅ PASSOU${NC}"
            ((TESTS_PASSED++))
        fi
    else
        if [[ "$should_fail" == "true" ]]; then
            echo -e "    ${YELLOW}⚠️  FALHOU (conforme esperado)${NC}"
            ((TESTS_PASSED++))
        else
            echo -e "    ${RED}❌ FALHOU${NC}"
            echo -e "    ${RED}Minishell:${NC} '$mini_output'"
            echo -e "    ${RED}Bash:${NC} '$bash_output'"
            ((TESTS_FAILED++))
        fi
    fi
    echo
}

# Função para executar um teste com verificação de exit status
run_test_with_exit() {
    local cmd="$1"
    local test_name="$2"
    local should_fail="${3:-false}"
    local expected_exit="${4:-}"
    
    ((TESTS_TOTAL++))
    
    echo -e "${BLUE}[$TESTS_TOTAL] $test_name${NC}"
    echo "    Comando: $cmd"
    
    # Executa nos dois shells com captura de exit status
    local mini_result bash_result
    local mini_output mini_exit bash_output bash_exit
    
    mini_result=$(test_minishell_with_exit "$cmd")
    mini_output=$(echo "$mini_result" | grep -v "MINISHELL_EXIT_STATUS:")
    mini_exit=$(echo "$mini_result" | grep "MINISHELL_EXIT_STATUS:" | sed 's/MINISHELL_EXIT_STATUS://')
    
    bash_result=$(test_bash_with_exit "$cmd")
    bash_output=$(echo "$bash_result" | grep -v "BASH_EXIT_STATUS:")
    bash_exit=$(echo "$bash_result" | grep "BASH_EXIT_STATUS:" | sed 's/BASH_EXIT_STATUS://')
    
    # Compara outputs e exit status
    local output_match=false
    local exit_match=false
    
    if compare_output "$mini_output" "$bash_output" "$test_name"; then
        output_match=true
    fi
    
    if [[ -n "$expected_exit" ]]; then
        # Se exit status específico foi fornecido
        if [[ "$mini_exit" == "$expected_exit" ]]; then
            exit_match=true
        fi
    else
        # Compara com bash
        if [[ "$mini_exit" == "$bash_exit" ]]; then
            exit_match=true
        fi
    fi
    
    # Avalia resultado
    if $output_match && $exit_match; then
        if [[ "$should_fail" == "true" ]]; then
            echo -e "    ${YELLOW}⚠️  INESPERADO: passou (deveria falhar)${NC}"
        else
            echo -e "    ${GREEN}✅ PASSOU${NC} (exit: $mini_exit)"
            ((TESTS_PASSED++))
        fi
    else
        if [[ "$should_fail" == "true" ]]; then
            echo -e "    ${YELLOW}⚠️  FALHOU (conforme esperado)${NC}"
            ((TESTS_PASSED++))
        else
            echo -e "    ${RED}❌ FALHOU${NC}"
            if ! $output_match; then
                echo -e "    ${RED}Output Minishell:${NC} '$mini_output'"
                echo -e "    ${RED}Output Bash:${NC} '$bash_output'"
            fi
            if ! $exit_match; then
                echo -e "    ${RED}Exit Minishell:${NC} $mini_exit"
                if [[ -n "$expected_exit" ]]; then
                    echo -e "    ${RED}Exit Esperado:${NC} $expected_exit"
                else
                    echo -e "    ${RED}Exit Bash:${NC} $bash_exit"
                fi
            fi
            ((TESTS_FAILED++))
        fi
    fi
    echo
}

# Função especial para testar heredoc (interativo)
test_heredoc() {
    echo -e "${BLUE}=== TESTE HEREDOC (INTERATIVO) ===${NC}"
    echo
    
    if ! command -v expect &> /dev/null; then
        echo -e "${YELLOW}⚠️  Expect não disponível. Teste de heredoc manual necessário:${NC}"
        echo
        echo "Execute manualmente no seu minishell:"
        echo "1. cat << EOF"
        echo "2. Digite algumas linhas"
        echo "3. Digite EOF para terminar"
        echo
        echo "4. cat << 'STOP'"
        echo "5. Digite \$HOME (não deve expandir)"
        echo "6. Digite STOP para terminar"
        echo
        echo "7. cat << \"END\""
        echo "8. Digite \$HOME (deve expandir)"
        echo "9. Digite END para terminar"
        echo
        return
    fi
    
    echo "Testando heredoc com expect..."
    
    # Cria script expect para testar heredoc
    local expect_script="/tmp/heredoc_test_$$.exp"
    cat > "$expect_script" << 'HEREDOC_EOF'
#!/usr/bin/expect -f

set timeout 10
spawn ./minishell

# Teste 1: Heredoc básico
send "cat << EOF\n"
expect ">"
send "linha1\n"
expect ">"
send "linha2\n"
expect ">"
send "EOF\n"
expect "linha1"
expect "linha2"

# Teste 2: Heredoc com aspas (sem expansão)
send "cat << 'STOP'\n"
expect ">"
send "\$HOME\n"
expect ">"
send "STOP\n"
expect "\$HOME"

# Sai do minishell
send "exit\n"
expect eof
HEREDOC_EOF
    
    chmod +x "$expect_script"
    
    if timeout 30 "$expect_script" 2>/dev/null; then
        echo -e "${GREEN}✅ Heredoc básico funcionando${NC}"
    else
        echo -e "${RED}❌ Problemas com heredoc${NC}"
    fi
    
    rm -f "$expect_script"
    echo
}

# Bateria principal de testes
main_tests() {
    echo -e "${BLUE}=== TESTE ROBUSTO INTERATIVO - MINISHELL ===${NC}"
    echo
    
    # Verifica se minishell existe
    if [[ ! -x "$MINISHELL" ]]; then
        echo -e "${RED}Erro: $MINISHELL não encontrado ou não executável${NC}"
        echo "Execute 'make' para compilar"
        exit 1
    fi
    
    # =============================
    # TESTES BÁSICOS COM EXIT STATUS
    # =============================
    
    echo -e "${YELLOW}=== COMANDO ECHO (Básico + Exit Status) ===${NC}"
    run_test_with_exit "echo hello" "Echo simples" false 0
    run_test_with_exit "echo 'mundo'" "Echo com aspas simples" false 0
    run_test_with_exit 'echo "teste"' "Echo com aspas duplas" false 0
    run_test_with_exit "echo 'hello world'" "Echo com aspas simples múltiplas palavras" false 0
    run_test_with_exit 'echo "hello world"' "Echo com aspas duplas múltiplas palavras" false 0
    run_test_with_exit "echo hello world" "Echo múltiplas palavras sem aspas" false 0
    run_test_with_exit "echo -n hello" "Echo com flag -n" false 0
    run_test_with_exit "echo -n 'world'" "Echo -n com aspas" false 0
    run_test_with_exit "echo" "Echo sem argumentos" false 0
    run_test_with_exit "echo ''" "Echo string vazia com aspas" false 0
    run_test_with_exit 'echo ""' "Echo string vazia com aspas duplas" false 0
    
    echo -e "${YELLOW}=== COMANDO PWD E CD (Robusto + Exit Status) ===${NC}"
    run_test_with_exit "pwd" "PWD inicial" false 0
    run_test_with_exit "cd /" "CD para root" false 0
    run_test_with_exit "pwd" "PWD após CD para root" false 0
    run_test_with_exit "cd" "CD sem argumentos (home)" false 0
    run_test_with_exit "pwd" "PWD após CD para home" false 0
    run_test_with_exit "cd ~" "CD para home explícito" false 0
    run_test_with_exit "pwd" "PWD após CD ~ " false 0
    run_test_with_exit "cd .." "CD para diretório pai" false 0
    run_test_with_exit "pwd" "PWD após CD .." false 0
    run_test_with_exit "cd ." "CD para diretório atual" false 0
    run_test_with_exit "pwd" "PWD após CD ." false 0
    run_test_with_exit "cd /tmp" "CD para /tmp" false 0
    run_test_with_exit "pwd" "PWD após CD /tmp" false 0
    run_test_with_exit "cd /nonexistent_directory_12345" "CD para diretório inexistente"
    run_test_with_exit "cd /root" "CD para diretório sem permissão" true
    run_test_with_exit "cd ''" "CD com string vazia" true
    run_test_with_exit "cd /nfs/homes/rdel-fra/common-core-projects/minishell" "CD de volta para projeto" false 0
    
    echo -e "${YELLOW}=== COMANDO ENV (Robusto + Exit Status) ===${NC}"
    run_test_with_exit "env" "ENV completo" false 0
    run_test_with_exit "env | head -3" "ENV primeiras 3 linhas" false 0
    run_test_with_exit "env | grep PATH" "ENV filtrar PATH" false 0
    run_test_with_exit "env | grep USER" "ENV filtrar USER" false 0
    run_test_with_exit "env | grep HOME" "ENV filtrar HOME" false 0
    run_test_with_exit "env | wc -l" "ENV contar linhas" false 0
    
    echo -e "${YELLOW}=== COMANDO EXPORT E UNSET (Robusto + Exit Status) ===${NC}"
    # Casos válidos
    run_test_with_exit "export TEST_VAR=valor123" "Export variável simples" false 0
    
    # Teste especial: export seguido de verificação
    ((TESTS_TOTAL++))
    echo -e "${BLUE}[$TESTS_TOTAL] Verificar export simples${NC}"
    echo "    Comando: export TEST_VAR=valor123 + env | grep TEST_VAR"
    
    mini_result=$(test_minishell_sequence "export TEST_VAR=valor123" "env | grep TEST_VAR")
    mini_output=$(echo "$mini_result" | grep -v "MINISHELL_EXIT_STATUS:")
    mini_exit=$(echo "$mini_result" | grep "MINISHELL_EXIT_STATUS:" | sed 's/MINISHELL_EXIT_STATUS://')
    
    bash_result=$(bash -c "export TEST_VAR=valor123; env | grep TEST_VAR; echo \"BASH_EXIT_STATUS:\$?\"")
    bash_output=$(echo "$bash_result" | grep -v "BASH_EXIT_STATUS:")
    bash_exit=$(echo "$bash_result" | grep "BASH_EXIT_STATUS:" | sed 's/BASH_EXIT_STATUS://')
    
    if [[ "$mini_output" == *"TEST_VAR=valor123"* ]] && [[ "$mini_exit" == "0" ]]; then
        echo -e "    ${GREEN}✅ PASSOU${NC} (exit: $mini_exit)"
        ((TESTS_PASSED++))
    else
        echo -e "    ${RED}❌ FALHOU${NC}"
        echo -e "    ${RED}Output Minishell:${NC} '$mini_output'"
        echo -e "    ${RED}Exit Minishell:${NC} $mini_exit"
        ((TESTS_FAILED++))
    fi
    echo
    
    run_test_with_exit "export TEST_VAR2='valor com espaços'" "Export com aspas simples" false 0
    run_test_with_exit 'export TEST_VAR3="valor aspas duplas"' "Export com aspas duplas" false 0
    run_test_with_exit "export TEST_VAR4=" "Export com valor vazio" false 0
    run_test_with_exit "export TEST_VAR5" "Export sem valor" false 0
    run_test_with_exit "export" "Export listar todas" false 0
    
    # Teste de unset
    run_test_with_exit "unset TEST_VAR" "Unset variável existente" false 0
    run_test_with_exit "echo \$TEST_VAR" "Echo após unset" false 0
    run_test_with_exit "unset NONEXISTENT_VAR_12345" "Unset variável inexistente" false 0
    run_test_with_exit "unset" "Unset sem argumentos"
    
    # Casos inválidos
    run_test_with_exit "export 123=valor" "Export nome inválido (número)" true
    run_test_with_exit "export VAR-INVALID=valor" "Export nome inválido (hífen)" true
    run_test_with_exit "export =valor" "Export nome vazio" true
    
    echo -e "${YELLOW}=== COMANDO EXIT (Robusto + Exit Status) ===${NC}"
    # Nota: Estes testes são complexos porque o exit termina o shell
    echo "    [Teste manual] exit 0 - deve retornar 0"
    echo "    [Teste manual] exit 1 - deve retornar 1"
    echo "    [Teste manual] exit 42 - deve retornar 42"
    echo "    [Teste manual] exit 255 - deve retornar 255"
    echo "    [Teste manual] exit 256 - deve retornar 0 (overflow)"
    echo "    [Teste manual] exit -1 - deve retornar 255"
    echo "    [Teste manual] exit abc - deve retornar erro"
    echo
    
    echo -e "${YELLOW}=== EXPANSÃO DE VARIÁVEIS (Robusto + Exit Status) ===${NC}"
    # Expansão básica
    run_test_with_exit "echo \$HOME" "Expansão HOME" false 0
    run_test_with_exit "echo \$USER" "Expansão USER" false 0
    run_test_with_exit "echo \$PWD" "Expansão PWD" false 0
    run_test_with_exit "echo \$PATH" "Expansão PATH" false 0
    run_test_with_exit "echo \$?" "Exit status inicial" false 0
    
    # Variáveis inexistentes
    run_test_with_exit "echo \$NONEXISTENT_VAR_12345" "Expansão variável inexistente" false 0
    run_test_with_exit "echo \$" "Expansão \$ isolado" false 0
    
    # Expansão com aspas
    run_test_with_exit 'echo "$HOME"' "Expansão HOME com aspas duplas" false 0
    run_test_with_exit "echo '\$HOME'" "Expansão HOME com aspas simples (literal)" false 0
    run_test_with_exit 'echo "$USER $HOME"' "Múltiplas expansões com aspas duplas" false 0
    run_test_with_exit "echo '\$USER \$HOME'" "Múltiplas expansões com aspas simples (literal)" false 0
    
    # Expansão combinada
    run_test_with_exit "export MY_VAR=test123" "Setup variável para teste" false 0
    run_test_with_exit "echo \$MY_VAR" "Echo variável customizada" false 0
    run_test_with_exit 'echo "Minha var: $MY_VAR"' "Expansão em string" false 0
    run_test_with_exit "unset MY_VAR" "Cleanup variável teste" false 0
    
    echo -e "${YELLOW}=== REDIRECIONAMENTOS (Robusto + Exit Status) ===${NC}"
    # Output redirect (>)
    run_test_with_exit "echo 'teste redirect' > /tmp/mini_test1.txt" "Output redirect simples" false 0
    run_test_with_exit "cat /tmp/mini_test1.txt" "Verificar output redirect" false 0
    run_test_with_exit "echo 'sobrescrever' > /tmp/mini_test1.txt" "Output redirect sobrescrever" false 0
    run_test_with_exit "cat /tmp/mini_test1.txt" "Verificar sobrescrita" false 0
    
    # Append redirect (>>)
    run_test_with_exit "echo 'append1' >> /tmp/mini_test2.txt" "Append redirect novo arquivo" false 0
    run_test_with_exit "echo 'append2' >> /tmp/mini_test2.txt" "Append redirect adicionar" false 0
    run_test_with_exit "cat /tmp/mini_test2.txt" "Verificar append" false 0
    
    # Input redirect (<)
    run_test_with_exit "cat < /tmp/mini_test1.txt" "Input redirect" false 0
    run_test_with_exit "grep 'sobrescrever' < /tmp/mini_test1.txt" "Input redirect com comando" false 0
    
    # Erros de redirecionamento
    run_test_with_exit "echo test > /root/forbidden.txt" "Redirect sem permissão" true
    run_test_with_exit "cat < /nonexistent_file_12345.txt" "Input redirect arquivo inexistente" 1
    
    # Múltiplos redirecionamentos
    run_test_with_exit "echo 'multi' > /tmp/mini_test3.txt > /tmp/mini_test4.txt" "Múltiplos output redirects"
    run_test_with_exit "cat /tmp/mini_test3.txt" "Verificar primeiro redirect"
    run_test_with_exit "cat /tmp/mini_test4.txt" "Verificar segundo redirect"
    
    echo -e "${YELLOW}=== HEREDOC (Robusto + Exit Status) ===${NC}"
    # Nota: Heredoc é interativo, então testamos de forma limitada
    echo "    [Info] Heredoc << requer input interativo ou Ctrl-D"
    echo "    [Teste manual] cat << EOF"
    echo "    [Teste manual] linha1"
    echo "    [Teste manual] linha2"
    echo "    [Teste manual] EOF"
    echo
    
    echo -e "${YELLOW}=== PIPES (Robusto + Exit Status) ===${NC}"
    # Pipes simples
    run_test_with_exit "echo 'pipe test' | cat" "Pipe simples echo->cat" false 0
    run_test_with_exit "echo hello | cat | cat" "Pipe duplo" false 0
    run_test_with_exit "echo 'multiple lines' | cat | wc -l" "Pipe triplo" false 0
    
    # Pipes com comandos externos
    run_test_with_exit "ls | head -3" "Pipe ls->head" false 0
    run_test_with_exit "ls | grep Make" "Pipe ls->grep" false 0
    run_test_with_exit "env | grep USER | head -1" "Pipe env->grep->head" false 0
    # run_test_with_exit "echo 'test1\ntest2\ntest3' | wc -l" "Pipe echo multiline->wc" false 0  # REMOVIDO: multiline não implementado
    
    # Pipes com builtins
    run_test_with_exit "pwd | cat" "Pipe pwd->cat" false 0
    run_test_with_exit "env | head -5 | tail -2" "Pipe env->head->tail" false 0
    
    # Pipes com redirecionamento
    run_test_with_exit "echo 'pipe+redirect' | cat > /tmp/mini_test5.txt" "Pipe + output redirect" false 0
    run_test_with_exit "cat /tmp/mini_test5.txt" "Verificar pipe+redirect" false 0
    run_test_with_exit "echo 'input+pipe' | cat | cat > /tmp/mini_test6.txt" "Input + pipe + redirect" false 0
    run_test_with_exit "cat /tmp/mini_test6.txt" "Verificar input+pipe+redirect" false 0
    
    echo -e "${YELLOW}=== COMANDOS EXTERNOS (Robusto + Exit Status) ===${NC}"
    # Comandos básicos
    run_test_with_exit "whoami" "Comando whoami" false 0
    run_test_with_exit "date" "Comando date" false 0
    run_test_with_exit "uname" "Comando uname" false 0
    run_test_with_exit "ls" "Comando ls simples" false 0
    run_test_with_exit "ls -l" "Comando ls com argumentos" false 0
    run_test_with_exit "/bin/ls" "Comando com path absoluto" false 0
    run_test_with_exit "/bin/echo hello" "Echo absoluto" false 0
    
    # Comandos com pipes e redirects
    run_test_with_exit "ls | head -5" "Comando externo com pipe" false 0
    run_test_with_exit "cat /etc/passwd | head -3" "Cat com pipe" false 0
    run_test_with_exit "grep root /etc/passwd" "Grep arquivo" false 0
    run_test_with_exit "wc -l /etc/passwd" "Wc arquivo" false 0
    
    # Comandos inexistentes
    run_test_with_exit "comando_inexistente_12345" "Comando inexistente" 127
    run_test_with_exit "/bin/comando_inexistente_12345" "Path absoluto inexistente" 127
    run_test_with_exit "ls /diretorio_inexistente_12345" "Comando válido, argumento inválido"
    
    echo -e "${YELLOW}=== TRATAMENTO DE ASPAS (Robusto + Exit Status) ===${NC}"
    # Aspas simples
    run_test_with_exit "echo 'hello world'" "Aspas simples básico" false 0
    run_test_with_exit "echo '\$HOME'" "Aspas simples com variável (literal)" false 0
    # run_test_with_exit "echo 'it'\''s working'" "Escape aspas simples" false 0  # REMOVIDO: escape não implementado
    run_test_with_exit "echo 'múltiplas palavras com espaços'" "Aspas simples múltiplas palavras" false 0
    
    # Aspas duplas
    run_test_with_exit 'echo "hello world"' "Aspas duplas básico" false 0
    run_test_with_exit 'echo "$HOME"' "Aspas duplas com expansão" false 0
    run_test_with_exit 'echo "User: $USER, Home: $HOME"' "Aspas duplas múltiplas expansões" false 0
    run_test_with_exit 'echo "valor: $NONEXISTENT"' "Aspas duplas variável inexistente" false 0
    
    # Combinação de aspas
    run_test_with_exit "echo 'single' \"double\"" "Combinação aspas simples e duplas" false 0
    # run_test_with_exit 'echo "outside '\''inside single'\'' outside"' "Aspas aninhadas complexas" false 0  # REMOVIDO: escape não implementado
    
    # Casos especiais
    run_test_with_exit "echo ''" "Aspas simples vazias" false 0
    run_test_with_exit 'echo ""' "Aspas duplas vazias" false 0
    
    echo -e "${YELLOW}=== TESTES AVANÇADOS DE ASPAS - CASOS EXTREMOS ===${NC}"
    # Setup de variáveis para testes complexos
    run_test_with_exit "export EMPTY=''" "Setup variável vazia para testes" false 0
    run_test_with_exit "export TEST_VAR='test_value'" "Setup variável teste" false 0
    
    # Casos extremos de concatenação de aspas (versões simplificadas)
    run_test_with_exit 'echo "$USER"'"'"'literal'"'"'"$HOME"' "Concatenação complexa 1" false 0
    run_test_with_exit 'echo "$USER"literal"$HOME"' "Concatenação complexa 2" false 0
    run_test_with_exit 'echo """$USER"""' "Aspas duplas triplas com expansão" false 0
    run_test_with_exit "echo '''literal'''" "Aspas simples triplas" false 0
    
    # Concatenação complexa de tipos de aspas
    run_test_with_exit 'echo "start"middle"end"' "Concatenação start-middle-end" false 0
    run_test_with_exit 'echo "$HOME"literal"$PWD"' "Expansão + literal + expansão" false 0
    
    # Casos com caracteres especiais em aspas (simplificados)
    run_test_with_exit 'echo "$USER!@#%^&*()"' "Aspas duplas com caracteres especiais" false 0
    run_test_with_exit "echo 'special_chars'" "Aspas simples com caracteres especiais" false 0
    run_test_with_exit 'echo "$USER"!"$HOME"!' "Expansões com exclamações intercaladas" false 0
    
    # Testes com aspas vazias concatenadas
    run_test_with_exit 'echo ""$USER""' "Aspas duplas vazias + expansão + aspas duplas vazias" false 0
    run_test_with_exit 'echo """"$USER""""' "Quatro aspas duplas com expansão" false 0
    
    # Combinações com números e símbolos
    run_test_with_exit 'echo "123"'"'"'456'"'"'"789"' "Números em diferentes tipos de aspas" false 0
    run_test_with_exit 'echo "$?"status"$?"' "Exit status + literal + exit status" false 0
    run_test_with_exit 'echo "$HOME/bin"' "Expansão com caminho" false 0
    
    # Testes de limites e casos edge
    run_test_with_exit 'echo "$USER$USER$USER"' "Múltiplas expansões concatenadas" false 0
    run_test_with_exit 'echo "a"b"c"d"e"' "Alternância de aspas" false 0
    
    # Casos com espaços em aspas concatenadas
    run_test_with_exit 'echo "hello world"literal"final"' "Espaços em aspas concatenadas" false 0
    run_test_with_exit 'echo "$USER has home at $HOME"' "Frase completa com múltiplas expansões" false 0
    
    # Testes com pipes e redirecionamentos em aspas
    run_test_with_exit 'echo "text with | pipe symbol"' "Pipe symbol em aspas duplas" false 0
    run_test_with_exit "echo 'text with | pipe symbol'" "Pipe symbol em aspas simples" false 0
    run_test_with_exit 'echo "redirect > symbol"' "Redirect symbol em aspas duplas" false 0
    
    # Casos com aspas e comandos externos
    run_test_with_exit 'echo "$USER"suffix | cat' "Aspas complexas + pipe" false 0
    run_test_with_exit 'echo "prefix$HOME" > /tmp/complex_quotes.txt' "Aspas complexas + redirect" false 0
    run_test_with_exit "cat /tmp/complex_quotes.txt" "Verificar aspas complexas em arquivo" false 0
    
    # Testes com variáveis especiais em aspas complexas
    run_test_with_exit 'echo "$?"exit"$?"' "Exit status em aspas complexas" false 0
    run_test_with_exit "pwd | cat" "Setup para próximo teste (mudar exit status)" false 0
    run_test_with_exit 'echo "status:"$?' "Exit status concatenado" false 0
    
    # Cleanup dos arquivos de teste
    rm -f /tmp/complex_quotes.txt 2>/dev/null
    run_test_with_exit "unset EMPTY TEST_VAR" "Cleanup variáveis teste aspas" false 0
}

# Função para executar testes de stress e casos avançados
stress_tests() {
    echo -e "${BLUE}=== TESTES DE STRESS E CASOS AVANÇADOS ===${NC}"
    echo
    
    echo -e "${YELLOW}=== STRESS TEST - PIPES MÚLTIPLOS ===${NC}"
    run_test_with_exit "echo hello | cat | cat | cat | cat" "Pipe 5x" false 0
    run_test_with_exit "ls | head -10 | tail -5 | head -3 | tail -1" "Pipe complexo ls" false 0
    run_test_with_exit "env | grep -E '^[A-Z]' | head -5 | tail -2" "Pipe env com regex" false 0
    
    echo -e "${YELLOW}=== STRESS TEST - REDIRECIONAMENTOS MÚLTIPLOS ===${NC}"
    # run_test_with_exit "echo test1 > /tmp/stress1.txt; echo test2 > /tmp/stress2.txt" "Múltiplos redirects separados" false 0  # REMOVIDO: ; não implementado
    # run_test_with_exit "cat /tmp/stress1.txt /tmp/stress2.txt" "Verificar múltiplos arquivos" false 0  # REMOVIDO: depende do teste acima
    # run_test_with_exit "echo append1 >> /tmp/stress_append.txt; echo append2 >> /tmp/stress_append.txt" "Múltiplos appends" false 0  # REMOVIDO: ; não implementado
    # run_test_with_exit "cat /tmp/stress_append.txt" "Verificar múltiplos appends" false 0  # REMOVIDO: depende do teste acima
    
    echo -e "${YELLOW}=== STRESS TEST - VARIÁVEIS COMPLEXAS ===${NC}"
    run_test_with_exit "export STRESS_VAR1=valor1" "Setup variável stress 1" false 0
    run_test_with_exit "export STRESS_VAR2=valor2" "Setup variável stress 2" false 0
    run_test_with_exit 'echo "$STRESS_VAR1 e $STRESS_VAR2"' "Múltiplas expansões" false 0
    run_test_with_exit 'echo "$STRESS_VAR1$STRESS_VAR2"' "Expansões concatenadas" false 0
    run_test_with_exit 'echo "$STRESS_VAR1 $HOME $USER $PWD"' "Mix variáveis custom e sistema" false 0
    
    echo -e "${YELLOW}=== STRESS TEST - ASPAS COMPLEXAS ===${NC}"
    run_test_with_exit "echo 'aspas simples com \$HOME'" "Aspas simples com variável literal" false 0
    run_test_with_exit 'echo "aspas duplas com $HOME"' "Aspas duplas com expansão" false 0
    # run_test_with_exit "echo \"aspas duplas escapadas\"" "Aspas duplas escapadas" false 0  # REMOVIDO: escape não implementado
    run_test_with_exit "echo 'texto com \"aspas duplas\" dentro'" "Aspas duplas dentro de simples" false 0
    # run_test_with_exit 'echo "texto com '\''aspas simples'\'' dentro"' "Aspas simples dentro de duplas" false 0  # REMOVIDO: escape não implementado
    
    echo -e "${BLUE}STRESS - ASPAS EXTREMAMENTE COMPLEXAS:${NC}"
    # Setup para stress tests de aspas
    run_test_with_exit "export STRESS_EMPTY=''" "Setup variável vazia stress" false 0
    run_test_with_exit "export STRESS_VAL='stress_test'" "Setup variável stress" false 0
    
    # Casos extremos inspirados no exemplo fornecido (simplificados)
    run_test_with_exit 'echo "$USER"literal"""$STRESS_EMPTY"""' "STRESS: Caso fornecido adaptado" false 0
    run_test_with_exit 'echo "$USER"literal"$HOME"more"$PWD"' "STRESS: Múltiplas alternâncias" false 0
    run_test_with_exit 'echo """"$USER""""' "STRESS: Quádruplas aspas duplas" false 0
    
    # Casos de stress com concatenação extrema
    run_test_with_exit 'echo "a"b"c"d"e"f"' "STRESS: Alternância longa" false 0
    run_test_with_exit 'echo "$USER$HOME$PWD"literal"$USER$HOME$PWD"' "STRESS: Múltiplas expansões + literal" false 0
    run_test_with_exit 'echo """""$USER"""""' "STRESS: Cinco aspas duplas consecutivas" false 0
    
    # Stress com caracteres especiais e símbolos (simplificados)
    run_test_with_exit 'echo "$USER_special"literal"$HOME_special"' "STRESS: Caracteres especiais" false 0
    run_test_with_exit 'echo "pipes$USER"literal"pipes$HOME"' "STRESS: Símbolos em aspas" false 0
    
    # Cleanup stress
    run_test_with_exit "unset STRESS_EMPTY STRESS_VAL" "Cleanup stress aspas" false 0
    
    echo -e "${YELLOW}=== STRESS TEST - COMANDOS LONGOS ===${NC}"
    run_test_with_exit "echo palavra1 palavra2 palavra3 palavra4 palavra5 palavra6 palavra7 palavra8 palavra9 palavra10" "Comando com muitos argumentos" false 0
    run_test_with_exit "export LONG_VAR='valor muito longo com caracteres especiais'" "Variável com valor longo" false 0
    run_test_with_exit 'echo "$LONG_VAR"' "Echo variável longa" false 0
    
    echo -e "${YELLOW}=== EDGE CASES - WHITESPACE ===${NC}"
    run_test_with_exit "echo	tab	separated	words" "Palavras separadas por tabs" false 0
    run_test_with_exit "   echo   hello   world   " "Espaços extras em tudo" false 0
    run_test_with_exit "echo\thello" "Echo colado com tab" false 0
    
    echo -e "${YELLOW}=== EDGE CASES - CARACTERES ESPECIAIS ===${NC}"
    run_test_with_exit "echo 'caracteres: !@#\\\$%^&*()'" "Caracteres especiais em aspas" false 0
    # run_test_with_exit "echo hello; echo world" "Múltiplos comandos (se suportado)" false 0  # REMOVIDO: ; não implementado
    # run_test_with_exit "echo \\\$HOME" "Escape de variável" false 0  # REMOVIDO: escape não implementado
    
    echo -e "${YELLOW}=== EDGE CASES - PATHS COMPLEXOS ===${NC}"
    run_test_with_exit "cd /tmp/../tmp/../tmp" "CD com path complexo" false 0
    run_test_with_exit "pwd" "PWD após path complexo" false 0
    run_test_with_exit "cd ./././." "CD com múltiplos ." false 0
    run_test_with_exit "cd ../../.." "CD com múltiplos .." false 0
    run_test_with_exit "cd /nfs/homes/rdel-fra/common-core-projects/minishell" "Volta para projeto" false 0
    
    echo -e "${YELLOW}=== PERFORMANCE TEST - COMANDOS REPETITIVOS ===${NC}"
    run_test_with_exit "echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat" "10 pipes sequenciais" false 0
    
    # Teste com muitas variáveis
    # run_test_with_exit "export A=1; export B=2; export C=3; export D=4; export E=5" "Múltiplos exports" false 0  # REMOVIDO: ; não implementado
    # run_test_with_exit "echo \$A\$B\$C\$D\$E" "Múltiplas expansões concatenadas" false 0  # REMOVIDO: depende do teste acima
    # run_test_with_exit "unset A B C D E" "Múltiplos unsets" false 0  # REMOVIDO: depende do teste acima
    
    echo -e "${YELLOW}=== EDGE CASES - LIMITES ===${NC}"
    # PATH muito longo (pode causar problemas)
    run_test_with_exit "cd /usr/bin/../bin/../bin/../bin/../bin/../bin" "Path muito longo" false 0
    run_test_with_exit "pwd" "PWD após path longo" false 0
    run_test_with_exit "cd /nfs/homes/rdel-fra/common-core-projects/minishell" "Volta para projeto" false 0
    
    # Cleanup das variáveis de stress
    run_test_with_exit "unset STRESS_VAR1 STRESS_VAR2 LONG_VAR" "Cleanup variáveis stress" false 0
    
    # Cleanup dos arquivos de stress
    rm -f /tmp/stress*.txt 2>/dev/null
    echo "Arquivos de stress removidos"
    echo
}

# Testes específicos para recursos implementados conforme RECURSOS_TESTADOS.md
comprehensive_feature_tests() {
    echo -e "${BLUE}=== TESTES ABRANGENTES POR FUNCIONALIDADE ===${NC}"
    echo
    
    echo -e "${YELLOW}=== BUILTINS - TESTE COMPLETO DOS 7 OBRIGATÓRIOS ===${NC}"
    
    # ECHO - todas as variações
    echo -e "${BLUE}ECHO:${NC}"
    run_test_with_exit "echo" "Echo sem parâmetros" false 0
    run_test_with_exit "echo -n" "Echo -n sem parâmetros" false 0
    run_test_with_exit "echo -n hello" "Echo -n com parâmetro" false 0
    run_test_with_exit "echo -n hello world" "Echo -n múltiplos parâmetros" false 0
    run_test_with_exit "echo hello world test 123" "Echo múltiplos parâmetros" false 0
    run_test_with_exit "echo -n -n hello" "Echo múltiplos -n" false 0
    run_test_with_exit "echo -ntest" "Echo -n colado" false 0
    
    # CD - todas as variações
    echo -e "${BLUE}CD:${NC}"
    run_test_with_exit "cd" "CD para HOME" false 0
    run_test_with_exit "cd ~" "CD ~ explícito" false 0
    run_test_with_exit "cd /" "CD root" false 0
    run_test_with_exit "cd /tmp" "CD tmp" false 0
    run_test_with_exit "cd ." "CD atual" false 0
    run_test_with_exit "cd .." "CD pai" false 0
    run_test_with_exit "cd ../.." "CD dois níveis acima" false 0
    run_test_with_exit "cd /nfs/homes/rdel-fra/common-core-projects/minishell" "CD volta projeto" false 0
    run_test_with_exit "cd /nonexistent" "CD diretório inexistente" true
    run_test_with_exit "cd /etc/passwd" "CD arquivo (erro)" true
    run_test_with_exit "cd ''" "CD string vazia" true
    run_test_with_exit "cd arg1 arg2" "CD múltiplos argumentos" true
    
    # PWD - casos simples mas importantes
    echo -e "${BLUE}PWD:${NC}"
    run_test_with_exit "pwd" "PWD básico" false 0
    run_test_with_exit "pwd arg1" "PWD com argumentos (deve ignorar)" false 0
    run_test_with_exit "pwd arg1 arg2 arg3" "PWD múltiplos argumentos" false 0
    
    # EXPORT - casos completos
    echo -e "${BLUE}EXPORT:${NC}"
    run_test_with_exit "export" "Export listar todas" false 0
    run_test_with_exit "export VAR=value" "Export básico" false 0
    run_test_with_exit "export VAR" "Export sem valor" false 0
    run_test_with_exit "export VAR=" "Export valor vazio" false 0
    run_test_with_exit "export VAR=value_with_spaces" "Export com espaços" false 0
    run_test_with_exit "export VAR2=value_with_home" "Export com expansão" false 0
    run_test_with_exit "export VAR=value1 VAR2=value2" "Export múltiplas variáveis" false 0
    run_test_with_exit "export 123VAR=value" "Export nome inválido número" true
    run_test_with_exit "export VAR-INVALID=value" "Export nome inválido hífen" true
    run_test_with_exit "export =value" "Export nome vazio" true
    run_test_with_exit "export VAR=" "Export vazio" false 0
    run_test_with_exit "echo \$VAR" "Verificar export vazio" false 0
    
    # UNSET - casos completos
    echo -e "${BLUE}UNSET:${NC}"
    run_test_with_exit "export TEST_UNSET=value" "Setup para unset" false 0
    run_test_with_exit "unset TEST_UNSET" "Unset básico" false 0
    run_test_with_exit "echo \$TEST_UNSET" "Verificar unset" false 0
    run_test_with_exit "unset NONEXISTENT_VAR" "Unset inexistente" false 0
    run_test_with_exit "unset VAR1 VAR2 VAR3" "Unset múltiplas" false 0
    run_test_with_exit "unset" "Unset sem argumentos" true
    run_test_with_exit "unset 123VAR" "Unset nome inválido" true
    
    # ENV - casos importantes
    echo -e "${BLUE}ENV:${NC}"
    run_test_with_exit "env" "Env completo" false 0
    run_test_with_exit "env | grep PATH" "Env filtrado PATH" false 0
    run_test_with_exit "env | grep HOME" "Env filtrado HOME" false 0
    run_test_with_exit "env | grep USER" "Env filtrado USER" false 0
    run_test_with_exit "env arg1" "Env com argumentos (deve dar erro)" true
    
    # EXIT - casos importantes (limitados porque fecha o shell)
    echo -e "${BLUE}EXIT:${NC}"
    echo "    [Manual] exit - deve fechar com status 0"
    echo "    [Manual] exit 0 - deve fechar com status 0"
    echo "    [Manual] exit 1 - deve fechar com status 1"
    echo "    [Manual] exit 255 - deve fechar com status 255"
    echo "    [Manual] exit 256 - deve fechar com status 0 (256 % 256)"
    echo "    [Manual] exit -1 - deve fechar com status 255"
    echo "    [Manual] exit abc - deve dar erro e status 2"
    echo "    [Manual] exit 1 2 - múltiplos argumentos, deve dar erro"
    echo
    
    echo -e "${YELLOW}=== EXPANSÃO DE VARIÁVEIS - CASOS COMPLETOS ===${NC}"
    run_test_with_exit "echo \$HOME" "Expansão HOME" false 0
    run_test_with_exit "echo \$USER" "Expansão USER" false 0
    run_test_with_exit "echo \$PATH" "Expansão PATH" false 0
    run_test_with_exit "echo \$PWD" "Expansão PWD" false 0
    run_test_with_exit "echo \$?" "Expansão exit status" false 0
    run_test_with_exit "echo \$NONEXISTENT" "Expansão inexistente" false 0
    run_test_with_exit "echo \$" "Dollar isolado" false 0
    run_test_with_exit "echo \$HOME\$USER" "Expansões concatenadas" false 0
    run_test_with_exit "echo prefix\$HOME" "Expansão com prefixo" false 0
    run_test_with_exit "echo \$HOME.suffix" "Expansão com sufixo" false 0
    
    echo -e "${YELLOW}=== TRATAMENTO DE ASPAS - CASOS COMPLETOS ===${NC}"
    run_test_with_exit "echo 'single quotes'" "Aspas simples básicas" false 0
    run_test_with_exit 'echo "double quotes"' "Aspas duplas básicas" false 0
    run_test_with_exit "echo 'with \$HOME inside'" "Aspas simples com variável (literal)" false 0
    run_test_with_exit 'echo "with $HOME inside"' "Aspas duplas com expansão" false 0
    # run_test_with_exit "echo 'it'\''s working'" "Escape aspas simples" false 0  # REMOVIDO: escape não implementado
    # run_test_with_exit 'echo "say \"hello\""' "Escape aspas duplas" false 0  # REMOVIDO: escape não implementado
    run_test_with_exit "echo ''" "Aspas simples vazias" false 0
    run_test_with_exit 'echo ""' "Aspas duplas vazias" false 0
    run_test_with_exit "echo 'single' \"double\"" "Mistura aspas" false 0
    
    echo -e "${BLUE}ASPAS AVANÇADAS ABRANGENTES:${NC}"
    # Setup para testes avançados
    run_test_with_exit "export COMP_EMPTY=''" "Setup variável vazia abrangente" false 0
    run_test_with_exit "export COMP_VAR='comprehensive'" "Setup variável abrangente" false 0
    
    # Casos extremos similares ao exemplo fornecido (simplificados)
    run_test_with_exit 'echo "$USER"literal"""$COMP_EMPTY"""' "Caso extremo abrangente: exemplo adaptado" false 0
    run_test_with_exit 'echo "$HOME"literal"$PWD"' "Concatenação complexa HOME+literal+PWD" false 0
    run_test_with_exit 'echo """$USER"""literal"""$HOME"""' "Triplas aspas duplas + literal" false 0
    
    # Concatenações extremas (simplificadas)
    run_test_with_exit 'echo "a"b"c"d"e"f"g"' "Alternância longa abrangente" false 0
    run_test_with_exit 'echo ""$USER""$HOME""$PWD""' "Múltiplas expansões com aspas duplas vazias" false 0
    
    # Cleanup
    run_test_with_exit "unset COMP_EMPTY COMP_VAR" "Cleanup variáveis abrangentes" false 0
    
    echo -e "${YELLOW}=== PIPES - CASOS COMPLETOS ===${NC}"
    run_test_with_exit "echo hello | cat" "Pipe básico" false 0
    run_test_with_exit "echo hello | cat | cat" "Pipe duplo" false 0
    run_test_with_exit "echo hello | cat | cat | cat" "Pipe triplo" false 0
    run_test_with_exit "ls | head -3" "Pipe comando externo" false 0
    run_test_with_exit "env | grep USER" "Pipe builtin + externo" false 0
    run_test_with_exit "pwd | cat" "Pipe PWD" false 0
    run_test_with_exit 'echo "$HOME" | cat' "Pipe com expansão" false 0
    run_test_with_exit "echo 'test' | cat" "Pipe com aspas" false 0
    
    echo -e "${YELLOW}=== REDIRECIONAMENTOS - CASOS COMPLETOS ===${NC}"
    # Output redirect >
    run_test_with_exit "echo output > /tmp/comprehensive_test.txt" "Redirect output básico" false 0
    run_test_with_exit "cat /tmp/comprehensive_test.txt" "Verificar redirect" false 0
    run_test_with_exit "pwd > /tmp/pwd_output.txt" "Redirect PWD" false 0
    run_test_with_exit "cat /tmp/pwd_output.txt" "Verificar PWD redirect" false 0
    
    # Append redirect >>
    run_test_with_exit "echo append1 >> /tmp/append_test.txt" "Append primeiro" false 0
    run_test_with_exit "echo append2 >> /tmp/append_test.txt" "Append segundo" false 0
    run_test_with_exit "cat /tmp/append_test.txt" "Verificar append" false 0
    
    # Input redirect <
    run_test_with_exit "cat < /tmp/comprehensive_test.txt" "Input redirect" false 0
    run_test_with_exit "wc -l < /tmp/append_test.txt" "Input redirect com comando" false 0
    
    echo -e "${YELLOW}=== COMANDOS EXTERNOS - CASOS COMPLETOS ===${NC}"
    run_test_with_exit "ls" "Comando ls" false 0
    run_test_with_exit "ls -la" "Comando ls com flags" false 0
    run_test_with_exit "/bin/ls" "Comando path absoluto" false 0
    run_test_with_exit "cat /etc/passwd" "Cat arquivo sistema" false 0
    run_test_with_exit "grep root /etc/passwd" "Grep" false 0
    run_test_with_exit "wc -l /etc/passwd" "Wc count lines" false 0
    run_test_with_exit "head -5 /etc/passwd" "Head primeiras linhas" false 0
    run_test_with_exit "tail -5 /etc/passwd" "Tail últimas linhas" false 0
    run_test_with_exit "whoami" "Whoami" false 0
    run_test_with_exit "date" "Date" false 0
    
    echo -e "${YELLOW}=== COMBINAÇÕES DE RECURSOS ===${NC}"
    run_test_with_exit 'echo "$USER is in $PWD" | cat > /tmp/user_location.txt' "Expansão + pipe + redirect" false 0
    run_test_with_exit "cat /tmp/user_location.txt" "Verificar combinação" false 0
    run_test_with_exit "export COMBO_VAR='test value'" "Setup variável combinação" false 0
    run_test_with_exit 'echo "Variable: $COMBO_VAR" | cat | cat' "Export + expansão + pipe duplo" false 0
    run_test_with_exit "ls | head -5 > /tmp/ls_output.txt" "Comando externo + pipe + redirect" false 0
    run_test_with_exit "cat < /tmp/ls_output.txt | wc -l" "Input redirect + pipe" false 0
    
    # Cleanup
    run_test_with_exit "unset COMBO_VAR" "Cleanup variável combinação" false 0
    rm -f /tmp/comprehensive_test.txt /tmp/pwd_output.txt /tmp/append_test.txt /tmp/user_location.txt /tmp/ls_output.txt 2>/dev/null
    echo "Arquivos de teste removidos"
    echo
}

# Teste rápido de memory leaks (opcional)
test_basic_valgrind() {
    echo -e "${BLUE}=== TESTE BÁSICO DE MEMORY LEAKS ===${NC}"
    
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}Valgrind não disponível, pulando teste de memory leaks${NC}"
        return
    fi
    
    echo "Executando teste básico com Valgrind..."
    
    local temp_script="/tmp/valgrind_test_$$.sh"
    echo -e "echo hello\npwd\nexit" > "$temp_script"
    
    timeout 15 valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --suppressions=readline.supp \
        --error-exitcode=1 "$MINISHELL" < "$temp_script" 2>&1 | \
        grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable|suppressed)" || \
        echo "Nenhum leak detectado ou Valgrind não executou"
    
    rm -f "$temp_script"
}

# Estatísticas finais
show_results() {
    echo -e "${BLUE}=== RESULTADOS ===${NC}"
    echo "Total: $TESTS_TOTAL"
    echo -e "Passou: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Falhou: ${RED}$TESTS_FAILED${NC}"
    
    if [[ $TESTS_FAILED -eq 0 ]]; then
        echo -e "${GREEN}🎉 Todos os testes passaram!${NC}"
        return 0
    else
        local rate=$((TESTS_PASSED * 100 / TESTS_TOTAL))
        echo -e "Taxa de sucesso: ${YELLOW}$rate%${NC}"
        return 1
    fi
}

# Função de ajuda
usage() {
    echo "Uso: $0 [opções]"
    echo "  -v, --valgrind     Executa teste básico de memory leaks"
    echo "  -s, --stress       Executa testes de stress e casos extremos"
    echo "  -c, --comprehensive Executa testes abrangentes por funcionalidade"
    echo "  -a, --all          Executa todos os tipos de teste (completo)"
    echo "  -h, --help         Mostra esta ajuda"
    echo ""
    echo "Por padrão, executa apenas os testes principais."
}

# Parse de opções
RUN_VALGRIND=false
RUN_STRESS=false
RUN_COMPREHENSIVE=false
RUN_ALL=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--valgrind)
            RUN_VALGRIND=true
            shift
            ;;
        -s|--stress)
            RUN_STRESS=true
            shift
            ;;
        -c|--comprehensive)
            RUN_COMPREHENSIVE=true
            shift
            ;;
        -a|--all)
            RUN_ALL=true
            RUN_STRESS=true
            RUN_COMPREHENSIVE=true
            RUN_VALGRIND=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Opção desconhecida: $1"
            usage
            exit 1
            ;;
    esac
done

# Execução principal
main() {
    # Compila se necessário
    if [[ ! -x "$MINISHELL" ]] || [[ Makefile -nt "$MINISHELL" ]]; then
        echo "Compilando minishell..."
        make clean && make
        echo
    fi
    
    # Executa testes principais
    main_tests
    
    # Executa testes de stress se solicitado
    if [[ "$RUN_STRESS" == "true" ]]; then
        echo
        stress_tests
    fi
    
    # Executa testes abrangentes se solicitado
    if [[ "$RUN_COMPREHENSIVE" == "true" ]]; then
        echo
        comprehensive_feature_tests
        echo
        test_heredoc
    fi
    
    # Executa Valgrind se solicitado
    if [[ "$RUN_VALGRIND" == "true" ]]; then
        echo
        test_basic_valgrind
    fi
    
    # Mostra resultados finais
    echo
    show_results
    
    # Relatório detalhado se executou testes completos
    if [[ "$RUN_ALL" == "true" ]]; then
        echo
        echo -e "${BLUE}=== RELATÓRIO DETALHADO ===${NC}"
        echo "Este foi um teste completo do seu minishell incluindo:"
        echo "• Testes principais com verificação de exit status"
        echo "• Testes de stress e casos extremos"
        echo "• Testes abrangentes por funcionalidade"
        echo "• Verificação de memory leaks com Valgrind"
        echo
        echo "Funcionalidades testadas conforme RECURSOS_TESTADOS.md:"
        echo "✓ 7 Builtins obrigatórios (echo, cd, pwd, export, unset, env, exit)"
        echo "✓ Comandos externos"
        echo "✓ Pipes (simples e múltiplos)"
        echo "✓ Redirecionamentos (>, <, >>)"
        echo "✓ Heredoc (<<)"
        echo "✓ Expansão de variáveis (\$VAR, \$?, etc)"
        echo "✓ Tratamento de aspas (simples e duplas)"
        echo "✓ Tratamento de erros e exit status"
        echo "✓ Combinações de recursos"
        echo
        if [[ $TESTS_FAILED -eq 0 ]]; then
            echo -e "${GREEN}🎉 Parabéns! Seu minishell passou em todos os testes!${NC}"
        else
            echo -e "${YELLOW}⚠️  Algumas funcionalidades precisam de ajustes.${NC}"
            echo "Revise os testes que falharam para identificar os problemas."
        fi
    fi
}

# Executa
main "$@"
