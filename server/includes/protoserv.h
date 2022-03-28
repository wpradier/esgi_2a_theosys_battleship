#ifndef BATTLESHIPSERVER_H
# define BATTLESHIPSERVER_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <arpa/inet.h>

# define MAX_USERS 10
# define MAX_LOGIN_SIZE 30
# define MAX_PASSWORD_SIZE 30
# define MSG_SIZE 1024
# define LOGINS_SHM_KEY 100
# define PASSWORDS_SHM_KEY 101
# define QUANTITY_SHM_KEY 102

/*
 * SERVER PROTOCOL
 */

# define STOP_CONNECTION "00"
# define INFO "01"
# define GET_INPUT "02"


/*
 * END SERVER PROTOCOL
 */

typedef struct sockaddr_in s_sockaddr_in;


typedef struct Credentials {
	int		logins_id;
	int		passwords_id;
	int		quantity_id;
} s_credentials;


typedef struct  Board {
	int     shm_id;
        int     width;
        int     height;
} s_board;


int		create_socket();
int		comserv();
int		serv_send(int ns, char *proto, char *msg);
int		login_i(int i);
int		password_i(int i);
int		add_user(s_credentials credentials, char *login, char *password);
int		login_user(s_credentials credentials, char *login, char *password);
int		show_boards(s_board board);
int		strike(s_board board, int x, int y);
char		*get_board(s_board board);

#endif
