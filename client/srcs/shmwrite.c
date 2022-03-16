#include "battleshipClient.h"

#define CLE 101

int 		create(){
	int 	id;

	id = shmget((key_t)CLE, 100, IPC_CREAT|0700);
	if(id == -1){
		perror("erreur shgmet()\n");
		return(EXIT_FAILURE);

	}

	return id;
}


int		write(int id, char buffer[100]){
	int	 i;
	char	 *pt;

    	printf("\necrire SHM\n");
    	pt = (char *) shmat((key_t)id, 0, IPC_NOWAIT);
    	if(pt == (char*)-1){
         	perror("\nerreur shmat()\n");
         	return(EXIT_SUCCESS);
    	}

        i=0;

	strcpy(pt,buffer);
	shmdt(pt);

	return EXIT_SUCCESS;
}


int 		main(int argc, char **argv){
	int 	id, i,loop, choice;
	char 	*pt;
	char 	buffer[100];
	
	loop = 1;
	
	id = create();

	while(loop){
		printf("=====MENU=====\n1: Ecrire sur la mémoire\n0: Quitter\n");
		printf("Que voulez vous faire ?   ");
		scanf("%d", &choice);
		
		if(choice){
			printf("Message: ");
			scanf("%s",buffer);
			write(id, buffer);
			choice = 0;				
		} else {
			return EXIT_SUCCESS;
		}
	}

	return EXIT_SUCCESS;
}
