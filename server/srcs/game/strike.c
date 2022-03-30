#include "protoserv.h"


/*int		check_sinked(s_board board) {
	int	i;
	int	j;
	int	k;
	int	l;
	char	*pt;

	i = 0;
	pt = (char*)shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
	while (i < board.height) {
		j = 0;
		while (j < board.len) {
			if (pt[(i * board.len) + j] == 'T') { // if cell is struck
				k = i;
				l = j + 1;
				while (l < board.len) {
					if (pt[(k * board.len) + l] == 'B') {
						break;
					}
					l++;
				}
			}
			j++;
		}
		i++;
	}
}*/

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
		points++;
	}

	shmdt(buffer);	

	return (points);
}
