#include  "protoserv.h"
#define ADDRESS "127.0.0.1"
#define PORT 12345


int			create_socket() {
	int		sd;
	s_sockaddr_in	my_addr;


	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Erreur socket");
		return (0);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = inet_addr(ADDRESS);
	bzero(&(my_addr.sin_zero), 8);
	
	if ((bind(sd,(struct sockaddr *) &my_addr, sizeof(struct sockaddr))) == -1) {
		perror("Erreur bind");
		return(0);
	}

	if ((listen(sd, MAX_USERS)) == -1) {
		perror("Erreur listen");
		return(0);
	}

	return (sd);
}
