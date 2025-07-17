make[1]: Entering directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
make[1]: Leaving directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
make[1]: Entering directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
make[1]: Leaving directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
make[1]: Entering directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
make[1]: Leaving directory '/nfs/homes/rdel-fra/common-core-projects/minishell/lib'
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/main.o -c ./src/main.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/read.o -c ./src/read.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/utils.o -c ./src/utils.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/clean.o -c ./src/clean.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/free_list.o -c ./src/free_list.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/token.o -c ./src/token/token.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/token_utils.o -c ./src/token/token_utils.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/token_handle_quotes.o -c ./src/token/token_handle_quotes.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/executor.o -c ./src/executor/executor.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/exec_cmd.o -c ./src/executor/exec_cmd.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/exec_pipes.o -c ./src/executor/exec_pipes.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/exec_utils.o -c ./src/executor/exec_utils.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/exec_child.o -c ./src/executor/exec_child.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse.o -c ./src/parse/parse.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse_list.o -c ./src/parse/parse_list.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse_stack.o -c ./src/parse/parse_stack.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse_list_redirs.o -c ./src/parse/parse_list_redirs.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse_types.o -c ./src/parse/parse_types.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/parse_quotes.o -c ./src/parse/parse_quotes.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/echo.o -c ./src/builtin/echo.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/unset.o -c ./src/builtin/unset.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/pwd.o -c ./src/builtin/pwd.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/env.o -c ./src/builtin/env.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/exit.o -c ./src/builtin/exit.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/cd.o -c ./src/builtin/cd.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/export.o -c ./src/builtin/export.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/export_sort.o -c ./src/builtin/export_sort.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/init_env.o -c ./src/expansion/init_env.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/expand.o -c ./src/expansion/expand.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/expand_utils.o -c ./src/expansion/expand_utils.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/expand_quotes.o -c ./src/expansion/expand_quotes.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/expand_quotes_utils.o -c ./src/expansion/expand_quotes_utils.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/redirs.o -c ./src/redirs/redirs.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/heredoc.o -c ./src/redirs/heredoc.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/signal.o -c ./src/signals/signal.c
cc -Wall -Werror -Wextra -I lib/includes -I ./includes -o objs/signal_utils.o -c ./src/signals/signal_utils.c
