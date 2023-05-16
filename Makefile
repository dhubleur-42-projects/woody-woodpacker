NAME		=	woody_woodpacker

C_SRCS		= 	\
				elf_parser.c \
				get_key.c \
				main.c \
				libft.c \
				random.c \
				encryption/cipher.c \
				encryption/do_a_round.c \
				encryption/ft_memcpy.c \
				encryption/generate_keys.c \
				encryption/mix_columns.c \
				encryption/reverse_round.c \
				encryption/shift_rows.c \
				encryption/sub_bytes.c \
				encryption/uncipher.c \
				encryption/xor_bytes.c

ASM_SRCS	=	

TEST_SRC	=	test/basic_exec.c
TEST_NAME	=	test/basic_exec

_OBJS		=	${C_SRCS:.c=.o} ${ASM_SRCS:.s=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CC			=	clang
CFLAGS		=   -Wall -Wextra -Werror
INCLUDE		=	-I includes/

ASMC		= nasm
ASMFLAGS	= -felf64

all		:	$(NAME)

build/%.o	:	srcs/C/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) ${CFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDE} -c $< -o $@

build/%.o	:	srcs/ASM/%.s
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(ASMC) $(ASMFLAGS) $< -o $@

$(NAME)	:	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

-include $(OBJS_DEPEND)

test	:	all $(TEST_NAME)
	./$(NAME) $(TEST_NAME)

$(TEST_NAME)	:	$(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_NAME)

clean	:	
	rm -Rf build/ $(TEST_NAME)


fclean	:	clean
	rm -f ${NAME}

re		:	fclean ${NAME}


.PHONY	:	all clean fclean re test
