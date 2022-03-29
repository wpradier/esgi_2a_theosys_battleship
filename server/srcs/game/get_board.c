#include "protoserv.h"

char		*get_board(s_board board, int admin){
	int	i;
	int	j;
	char  	*buffer = malloc(sizeof(char)*MSG_SIZE);
	char	*pt;

	pt = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(pt == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return 0;
        }

	// i = y
	// j = x

	bzero(buffer, MSG_SIZE);
	for(i = 0; i < board.height; i++){
		for(j = 0; j < board.width; j++){
			strcat(buffer,"+---");
		}
		strcat(buffer,"+\n");

		for(j = 0; j < board.width; j++){
      			if (pt[(board.width * i) + j] == 'C'){
				strcat(buffer,"| C ");
	             	} else if (pt[(board.width * i) + j] == 'T') {
				strcat(buffer,"| T ");
	             	} else if (pt[(board.width * i) + j] == 'B' && admin) {
				strcat(buffer,"| B ");
			} else {
				strcat(buffer,"|   ");
			}
		}
                strcat(buffer,"|\n");
	}
	for(j = 0; j < board.width; j++){
		strcat(buffer,"+---");
        }
        strcat(buffer,"+\n");


	return buffer;
} 
