#include "battleshipClient.h"

#define ADDRESS "192.168.0.145"
#define PORT 	12345

//int 			main(int argc, char **argv){
int 			main(){
	int 		id, choice;
	short		programLoop = 1;	
	char 		msg[100];	

	id = screate();

	while(programLoop){
		printf("=====MENU=====\n1: Connexion au serveur\n2: Ecrire sur la mémoire\n3: Lire la mémoire\n0: Quitter\n");
 		printf("Que voulez vous faire ?   ");
 		scanf("%d", &choice);

                switch(choice){
			case 0: 
				return EXIT_SUCCESS; 
				break;
			case 1:
				srvConnect();
				printf("connect...\n");
				break;

			case 2: 
				printf("Message: ");
                       		scanf("%s",msg);
                        	swrite(id, msg);
                        	break;
			case 3:
				sread(id);
				break;
			default:
				printf("Invalide input\n");
                }

		choice = 0;

	}

	return EXIT_SUCCESS;

}
