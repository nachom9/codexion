NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \
       algorithm.c \
       coder_actions.c \
       death_algorithm.c \
       parser.c \
       set_coders.c \
       set_dongles.c \
       state_utils.c \
       utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all