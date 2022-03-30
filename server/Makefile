NAME = servjeu
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

INCLUDES = includes

SRCS = srcs/main.c				\
       srcs/comserv.c				\
       srcs/gestpart.c				\
       srcs/create_socket.c			\
       srcs/serv_send.c				\
       srcs/game/strike.c			\
       srcs/game/get_board.c			\
       srcs/users_funcs/init_users.c		\
       srcs/users_funcs/add_user.c		\
       srcs/users_funcs/free_users.c		\
       srcs/users_funcs/login_user.c		\
       srcs/menu_funcs/admin_phase.c		\
       srcs/menu_funcs/player_menu.c		\
       srcs/menu_funcs/admin_menu.c		\
       srcs/menu_funcs/initial_admin_menu.c	\
       srcs/menu_funcs/admin_connection_menu.c	\

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
