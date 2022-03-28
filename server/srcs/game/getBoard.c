#include "protoserv.h"

char		*get_board(s_board board){
	
	char  	*buffer = malloc(sizeof(char)*MSG_SIZE);
	char	*pt;

	pt = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(pt == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return 0;
        }

	

	for(i = 0; i < board.height; i++){
		for(y = 0; y < board.width; y++){
			strcat(buffer,"+---");
		}
		strcat(buffer,"+\n");

		for(y = 0; y < board.width; y++){
      			if(pt[board.width * i + y] == 'C'){
				strcat(buffer,"| C ");
				y++;				
	             	} else {
				strcat(buffer,"|   ");
				y++;
			}
		}
                strcat(buffer,"|\n");
	}
	for(y = 0; y < board.width; y++){
		strcat(buffer,"+---");
        }
        strcat(buffer,"+\n");


	return buffer;
} 
