#include "protoserv.h"

int		admin_connection_menu(int ns, int ad_pipes[2][2]) {
	char	srv_msg[MSG_SIZE];
	char	cli_msg[MSG_SIZE];
	char	buff[MSG_SIZE];

	bzero(buff, MSG_SIZE);
	while (1) {
		read(ad_pipes[FROMSERV][P_READ], srv_msg, MSG_SIZE);

		if (!strncmp(srv_msg, END_CONN, 2)) {
			return (1);
		}

		if (!strncmp(srv_msg, CONN, 2)) {
			strncpy(buff, srv_msg + 2, MSG_SIZE);
			strcat(buff, " is trying to join. (y/n)\n");
			while (1) {
				serv_send(ns, GET_INPUT, buff);
				recv(ns, cli_msg, MSG_SIZE, 0);
				if (cli_msg[0] == 'y') {
					strncpy(buff, VALID, MSG_SIZE);
					write(ad_pipes[TOSERV][P_WRITE], buff, MSG_SIZE);
					break;
				}

				if (cli_msg[0] == 'n') {
					strncpy(buff, INVALID, MSG_SIZE);
					write(ad_pipes[TOSERV][P_WRITE], buff, MSG_SIZE);
					break;
				}
				serv_send(ns, INFO, "I did not understand your answer.\n");
			}
			continue;
		}
	}
}
