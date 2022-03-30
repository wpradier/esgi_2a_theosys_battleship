#ifndef PROTOSERV_H
# define PROTOSERV_H

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
# define MSG_SIZE 2048
# define BOARD_SHM_KEY 101

/*
 * SERVER PROTOCOL
 */

# define STOP_CONNECTION "00"
# define INFO "01"
# define GET_INPUT "02"


/*
 * END SERVER PROTOCOL
 */

/*
 * PIPE COMS PROTOCOL
 */


// connexion
# define END_CONN "00"
# define CONN "01"
# define VALID "02"
# define INVALID "03"

// admin
# define END_PHASE "00"
# define BOARD_SIZE "01"
# define CREDENTIALS "02"
# define BOAT_COORDS "03"

// players
# define END_GAME "00"
# define START_GAME "01"
# define DISPLAY "02"
# define PLAY "03"

/*
 * END PIPE COMS PROTOCOL
 */

# define P_READ 0
# define P_WRITE 1

# define FROMSERV 0
# define TOSERV 1

typedef struct sockaddr_in s_sockaddr_in;

typedef struct Users {
	char	**logins;
	char	**passwords;
	size_t	quantity;
} s_users;

typedef struct Board {
	int	shm_id;
	int	width;
	int	height;
	int	len;
} s_board;


int		create_socket();
int		comserv();
int		gestpart(s_board board, int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users);
int		serv_send(int ns, char *proto, char *msg);
int		strike(s_board board, int x, int y);
char		*get_board(s_board board, int admin);
s_users		*init_users();
int		add_user(s_users *users, char *login, char *password);
void		free_users(s_users *users);
int		login_user(s_users *users, char *login, char *password);
s_board		admin_phase(int admin_fd, s_users *users);
int		initial_admin_menu(int admin_ns, int serv_fd);
int		player_menu(int ns, int pipes[2][2], s_board board);
int		admin_menu(int ns, int pipes[2][2]);
int		admin_connection_menu(int ns, int ad_pipes[2][2]);


#endif
