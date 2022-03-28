#include "protoserv.h"

char	*get_board(s_board board){
	
	buffer = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(pt == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return 0;
        }


	return buffer;
} 
