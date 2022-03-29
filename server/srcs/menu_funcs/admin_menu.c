#include "protoserv.h"


int		admin_menu(int ns, int pipes[2][2]) {
	char	serv_msg[MSG_SIZE];

	if (!serv_send(ns, INFO, "Waiting all players to start game...\n"))return (0);

	while (1) {
		read(pipes[FROMSERV][P_READ], serv_msg, MSG_SIZE);
		if (!strncmp(serv_msg, DISPLAY, 2)) {
			if (!serv_send(ns, INFO, serv_msg + 2))return 0;
			continue;
		}
		if (!strncmp(serv_msg, START_GAME, 2)) {
			break;
		}
	}

	if (!serv_send(ns, INFO, "STARTING GAME !\n"))return (0);

	while (1) {
		read(pipes[FROMSERV][P_READ], serv_msg, MSG_SIZE);

		if (!strncmp(serv_msg, END_GAME, 2)) {
			break;
		}

		if (!strncmp(serv_msg, DISPLAY, 2)) {
			if (!serv_send(ns, INFO, serv_msg + 2))return 0;
			continue;
		}
	}

	return (1);
}
