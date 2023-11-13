NAME		=	woody_woodpacker

C_SRCS		= 	\
				get_key.c \
				main.c \
				libft.c \
				random.c \
				$(addprefix injection/, \
					inject.c \
					code_cave.c \
					elf_parser.c \
					elf_shifting.c) \
				$(addprefix encryption/, \
					encrypt.c)

ASM_SRCS	=	\
				xor_cypher.s

TEST_SRC	=	test/basic_exec.c
TEST_NAME	=	test/basic_exec

_OBJS		=	${C_SRCS:.c=.o} ${ASM_SRCS:.s=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CC			=	clang
CFLAGS		=   -Wall -Wextra -Werror -g3
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

payload:
		cd payload && sh convert_payload.sh


.PHONY	:	all clean fclean re test payload
