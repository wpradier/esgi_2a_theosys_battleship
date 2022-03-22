NAME = clientjeu
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

INCLUDES = includes

SRCS = srcs/main.c			\
       srcs/connect_to_battleship.c	\
       srcs/client_loop.c		\


OBJS = $(SRCS:.c=.o)



all: $(NAME)


$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS): $(INCLUDES)/battleshipClient.h

%.o: %.c
	$(CC) -o $@ -c $< -I $(INCLUDES) $(CFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
