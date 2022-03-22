#include "battleshipClient.h"

#define ADDRESS "192.168.64.144"
#define PORT 12345

int		srvSender(char	*message){
	int	sd;
	int	ns;
	//int	i;
	int	fromlen;
	int	retfork;
	int 	retsend;
	
	s_sockaddr_in	my_addr;
	s_sockaddr_in	user_addr;	


	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Erreur socket :\n");
		exit(EXIT_FAILURE);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = inet_addr(ADDRESS);
	bzero(&(my_addr.sin_zero), 8);
	
	if ((bind(sd,(struct sockaddr *) &my_addr, sizeof(struct sockaddr))) == -1) {
		perror("Erreur bind :\n");
		return EXIT_FAILURE;
	}

	/*
	if ((listen(sd, NBCLI)) == -1) {
		perror("Erreur listen :\n");
		return EXIT_FAILURE;
	}
	*/

	ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

	if (ns == -1) {
		perror("Erreur accept :\n");
		return EXIT_FAILURE;
	}
	
	retfork = fork();

	if (retfork == 0) {
		retsend = write(ns, message, strlen(message) + 1);
		
		if (retsend == -1) {
			perror("Erreur write :\n");
			return EXIT_FAILURE;
		}

		printf("retour send = %d\n", retsend);
	}

	/*
	if (i == NBCLI) {
		return EXIT_FAILURE;
	}
	*/


	return EXIT_SUCCESS;
}
