#ifndef BATTLESHIPSERVER_H
# define BATTLESHIPSERVER_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>

typedef struct sockaddr_in s_sockaddr_in;

typedef struct	User {
	char		*login;
	char		*password;
	int		isAdmin; // 0 = user, 1 = admin
} s_user;

typedef struct		AllUsers {
	int		quantity;
	struct User	*users;
} s_allusers

#endif
