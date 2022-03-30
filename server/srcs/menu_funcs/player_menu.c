#include "protoserv.h"

int		validate_value(char c, char max) {

	if (max >= 'a' && max <= 'f') {
		return ((c >= 'a' && c <= max) || (c >= '0' && c <= '9'));
	}

	return (c >= '0' && c <= max);
}

char		convert_c(int n) {
	if (n >= 0 && n <= 9) {
		return (n + 48);
	}
	return (n + 87);
}

int		convert_i(char c) {
	if (c >= '0' && c <= '9') {
		return (c - 48);
	}
	return (c - 87);
}


int		player_menu(int ns, int pipes[2][2], s_board board) {
	char	serv_msg[MSG_SIZE];
	char	cli_msg[MSG_SIZE];
	char	buff[MSG_SIZE];
	int	points;

	bzero(buff, MSG_SIZE);
	if (!serv_send(ns, INFO, "Waiting all players to start game...\n"))return (0);

	while (1) {
		read(pipes[FROMSERV][P_READ], serv_msg, MSG_SIZE);
		printf("SON: RECEIVED FROM SERV (1): %s\n", serv_msg);
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

		printf("SON: RECEIVED FROM SERV (2): %s\n", serv_msg);
		if (!strncmp(serv_msg, END_GAME, 2)) {
			if (!serv_send(ns, STOP_CONNECTION, serv_msg + 2))return (0);
			break;
		}

		if (!strncmp(serv_msg, DISPLAY, 2)) {
			if (!serv_send(ns, INFO, serv_msg + 2))return (0);
			continue;
		}

		if (!strncmp(serv_msg, PLAY, 2)) {

			while (1) {
				if (!serv_send(ns, GET_INPUT, "Your turn!\nType <x>:<y>\n"))return 0;
				recv(ns, cli_msg, MSG_SIZE, 0);

				if (!strchr(cli_msg, ':')) {
					if (!serv_send(ns, INFO, "Missing ':'\n"))return 0;
					continue;
				}

				if (!validate_value(cli_msg[0], convert_c(board.width - 1)) || !validate_value(cli_msg[2], convert_c(board.height - 1))) {
					snprintf(buff, MSG_SIZE, "Invalid value (max x = %c, max y = %c)\n", convert_c(board.width - 1), convert_c(board.height - 1));
					if (!serv_send(ns, INFO, buff))return 0;
					continue;
				}

				points = strike(board, convert_i(cli_msg[0]), convert_i(cli_msg[0]));
				snprintf(buff, MSG_SIZE, "%d", points);
				write(pipes[TOSERV][P_WRITE], buff, MSG_SIZE);

				break;
			}
			continue;
		}
	}

	return (1);
}
