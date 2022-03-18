#include "battleshipClient.h"

int		swrite(int id, char buffer[100]){
	char	 *pt;

    	printf("\necrire SHM\n");
    	pt = (char *) shmat((key_t)id, 0, IPC_NOWAIT);
    	if(pt == (char*)-1){
         	perror("\nerreur shmat()\n");
         	return EXIT_FAILURE;
    	}

	strcpy(pt,buffer);
	shmdt(pt);

	return EXIT_SUCCESS;
}
