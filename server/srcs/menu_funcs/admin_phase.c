#include "protoserv.h"

int		parse_value(char c) {
	if (c >= 'a' && c <= 'f') {
		return (c - 87); // hex
	}

	if (c >= '0' && c <= '9') {
		return (c - 48);
	}

	return (-1);
}

s_board		admin_phase(int admin_fd, s_users *users) {
	char	admin_msg[MSG_SIZE];
	char	login[MAX_LOGIN_SIZE];
	char	password[MAX_PASSWORD_SIZE];
	char	*shmpt;
	int	i;
	int	j;
	int	x;
	int	y;
	s_board	board;

	board.shm_id = -1;
	board.width = 0;
	board.height = 0;
	board.len = 0;
	while (1) {
		printf("---WAITING FOR ADMIN MSG---\n");
		read(admin_fd, admin_msg, MSG_SIZE);
		printf("---RECEIVED ADMIN MSG---: %s\n", admin_msg);
		
		if (!strncmp(admin_msg, END_PHASE, 2)) {
			printf("---END MENU---\n");
			return (board);
		}

		if (!strncmp(admin_msg, BOARD_SIZE, 2)) {
			printf("---BOARD SIZE---\n");
			board.width = parse_value(admin_msg[2]);
			board.height = parse_value(admin_msg[3]);
			board.len = board.width * board.height;
			printf("w: %d\n", board.width);
			printf("h: %d\n", board.height);
			printf("l: %d\n", board.len);
			board.shm_id = shmget((key_t)BOARD_SHM_KEY,
					sizeof(char) * board.len,
					IPC_CREAT|0700);

			shmpt = (char*)shmat((key_t)board.shm_id, 0, IPC_NOWAIT);
			i = 0;
			while (i < board.len) {
				shmpt[i] = 'X';
				i++;
			}

			printf("SHM CONTENT:\n");
			i = 0;
			while (i < board.len) {
				printf("%c", shmpt[i]);
				i++;
			}
			printf("\n");
		}

		if (!strncmp(admin_msg, CREDENTIALS, 2)) {
			printf("---CREDENTIALS---\n");
			i = 2;
			j = 0;
			bzero(login, MAX_LOGIN_SIZE);
			while (admin_msg[i] != ':') {
				login[j] = admin_msg[i];
				i++;
				j++;
			}
			
			j = 0;
			i++;
			bzero(password, MAX_PASSWORD_SIZE);
			while (admin_msg[i] != '\0') {
				password[j] = admin_msg[i];
				i++;
				j++;
			}

			add_user(users, login, password);
		}
		
		if (!strncmp(admin_msg, BOAT_COORDS, 2)) {
			printf("---BOAT COORDS---\n");
			x = parse_value(admin_msg[2]);
			y = parse_value(admin_msg[3]);

			shmpt[(y * board.width) + x] = 'B';
			printf("SHM CONTENT:\n");
			i = 0;
			while (i < board.len) {
				printf("%c", shmpt[i]);
				i++;
			}
			printf("\n");

		}

	}
}
