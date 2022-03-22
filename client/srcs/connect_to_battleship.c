#include "battleshipClient.h"

#define ADDRESS "127.0.0.1"
#define PORT 	12345
#define BUFF_SIZE	500

int 			connect_to_battleship(){
	int 		sd;
	s_sockaddr_in 	dest_addr;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == 1){
		perror("Erreur socket"); 
		return (0);
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	dest_addr.sin_addr.s_addr = inet_addr(ADDRESS);
	bzero(&(dest_addr.sin_zero),8);
	if((connect(sd,(struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) == -1){
		perror("Error connect");
		return (0);
	}
	
	return (sd);
}
