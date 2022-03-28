#include "protoserv.h"

int		strike(s_board board, int x, int y){

	char 	*buffer;
	s_board board;

	buffer = get_board(board);

	if(buffer[board.width * x + y] == 'B'){
		buffer[board.width * x + y] = 'C'; 
	}
	

	return 1;
}
