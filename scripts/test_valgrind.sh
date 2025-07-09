#!/bin/bash

# Script específico para testes com Valgrind

MINISHELL="./minishell"

# Cores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
PURPLE='\033[0;35m'
NC='\033[0m'

if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}Erro: Valgrind não encontrado. Instale com: sudo apt install valgrind${NC}"
    exit 1
fi

echo -e "${PURPLE}=== TESTE DE MEMORY LEAKS COM VALGRIND ===${NC}"
echo

test_with_valgrind() {
    local cmd="$1"
    local description="$2"
    
    echo -e "${YELLOW}Testando: $description${NC}"
    echo "Comando: $cmd"
    
    # Executar com valgrind
    echo "$cmd" | valgrind --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --quiet \
        --error-exitcode=42 \
        $MINISHELL > /dev/null 2> valgrind_output.tmp
    
    local exit_code=$?
    
    if [ $exit_code -eq 42 ]; then
        echo -e "${RED}❌ MEMORY LEAK DETECTADO${NC}"
        echo "Detalhes do Valgrind:"
        cat valgrind_output.tmp
    else
        echo -e "${GREEN}✅ SEM MEMORY LEAKS${NC}"
    fi
    
    echo "---"
    rm -f valgrind_output.tmp
}

# Testes básicos
test_with_valgrind "echo hello" "Echo simples"
test_with_valgrind "pwd" "PWD"
test_with_valgrind "env | head -3" "ENV com pipe"
test_with_valgrind "echo test > /tmp/test_valgrind.txt" "Redirecionamento"
test_with_valgrind "export TEST=123" "Export"
test_with_valgrind "comando_inexistente" "Comando inexistente"
test_with_valgrind "exit 0" "Exit"

# Cleanup
rm -f /tmp/test_valgrind.txt

echo -e "${PURPLE}=== TESTE VALGRIND CONCLUÍDO ===${NC}"
