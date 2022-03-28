#ifndef BATTLESHIPSERVER_H

# define BATTLESHIPSERVER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <arpa/inet.h>

# define MSG_SIZE 2048

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

int	connect_to_battleship();
int	client_loop(int sd);

#endif
