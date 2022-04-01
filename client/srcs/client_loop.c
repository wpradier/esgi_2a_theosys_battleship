#include "battleshipClient.h"

void		display_srvMsg(char *str) {
	printf("\033[0;31m");
	printf("Server:\n%s\n", strchr(str, ':') + 1);
	printf("\033[0m"); 
}

void		manage_input(char *str) {
	int	len;

	fgets(str, MSG_SIZE, stdin);
	len = strlen(str);

	if (len <= MSG_SIZE) {
		str[len - 1] = '\0';
	}
}

int		client_loop(int sd) {
	char	srvMsg[MSG_SIZE];
	char	cliMsg[MSG_SIZE];

	while(1) {
		if (recv(sd, srvMsg, MSG_SIZE, 0) == -1) {
			perror("Error receiving server message");
			return (0);
		}

		if (!strchr(srvMsg, ':')) {
			continue;
		}

		display_srvMsg(srvMsg);

		if (!strncmp(srvMsg, STOP_CONNECTION, 2)) {
			return (1);
		}

		if (!strncmp(srvMsg, INFO, 2)) {
			continue;
		}

		if (!strncmp(srvMsg, GET_INPUT, 2)) {
			manage_input(cliMsg);
			if (send(sd, cliMsg, MSG_SIZE, 0) == -1) {
				perror("Error sending input to server");
				return (0);
			}
		}
	}	
}
