#include  "battleshipServer.h"
#define ADDRESS "192.168.0.145"
#define PORT 12345
#define NBCLI 3


int			main() {
	int		sd;
	int		ns;
	int		fromlen;
	int		i;
	int		retfork;
	int		retsend;
	s_sockaddr_in	my_addr;
	s_sockaddr_in	user_addr;

	char	*message = "Wesh alors bien ou bien";

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
		exit(EXIT_FAILURE);
	}

	if ((listen(sd, NBCLI)) == -1) {
		perror("Erreur listen :\n");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while (1) {
		printf("En attente de connection d'un client...\n");
		ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

		if (ns == -1) {
			perror("Erreur accept :\n");
			exit(EXIT_FAILURE);
		}
		i++;

		retfork = fork();

		if (retfork == 0) {
			retsend = write(ns, message, strlen(message) + 1);
			
			if (retsend == -1) {
				perror("Erreur write :\n");
				exit(EXIT_FAILURE);
			}

			printf("i = %d, retour send = %d\n", i, retsend);
			exit(EXIT_SUCCESS);
		}

		if (i == NBCLI) {
			exit(EXIT_FAILURE);
		}
	}
}
