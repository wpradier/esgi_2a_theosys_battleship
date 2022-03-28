//#include "protoserv.h"
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

int		showBoard(int size){

	char	*message;
	short	i;
	short	y;
	short 	x;

	char	arrayMove[10][2] = {"13", "22", "33", "44", "55", "66"};
	

	for(i = 0; i < size; i++){
		for(y = 0; y < size; y++){
			printf("+---");
		}
		printf("+\n");

		for(y = 0; y < size; y++){
                        for(x = 0; x < 5; x++){
								
				char iStr[2]; 
				char yStr[2];

				sprintf(iStr, "%d", i+1);
				sprintf(yStr, "%d", y+1);			
				
				char buff[2];
				strcpy(buff,arrayMove[x]);				
				
				if(buff[0] == iStr[0] && buff[1] == yStr[0]){
					printf("| x ");
					y++;
				}				
	             	}
			printf("|   ");
		}
                printf("|\n");
	}
	for(y = 0; y < size; y++){
		printf("+---");
        }
        printf("+\n");

	return 1;
}

int 		main(){
	int	choice;
	printf("Nombre de case en x*x: ");
	scanf("%d",&choice);
	showBoard(choice);
	return 0;
}
