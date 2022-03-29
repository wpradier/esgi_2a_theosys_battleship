#include "protoserv.h"


int		detect_endgame(s_board board) {
	int	i;
	int	detect_boat;
	char	*shmpt;


	shmpt = (char*)shmat((key_t)board.shm_id, 0, IPC_NOWAIT);

	detect_boat = 0;
	while (i < board.len) {
		if (shmpt[i] == 'B') {
			detect_boat = 1;
		}
		i++;
	}

	return (!detect_boat);
}

void		display_endgame(int *points, int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users) {
	int	i;
	int	q;
	char	buff[MSG_SIZE];
	char	end[MSG_SIZE];
	char	score[5];


	strncpy(buff, DISPLAY, MSG_SIZE);
	strncpy(end, END_GAME, MSG_SIZE);
	strcat(buff, "---END OF GAME---\n");

	q = users->quantity;
	i = 0;

	while (i < q) {
		strcat(buff, users->logins[i]);
		strcat(buff, ": ");
		sprintf(score, "%d", points[i]);
		strcat(buff, score);
		strcat(buff, "\n");

		i++;
	}

	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	write(ad_pipes[FROMSERV][P_WRITE], end, MSG_SIZE);

	i = 0;
	while (i < q) {
		write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
		write(u_pipes[i][FROMSERV][P_WRITE], end, MSG_SIZE);
		i++;
	}
}

void		display_gamestate(s_board board, int *points, int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users) {
	int	i;
	int	q;
	char	buff[MSG_SIZE];
	char	board_buff[MSG_SIZE];
	char	score[5];


	q = users->quantity;

	i = 0;

	strncpy(buff, DISPLAY, MSG_SIZE);
	while (i < q) {
		strcat(buff, users->logins[i]);
		strcat(buff, ": ");
		sprintf(score, "%d", points[i]);
		strcat(buff, score);
		strcat(buff, "\n");
		
		i++;
	}

	strncpy(board_buff, DISPLAY, MSG_SIZE);
	strcat(board_buff, get_board(board, 1)); // admin board
	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	write(ad_pipes[FROMSERV][P_WRITE], board_buff, MSG_SIZE);
	
	strncpy(board_buff, DISPLAY, MSG_SIZE);
	strcat(board_buff, get_board(board, 0)); // player board
	
	i = 0;
	while (i < q) {
		write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
		write(u_pipes[i][FROMSERV][P_WRITE], board_buff, MSG_SIZE);
		i++;
	}
}

void		display_turn(int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users, int turn) {
	int	i;
	int	q;
	char	buff[MSG_SIZE];

	strncpy(buff, DISPLAY, MSG_SIZE);
	strcat(buff, users->logins[turn]);
	strcat(buff, "'s turn!\n");

	q = users->quantity;
	i = 0;

	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	while (i < q) {
		write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
		i++;
	}
}

int		gestpart(s_board board, int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users) {
	int	i;
	int	q;
	char	buff[MSG_SIZE];
	int	*points;


	points = malloc(sizeof(int) * users->quantity);
	q = users->quantity;
	
	strncpy(buff, START_GAME, MSG_SIZE);
	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	while (i < q) {
		write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
		points[i] = 0;
		i++;
	}

	while (1) {
		i = 0;
		while (i < q) {

			if (detect_endgame(board)) {
				display_endgame(points, ad_pipes, u_pipes, users);
				return (1);
			}
			
			display_gamestate(board, points, ad_pipes, u_pipes, users);
			display_turn(ad_pipes, u_pipes, users, i);

			strncpy(buff, PLAY, MSG_SIZE);
			write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
			read(u_pipes[i][TOSERV][P_READ], buff, MSG_SIZE);
			points[i] = atoi(buff);

			i++;
		}
		
	}
}
