# Correção: Exit Status em Modo Não Interativo

## 🔧 Problema Identificado

O minishell estava sempre saindo com código 0 em modo não interativo, independente do `exit_status` do último comando executado.

## ✅ Solução Implementada

### Arquivo: `src/read.c`

**Antes:**
```c
if (!data->prompt)
{
    if (data->interactive_mode)
        ft_putendl_fd("exit", STDOUT_FILENO);
    free_program(data, NULL);
    exit(0);  // ❌ Sempre saía com 0
}
```

**Depois:**
```c
if (!data->prompt)
{
    if (data->interactive_mode)
        ft_putendl_fd("exit", STDOUT_FILENO);
    free_program(data, NULL);
    if (data->interactive_mode)
        exit(0);
    else
        exit(data->exit_status);  // ✅ Preserva exit_status
}
```

## 🧪 Resultados dos Testes

### ✅ Comportamento Correto Agora:

```bash
# Comando bem-sucedido
echo "pwd" | ./minishell
# Exit code: 0 ✓

# Comando inexistente  
echo "comando_inexistente" | ./minishell
# Exit code: 127 ✓

# Exit explícito
echo "exit 42" | ./minishell  
# Exit code: 42 ✓
```

### 📊 Batch Test Results:
- **Antes**: 13/16 passando
- **Depois**: 14/16 passando
- **Melhoria**: +1 teste passando (comando inexistente)

## 🎯 Comportamento Final

### Modo Interativo:
- Sempre sai com código 0 ao detectar EOF
- Imprime "exit" na saída
- Comportamento de terminal tradicional

### Modo Não Interativo:
- Sai com `data->exit_status` do último comando
- Não imprime "exit"
- Compatível com scripts e automação

## ✅ Validação

```bash
# Teste rápido
echo "comando_inexistente" | ./minishell 2>/dev/null; echo $?
# Resultado: 127 ✓

# Comparação com bash
echo "comando_inexistente" | bash 2>/dev/null; echo $?  
# Resultado: 127 ✓
```

**Status: ✅ CORRIGIDO E FUNCIONANDO**
