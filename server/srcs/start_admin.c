#include "protoserv.c"


int		start_admin(s_allusers *allusers) {
	char	buff[MSG_SIZE];
	int	startGame;

	startGame = 0;
	while(!startGame) {
		strncpy(buff, "---ADMIN MENU---\n0: Add player\n1: Start game\n");
	}
}
