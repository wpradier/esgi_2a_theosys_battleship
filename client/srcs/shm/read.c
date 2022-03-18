#include "battleshipClient.h"

int 		sread(int id){
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
