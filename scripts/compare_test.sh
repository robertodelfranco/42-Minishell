#!/bin/bash

# Script para comparar saída do minishell com bash

TEST_CMD="$1"

if [ -z "$TEST_CMD" ]; then
    echo "Uso: $0 'comando para testar'"
    exit 1
fi

echo "=== Comando: $TEST_CMD ==="
echo
echo "--- Saída do bash ---"
bash -c "$TEST_CMD"
echo "Exit code bash: $?"
echo
echo "--- Saída do minishell ---"
echo "$TEST_CMD" | ./minishell
echo "Exit code minishell: $?"
