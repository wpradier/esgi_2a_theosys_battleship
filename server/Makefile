NAME = servjeu
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

INCLUDES = includes

SRCS = srcs/main.c			\
       srcs/comserv.c			\
       srcs/create_socket.c		\
       srcs/serv_send.c			\
       srcs/users_funcs/add_user.c	\
       srcs/users_funcs/create_user.c	\
       srcs/users_funcs/init_allusers.c	\
       srcs/users_funcs/login_user.c	\
       srcs/users_funcs/cred_i.c	\

OBJS = $(SRCS:.c=.o)


all: $(NAME)


$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS): $(INCLUDES)/protoserv.h

%.o: %.c
	$(CC) -o $@ -c $< -I $(INCLUDES) $(CFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
