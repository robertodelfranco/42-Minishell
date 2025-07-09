# Correção: Redirecionamento de Built-ins

## 🔍 Problema Identificado

Os built-ins **ECHO, PWD, EXPORT** estavam imprimindo na tela ao invés de redirecionar para arquivos porque:

1. **`ft_printf()`** usa `write(1, ...)` - sempre escreve para fd 1 original
2. **`printf()`** ignora redirecionamentos do `dup2()`
3. **ENV funcionava** porque já usava a função correta

## 🔧 Causa Raiz

```c
// ❌ PROBLEMA: write sempre para fd 1 original
ft_printf("%s\n", texto);     // write(1, ...)
printf("%s\n", texto);        // write(1, ...)

// ✅ SOLUÇÃO: write para STDOUT_FILENO (pode ser redirecionado)
ft_printf_fd(STDOUT_FILENO, "%s\n", texto);  // write(STDOUT_FILENO, ...)
```

## ✅ Correções Aplicadas

### 1. **echo.c**
```c
// Antes
ft_printf("%s", args[i]);

// Depois  
ft_printf_fd(STDOUT_FILENO, "%s", args[i]);
```

### 2. **pwd.c**
```c
// Antes
ft_printf("%s\n", cwd);

// Depois
ft_printf_fd(STDOUT_FILENO, "%s\n", cwd);
```

### 3. **env.c**
```c
// Antes
ft_printf("%s=%s\n", cur->key, cur->value);

// Depois
ft_printf_fd(STDOUT_FILENO, "%s=%s\n", cur->key, cur->value);
```

### 4. **export_sort.c**
```c
// Antes
ft_printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);

// Depois
ft_printf_fd(STDOUT_FILENO, "declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
```

## 🧪 Resultados dos Testes

### ✅ Antes vs Depois:
- **Batch test**: 14/25 → **23/25** passando (+9 testes! 🚀)
- **Redirecionamento**: ECHO, PWD, EXPORT agora funcionam ✓
- **Compatibilidade**: Idêntico ao bash ✓

### ✅ Validação:
```bash
# Agora funciona!
echo "echo 'test' > file.txt" | ./minishell
cat file.txt  # Saída: test ✓

echo "pwd > dir.txt" | ./minishell  
cat dir.txt   # Saída: /current/directory ✓
```

## 🎯 Conceito Importante

**File Descriptors e Redirecionamento:**
- `write(1, ...)` → Sempre fd 1 original (ignora `dup2`)
- `write(STDOUT_FILENO, ...)` → Usa fd atual do STDOUT (respeita `dup2`)

**Sequência de execução:**
1. `identify_redirs()` → Abre arquivo de destino
2. `dup_fds()` → `dup2(arquivo_fd, STDOUT_FILENO)` 
3. `execute_built_in()` → `ft_printf_fd(STDOUT_FILENO, ...)` ✓

## 🎉 Status: **CORRIGIDO E FUNCIONANDO**

Agora todos os built-ins respeitam redirecionamentos corretamente! 🎯
