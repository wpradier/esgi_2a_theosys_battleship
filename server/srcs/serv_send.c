#include "protoserv.h"


int	serv_send(int ns, char *proto, char *msg) {
	char final_msg[MSG_SIZE];

	strncpy(final_msg, proto, MSG_SIZE);
	strncat(final_msg, ":", MSG_SIZE - 1);
	strncat(final_msg, msg, MSG_SIZE - 1);

	if (send(ns, final_msg, MSG_SIZE, 0) == -1) {
		perror("Erreur send");
		return (0);
	}

	return (1);
}
