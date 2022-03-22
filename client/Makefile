NAME = clientjeu
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

INCLUDES = includes

SRCS = 	srcs/main.c		\
	srcs/shm/read.c		\
	srcs/shm/write.c	\
	srcs/shm/create.c	\
	srcs/srv/connect.c	\
	srcs/srv/listenner.c	\
	srcs/srv/sender.c	\


OBJS = $(SRCS:.c=.o)



all: $(NAME)


$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< -I $(INCLUDES) $(CFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
