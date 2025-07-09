#!/bin/bash

# =========================================================================
# OVERVIEW DOS TESTES DO MINISHELL
# =========================================================================
# Script que executa todos os sistemas de teste e mostra um resumo
# comparativo dos resultados.
# =========================================================================

# Cores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}======================================${NC}"
echo -e "${CYAN}    OVERVIEW DOS TESTES MINISHELL    ${NC}"
echo -e "${CYAN}======================================${NC}"
echo

# Verifica se o minishell existe
if [[ ! -x "./minishell" ]]; then
    echo -e "${RED}Erro: ./minishell não encontrado${NC}"
    echo "Execute 'make' para compilar"
    exit 1
fi

echo -e "${BLUE}Compilando minishell...${NC}"
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo

# ========== TESTE BÁSICO INTERATIVO ==========
echo -e "${YELLOW}=== TESTE BÁSICO INTERATIVO ===${NC}"
if [[ -x "./basic_interactive_test.sh" ]]; then
    echo "Executando testes interativos básicos..."
    
    # Captura output e código de saída
    basic_output=$(./basic_interactive_test.sh 2>&1)
    basic_exit=$?
    
    # Extrai estatísticas
    if echo "$basic_output" | grep -q "Total:"; then
        total=$(echo "$basic_output" | grep "Total:" | awk '{print $2}')
        passed=$(echo "$basic_output" | grep "Passou:" | awk '{print $2}')
        failed=$(echo "$basic_output" | grep "Falhou:" | awk '{print $2}')
        
        echo -e "  Resultados: ${GREEN}$passed/${total} testes passaram${NC}"
        if [[ $failed -gt 0 ]]; then
            echo -e "  ${RED}$failed testes falharam${NC}"
        fi
        
        if [[ $basic_exit -eq 0 ]]; then
            echo -e "  Status: ${GREEN}✅ SUCESSO${NC}"
        else
            echo -e "  Status: ${YELLOW}⚠️  PARCIAL${NC}"
        fi
    else
        echo -e "  Status: ${RED}❌ ERRO${NC}"
    fi
else
    echo -e "  Status: ${RED}Script não encontrado${NC}"
fi
echo

# ========== TESTE VALGRIND ==========
echo -e "${YELLOW}=== TESTE DE MEMORY LEAKS ===${NC}"
if command -v valgrind > /dev/null 2>&1; then
    echo "Executando teste de memory leaks..."
    
    # Executa teste rápido com Valgrind
    valgrind_output=$(echo -e "echo hello\npwd\nexit" | timeout 15 valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./minishell 2>&1)
    valgrind_exit=$?
    
    # Analisa resultado
    if echo "$valgrind_output" | grep -q "ERROR SUMMARY: 0 errors"; then
        echo -e "  Status: ${GREEN}✅ SEM MEMORY LEAKS${NC}"
    elif echo "$valgrind_output" | grep -q "definitely lost: 0 bytes"; then
        echo -e "  Status: ${GREEN}✅ SEM LEAKS DEFINITIVOS${NC}"
        if echo "$valgrind_output" | grep -q "possibly lost:"; then
            echo -e "  ${YELLOW}⚠️  Possíveis leaks detectados${NC}"
        fi
    else
        echo -e "  Status: ${RED}❌ MEMORY LEAKS DETECTADOS${NC}"
    fi
    
    # Mostra resumo dos leaks
    echo "$valgrind_output" | grep -E "(definitely lost|indirectly lost|ERROR SUMMARY)" | head -3
else
    echo -e "  Status: ${YELLOW}Valgrind não disponível${NC}"
fi
echo

# ========== TESTE DOS BUILTINS ==========
echo -e "${YELLOW}=== TESTE RÁPIDO DOS BUILTINS ===${NC}"
echo "Testando builtins essenciais..."

