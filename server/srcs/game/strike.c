#include "protoserv.h"

int		strike(s_board board, int x, int y){

	char 	*buffer;

	buffer = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(buffer == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return 0;
        }


	if(buffer[board.width * x + y] == 'B'){
		buffer[board.width * x + y] = 'C'; 
	}

	shmdt(buffer);	

	return 1;
}
