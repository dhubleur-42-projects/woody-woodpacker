NAME		=	woody_woodpacker

C_SRCS		= 	\
				main.c \
				random.c \
				key_generator.c \
				$(addprefix parser/, \
					parser.c \
					parse_options.c \
					elf64_parser.c) \
				$(addprefix injection/, \
					code_cave.c \
					prepare_injection_elf64.c \
					prepare_injection.c \
					inject.c)

ASM_SRCS	=	\
				xor_cipher.s

TEST_SRC	=	test/basic_exec.c
TEST_NAME	=	test/basic_exec

_OBJS		=	${C_SRCS:.c=.o} ${ASM_SRCS:.s=.o}
OBJS		=	$(addprefix build/, $(_OBJS))
OBJS_DEPEND	=	${OBJS:.o=.d}

CC			=	clang
CFLAGS		=   -Wall -Wextra -Werror -g3
INCLUDE		=	-I includes/ -I libft

ASMC		= nasm
ASMFLAGS	= -felf64

LIBFT		=	libft/libft.a

all		:	$(NAME)

$(LIBFT): FORCE
	make -C libft

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

$(NAME)	:	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

-include $(OBJS_DEPEND)

test	:	all $(TEST_NAME)
	./$(NAME) -v $(TEST_NAME)

$(TEST_NAME)	:	$(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_NAME)

clean	:	
	rm -Rf build/ $(TEST_NAME)


fclean	:	clean
	rm -f ${NAME}

fcleanlib: fclean
	make -C libft fclean

re		:	fclean
			make ${NAME}

relib	:	fcleanlib
			make ${NAME}

payload:
		cd payload && sh convert_payload.sh

FORCE:

.PHONY	:	all clean fclean re test payload FORCE fcleanlib relib
