#include "battleshipClient.h"

int 		read(int id){
	char	*pt;

	pt = (char *) shmat((key_t)id, 0, IPC_NOWAIT);
        if(pt == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return(EXIT_FAILURE);
        }
	printf("Message: %s\n",pt);
	
	shmdt(pt);	

	return EXIT_SUCCESS;
}




int 		main(int argc, char **argv){
	int 	id, i, loop,	choice;
	char 	*pt;
	loop = 1;

	//id=atoi(argv[1]);
   	id = 21;

        while(loop){
		printf("=====MENU=====\n1: Lire la mémoire\n0: Quitter\n");
        	printf("Que voulez vous faire ?   ");
                scanf("%d", &choice);
                if(choice){
                        read(id);
                        choice = 0;       
                } else {
                        return EXIT_SUCCESS;
                 }
        }


}

