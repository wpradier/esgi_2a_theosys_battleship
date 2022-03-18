#include "battleshipClient.h"

#define ADDRESS "192.168.0.145"
#define PORT 	12345

int		srvConnect(){
	int	sd, retrecv;
	char 	essai[39];
	//char	*pt;

        s_sockaddr_in   dest_addr;

        if((sd = socket(AF_INET, SOCK_STREAM, 0)) == 1){
                perror("\n Erreur socket : ");
                return EXIT_FAILURE;
        }

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        dest_addr.sin_addr.s_addr = inet_addr(ADDRESS);
        bzero(&(dest_addr.sin_zero),8);
        if((connect(sd,(struct sockaddr * )&dest_addr, sizeof(struct sockaddr))) == -1){
                perror("\n Error connect : ");
                return EXIT_FAILURE;
        }

        retrecv = recv(sd, essai, 39, 0);
        if(retrecv == -1){
                perror("\n Error recv : ");
                return EXIT_FAILURE;
        }


	return EXIT_SUCCESS;
}