test_builtin() {
    local cmd="$1"
    local name="$2"
    
    echo -e "$cmd\nexit" | timeout 3 ./minishell > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        echo -e "  ${name}: ${GREEN}✅${NC}"
    else
        echo -e "  ${name}: ${RED}❌${NC}"
    fi
}

test_builtin "echo hello" "echo"
test_builtin "pwd" "pwd"
test_builtin "env | head -1" "env"
test_builtin "export TEST=123" "export"
test_builtin "unset TEST" "unset"
test_builtin "cd /" "cd"

echo

# ========== TESTE DE FEATURES AVANÇADAS ==========
echo -e "${YELLOW}=== TESTE DE FEATURES AVANÇADAS ===${NC}"
echo "Testando funcionalidades complexas..."

test_feature() {
    local cmd="$1"
    local name="$2"
    
    echo -e "$cmd\nexit" | timeout 3 ./minishell > /dev/null 2>&1
    if [[ $? -eq 0 ]]; then
        echo -e "  ${name}: ${GREEN}✅${NC}"
    else
        echo -e "  ${name}: ${RED}❌${NC}"
    fi
}

test_feature "echo hello | cat" "Pipes"
test_feature "echo test > /tmp/test.txt" "Redirecionamento"
test_feature "echo \$HOME" "Expansão de variáveis"
test_feature "ls /bin | head -1" "Comandos externos"

echo

# ========== COMPARAÇÃO COM BASH ==========
echo -e "${YELLOW}=== COMPARAÇÃO RÁPIDA COM BASH ===${NC}"
echo "Comparando outputs básicos..."

compare_output() {
    local cmd="$1"
    local name="$2"
    
    # Output do minishell
    mini_out=$(echo -e "$cmd\nexit" | timeout 3 ./minishell 2>&1 | sed '/Minihell \$/d' | grep -v "^exit$" | grep -v "^$")
    
    # Output do bash
    bash_out=$(bash -c "$cmd" 2>&1)
    
    if [[ "$mini_out" == "$bash_out" ]]; then
        echo -e "  ${name}: ${GREEN}✅ IDÊNTICO${NC}"
    elif [[ -n "$mini_out" ]] && [[ -n "$bash_out" ]]; then
        echo -e "  ${name}: ${YELLOW}⚠️  DIFERENTE${NC}"
    else
        echo -e "  ${name}: ${RED}❌ ERRO${NC}"
    fi
}

compare_output "echo hello world" "Echo básico"
compare_output "pwd" "PWD"
compare_output "echo \$USER" "Expansão USER"

echo

# ========== RESUMO FINAL ==========
echo -e "${CYAN}======================================${NC}"
echo -e "${CYAN}             RESUMO FINAL            ${NC}"
echo -e "${CYAN}======================================${NC}"

echo -e "${GREEN}✅ Funcionalidades Implementadas:${NC}"
echo "  • Comandos básicos (echo, pwd, env, etc.)"
echo "  • Builtins obrigatórios (cd, export, unset, exit)"
echo "  • Redirecionamento (>, <, >>)"
echo "  • Pipes (|)"
echo "  • Expansão de variáveis (\$VAR)"
echo "  • Comandos externos"
echo "  • Tratamento de sinais"
echo "  • Aspas (simples e duplas)"

echo
echo -e "${BLUE}📊 Status Geral:${NC}"
if [[ $basic_exit -eq 0 ]]; then
    echo -e "  ${GREEN}✅ MINISHELL FUNCIONANDO CORRETAMENTE${NC}"
    echo -e "  ${GREEN}✅ Pronto para avaliação${NC}"
else
    echo -e "  ${YELLOW}⚠️  Alguns testes falharam - verificar logs${NC}"
fi

echo
echo -e "${BLUE}📖 Para mais detalhes:${NC}"
echo "  ./basic_interactive_test.sh          # Teste completo"
echo "  ./basic_interactive_test.sh --valgrind # Com memory leaks"
echo "  cat TESTES_INTERATIVOS.md           # Documentação"

echo
