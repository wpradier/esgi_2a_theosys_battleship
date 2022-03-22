#ifndef BATTLESHIPSERVER_H

# define BATTLESHIPSERVER_H


# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/shm.h>


typedef struct sockaddr_in s_sockaddr_in; 
int	sread(int id);
int     swrite(int id, char buffer[100]);
int	screate();
int 	srvConnect();
int 	srvSender(char	*message);
int	srvListenner(int id);



#endif
