#include "battleshipClient.h"

#define ADDRESS "127.0.0.1"
#define PORT 	12345
#define BUFF_SIZE	500

int 			main(){
	int 		sd;

	if (!(sd = connect_to_battleship())) {
		return (EXIT_FAILURE);
	}

	if (!client_loop(sd)) {
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
