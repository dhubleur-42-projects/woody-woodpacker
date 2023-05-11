NAME		=	woody_woodpacker

C_SRCS		= 	main.c \
				random.c \
				elf_parser.c \
				writter.c

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