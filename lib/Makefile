NAME		=	libft.a

CC			=	 cc

FLAGS		=	-Wall -Werror -Wextra

SRCS		=	./libft/ft_isalpha.c \
				./libft/ft_isdigit.c \
				./libft/ft_isalnum.c \
				./libft/ft_isascii.c \
				./libft/ft_isprint.c \
				./libft/ft_strchr.c \
				./libft/ft_strlen.c \
				./libft/ft_strcmp.c \
				./libft/ft_strncmp.c \
				./libft/ft_strrchr.c \
				./libft/ft_findchar.c \
				./libft/ft_strchr_count.c \
				./libft/ft_toupper.c \
				./libft/ft_tolower.c \
				./libft/ft_strnstr.c \
				./libft/ft_bzero.c \
				./libft/ft_strdup.c \
				./libft/ft_calloc.c \
				./libft/ft_atoi.c \
				./libft/ft_atol.c \
				./libft/ft_strlcpy.c \
				./libft/ft_strlcat.c \
				./libft/ft_memset.c \
				./libft/ft_memcpy.c \
				./libft/ft_memmove.c \
				./libft/ft_memchr.c \
				./libft/ft_memcmp.c \
				./libft/ft_substr.c \
				./libft/ft_strjoin.c \
				./libft/ft_strjoin_free.c \
				./libft/ft_strjoin_three.c \
				./libft/ft_strtrim.c \
				./libft/ft_itoa.c \
				./libft/ft_split.c \
				./libft/ft_strmapi.c \
				./libft/ft_striteri.c \
				./libft/ft_putchar_fd.c \
				./libft/ft_printf_fd.c \
				./libft/ft_putstr_fd.c \
				./libft/ft_putendl_fd.c \
				./libft/ft_putnbr_fd.c \
				./printf/ft_printf.c \
				./libft/ft_ptrlen.c \
				./printf/ft_utils.c \
				./printf/ft_lib.c \
				./printf/ft_types.c \
				./printf/ft_flags.c \
				./gnl/get_next_line_bonus.c \
				./libft/ft_lstnew_bonus.c \
				./libft/ft_lstadd_front_bonus.c \
				./libft/ft_lstsize_bonus.c \
				./libft/ft_lstlast_bonus.c \
				./libft/ft_lstadd_back_bonus.c \
				./libft/ft_lstdelone_bonus.c \
				./libft/ft_lstclear_bonus.c \
				./libft/ft_lstiter_bonus.c \
				./libft/ft_lstmap_bonus.c \
				./libft/ft_lstsecond_to_last.c

HEADERS		=	./includes

CYAN		=	\033[1;96m
NC			=	\033[0m

AR			=	ar rcs

RM			=	rm -rf

OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJ_DIR		=	objs
VPATH		=	./libft:./gnl:./printf

all: ${NAME}

${NAME}: ${OBJS}
	@echo "${CYAN}Libft compiled successfully!${NC}"

$(OBJ_DIR)/%.o:%.c ${HEADERS}
	@mkdir -p $(OBJ_DIR)
	@${CC} ${FLAGS} -c $< -o $@
	@${AR} ${NAME} $@

clean:
	@${RM} ${OBJS}

fclean:	clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re