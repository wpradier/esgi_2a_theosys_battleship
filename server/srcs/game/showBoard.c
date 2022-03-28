#include "protoserv.h"

int		show_board(s_board board){
	short	i;
	short	y;

	char	arrayMove[30] = {"XXXCXXXCXBXBXXXXXXXXXXXXXXXX"};
	

	for(i = 0; i < board.height; i++){
		for(y = 0; y < board.width; y++){
			printf("+---");
		}
		printf("+\n");

		for(y = 0; y < board.width; y++){
      			if(arrayMove[board.width * i + y] == 'C'){
				printf("| C ");
				y++;				
	             	} else {
				printf("|   ");
			}
		}
                printf("|\n");
	}
	for(y = 0; y < board.width; y++){
		printf("+---");
        }
        printf("+\n");

	return 1;
}
