NAME		=	minishell

CC			=	cc

HEADERS		=	-I $(LIBFT)/includes -I ./includes

FLAGS		=	-Wall -Werror -Wextra ${HEADERS}

LIBFT		=	lib

LIB			=	$(LIBFT)/libft.a

SRCS		=	./src/main.c \
				./src/read.c \
				./src/clean.c \
				./src/free_list.c \
				./src/token/token.c \
				./src/token/token_utils.c \
				./src/token/token_handle_quotes.c \
				./src/executor/executor.c \
				./src/executor/exec_cmd.c \
				./src/executor/exec_pipes.c \
				./src/executor/exec_utils.c \
				./src/parse/parse.c \
				./src/parse/parse_list.c \
				./src/parse/parse_stack.c \
				./src/parse/parse_list_redirs.c \
				./src/parse/parse_types.c \
				./src/parse/parse_quotes.c \
				./src/builtin/echo.c \
				./src/builtin/unset.c \
				./src/builtin/pwd.c \
				./src/builtin/env.c \
				./src/builtin/exit.c \
				./src/builtin/cd.c \
				./src/builtin/export.c \
				./src/builtin/export_sort.c \
				./src/expansion/init_env.c \
				./src/expansion/expand.c \
				./src/expansion/expand_utils.c \
				./src/expansion/expand_quotes.c \
				./src/redirs/redirs.c \
				./src/redirs/heredoc.c \
				./src/signals/signal.c

OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJ_DIR		=	objs
VPATH		=	./src ./src/builtin ./src/token ./src/parse ./src/executor ./src/expansion ./src/pipes ./src/redirs ./src/signals

all: libft ${NAME}

libft:
	@make -C $(LIBFT)

${NAME}: ${OBJS}
	@$(CC) $(OBJS) $(LIB) $(HEADERS) -o $(NAME) -lreadline

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	${CC} ${FLAGS} -o $@ -c $<

clean:
	@rm -rf ${OBJS}
	@$(MAKE) -C $(LIBFT) clean

fclean:	clean
	@rm -rf ${NAME}
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell

.PHONY: all clean fclean re libft val