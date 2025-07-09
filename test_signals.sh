#!/bin/bash

# Cores para melhor visualização
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Caminhos
MINISHELL="./minishell"
BASH="/bin/bash"

# Função para imprimir seções
print_section() {
    echo -e "\n${YELLOW}===== $1 =====${NC}"
}

# Função para imprimir informações
print_info() {
    echo -e "${BLUE}INFO:${NC} $1"
}

# Função para imprimir comandos
print_cmd() {
    echo -e "${GREEN}Executando:${NC} $1"
}

# Função para imprimir instruções de teste
print_test() {
    echo -e "\n${BOLD}${GREEN}$1${NC}"
    echo -e "${BLUE}Instruções:${NC} $2"
    echo -e "${YELLOW}Comportamento esperado:${NC} $3"
    echo -e "${RED}Pressione ENTER para continuar...${NC}"
    read
}

# Compilar o minishell
print_section "Compilando o minishell"
make

# Verifica se o minishell foi compilado com sucesso
if [ ! -x "$MINISHELL" ]; then
    echo -e "${RED}Erro: O minishell não foi compilado corretamente.${NC}"
    exit 1
fi

print_section "Lista de verificação manual adicional"
print_info "Além dos testes interativos, verifique também:"

echo -e "\n${GREEN}1. Teste de Ctrl+D no prompt vazio:${NC}"
echo "   Pressione Ctrl+D no prompt vazio. O shell deve exibir uma mensagem de saída e encerrar."

echo -e "\n${GREEN}2. Teste de heredoc com delimitador normal:${NC}"
echo "   Execute: cat << EOF"
echo "   Digite algumas linhas e termine com 'EOF'. Verifique se o conteúdo é exibido corretamente."

echo -e "\n${GREEN}3. Teste de comando em background:${NC}"
echo "   Execute: sleep 10 &"
echo "   Pressione Enter algumas vezes e verifique se o prompt continua funcionando."

echo -e "\n${GREEN}4. Teste de sinais com processos filhos aninhados:${NC}"
echo "   Execute comandos complexos como: (cat | grep a | wc -l)"
echo "   Pressione Ctrl+C e verifique se todos os processos são terminados corretamente."

echo -e "\n${GREEN}5. Teste de sinais após expansão de variáveis:${NC}"
echo "   Execute: echo \"$HOME\" (sem as aspas duplas externas)"
echo "   Pressione Ctrl+C durante ou após a expansão e verifique o comportamento."

print_section "Fim dos testes"
print_info "Todos os testes foram concluídos. Revise os resultados acima."

print_section "Testes interativos de sinais"
print_info "Você será guiado através de vários testes para verificar o comportamento dos sinais no seu minishell."
print_info "Siga as instruções na tela e observe o comportamento esperado."

# TESTE 1: Ctrl+C no prompt
print_test "1. Teste básico de Ctrl+C no prompt" \
           "Inicie o minishell, espere o prompt aparecer e pressione Ctrl+C." \
           "O prompt deve ser limpo, uma nova linha deve aparecer com um novo prompt."

print_cmd "Executando: $MINISHELL"
echo "Pressione Ctrl+C quando ver o prompt."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 2: Ctrl+\ no prompt
print_test "2. Teste de Ctrl+\ no prompt" \
           "Inicie o minishell, espere o prompt aparecer e pressione Ctrl+\\ (SIGQUIT)." \
           "O sinal deve ser ignorado (nenhuma mudança visível)."

print_cmd "Executando: $MINISHELL"
echo "Pressione Ctrl+\\ quando ver o prompt."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 3: Ctrl+C durante comando interativo
print_test "3. Teste de Ctrl+C durante comando interativo" \
           "Inicie o minishell, execute 'cat' sem argumentos, digite algumas linhas e pressione Ctrl+C." \
           "O comando 'cat' deve ser interrompido e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'cat' e pressione Enter."
echo "Digite algumas linhas de texto."
echo "Pressione Ctrl+C para interromper o comando."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 4: Ctrl+C durante heredoc
print_test "4. Teste de heredoc com Ctrl+C" \
           "Inicie o minishell, execute 'cat << EOF', digite algumas linhas e pressione Ctrl+C." \
           "O heredoc deve ser interrompido, e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'cat << EOF' e pressione Enter."
echo "Digite algumas linhas de texto."
echo "Pressione Ctrl+C para interromper o heredoc."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 5: Ctrl+D durante heredoc
print_test "5. Teste de heredoc com Ctrl+D" \
           "Inicie o minishell, execute 'cat << EOF', digite algumas linhas e pressione Ctrl+D." \
           "Deve aparecer uma mensagem indicando que o heredoc foi delimitado por EOF."

print_cmd "Executando: $MINISHELL"
echo "Digite 'cat << EOF' e pressione Enter."
echo "Digite algumas linhas de texto."
echo "Pressione Ctrl+D para simular EOF no heredoc."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 6: Múltiplos Ctrl+C consecutivos
print_test "6. Teste de múltiplos Ctrl+C consecutivos" \
           "Inicie o minishell e pressione Ctrl+C várias vezes em sequência rápida." \
           "O shell deve permanecer estável, mostrando um novo prompt após cada Ctrl+C."

print_cmd "Executando: $MINISHELL"
echo "Pressione Ctrl+C várias vezes seguidas."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 7: Ctrl+C durante comando com pipe
print_test "7. Teste de pipe com Ctrl+C" \
           "Inicie o minishell, execute 'cat | grep test' e pressione Ctrl+C." \
           "Os processos devem ser interrompidos e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'cat | grep test' e pressione Enter."
echo "Pressione Ctrl+C para interromper os processos."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 8: Ctrl+C durante sleep
print_test "8. Teste de Ctrl+C durante sleep" \
           "Inicie o minishell, execute 'sleep 5' e pressione Ctrl+C antes que termine." \
           "O comando deve ser interrompido e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'sleep 5' e pressione Enter."
echo "Pressione Ctrl+C antes que o sleep termine."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 9: Ctrl+\ durante sleep
print_test "9. Teste de Ctrl+\ durante sleep" \
           "Inicie o minishell, execute 'sleep 5' e pressione Ctrl+\\ antes que termine." \
           "Deve aparecer a mensagem 'Quit (core dumped)' e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'sleep 5' e pressione Enter."
echo "Pressione Ctrl+\\ antes que o sleep termine."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL

# TESTE 10: Ctrl+C após múltiplos heredocs
print_test "10. Teste de Ctrl+C após múltiplos heredocs" \
           "Inicie o minishell, execute 'cat << EOF1 && cat << EOF2', complete o primeiro heredoc digitando 'EOF1' e interrompa o segundo com Ctrl+C." \
           "O heredoc deve ser interrompido e o prompt deve retornar."

print_cmd "Executando: $MINISHELL"
echo "Digite 'cat << EOF1 && cat << EOF2' e pressione Enter."
echo "Digite algumas linhas e depois 'EOF1' para completar o primeiro heredoc."
echo "Digite algumas linhas para o segundo heredoc e pressione Ctrl+C."
echo "Depois, digite 'exit' para sair do minishell."
$MINISHELL
