#include "protoserv.h"

s_board		admin_phase(int admin_fd, s_users *users) {
	char	admin_msg[MSG_SIZE];
	int	w;
	int	h;
	s_board	board;

	board.shm_id = -1;
	board.width = 0;
	board.height = 0;
	while (1) {
		read(admin_fd, admin_msg, MSG_SIZE);
		printf("---RECEIVED ADMIN MSG---: %s\n", admin_msg);
		
		if (!strncmp(admin_msg, END_PHASE, 2)) {
			return (board);
		}

		if (!strncmp(admin_msg, BOARD_SIZE, 2)) {
			board.width = admin_msg[2];
			board.height = admin_msg[3];
			board.shm_id = shmget((key_t)BOARD_SHM_KEY,
					sizeof(char) * board.width * board.height,
					IPC_CREAT|0700);
		}

		if (!strncmp(admin_msg, CREDENTIALS, 2)) {

		}

	}
}
