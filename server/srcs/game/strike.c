#include "protoserv.h"

int		strike(s_board board, int x, int y){
	char 	*buffer;
	int	points;
	int	i;

	buffer = (char *) shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	if(buffer == (char*)-1){ 
                perror("\nerreur shmat()\n");
                return (-1);
        }


	points = 0;

	i = 0;
	printf("buffer in strike:\n");
	while (i < board.len) {
		printf("%c", buffer[i]);
		i++;
	}
	printf("\n");

	printf("x = %d\n", x);
	printf("y = %d\n", y);
	printf("board.width = %d\n", board.width);
	printf("board.width * y + x = %d\n", board.width * y + x);
	if (buffer[board.width * y + x] == 'B'){
		buffer[board.width * y + x] = 'C';
		points = 1;
	}

	shmdt(buffer);	

	return (points);
}
