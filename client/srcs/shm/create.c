#define CLE 101
#include "battleshipClient.h"


int		screate(){
	int	id;
	id = shmget((key_t)CLE, 100, IPC_CREAT|0700);
	if(id == -1){
		perror("error shgmet()\n");
		return EXIT_FAILURE;
	}
	
	return id;
}
