#include "protoserv.h"

int		strike(s_board board, int x, int y){

	char 	*buffer;
	int	points;

	buffer = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(buffer == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return (-1);
        }


	points = 0;

	if (buffer[board.width * y + x] == 'B'){
		buffer[board.width * y + x] = 'C';
		points = 1;
	}

	shmdt(buffer);	

	return (points);
}
