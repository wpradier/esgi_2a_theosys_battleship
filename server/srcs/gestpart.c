#include "protoserv.h"


int		detect_endgame(s_board board) {
	int	i;
	int	detect_boat;
	char	*shmpt;


	shmpt = (char*)shmat((key_t)board.shm_id, 0, IPC_NOWAIT);

	detect_boat = 0;
	i = 0;
	while (i < board.len) {
		if (shmpt[i] == 'B') {
			detect_boat = 1;
		}
		i++;
	}

	return (!detect_boat);
}

char		*get_winners(int *points, s_users *users) {
	int	i;
	int	q;
	int	tot;
	int	max;
	char	winners[MSG_SIZE];
	char	*buff;


	buff = malloc(sizeof(char) * MSG_SIZE);

	bzero(winners, MSG_SIZE);
	bzero(buff, MSG_SIZE);

	i = 0;
	q = users->quantity;
	max = 0;
	while (i < q) {
		if (points[i] > max) {
			max = points[i];
		}
		i++;
	}

	i = 0;
	tot = 0;
	while (i < q) {
		if (points[i] == max) {
			if (tot > 0) {
				strcat(winners, ", ");
			}
			strcat(winners, users->logins[i]);
			
			tot++;
		}
		i++;
	}

	if (tot >= 2) {
		strncpy(buff, "Winners: ", MSG_SIZE);
	} else {
		strncpy(buff, "Winner: ", MSG_SIZE);
	}

	strcat(buff, winners);
	strcat(buff, " !\n");

	return (buff);
}

void		display_endgame(int *points, int ad_pipes[2][2], int u_pipes[MAX_USERS][2][2], s_users *users, s_board board) {
	int	i;
	int	q;
	char	buff[MSG_SIZE];
	char	end[MSG_SIZE];
	char	score[5];
	char	*winners;


	strncpy(buff, DISPLAY, MSG_SIZE);
	strcat(buff, "---END OF GAME---\n");
	strcat(buff, get_board(board, 0));

	strncpy(end, END_GAME, MSG_SIZE);
	strcat(end, "Bye !");

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

	winners = get_winners(points, users);
	strcat(buff, winners);
	free(winners);

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


	
	q = users->quantity;
	
	points = malloc(sizeof(int) * q);
	
	strncpy(buff, START_GAME, MSG_SIZE);
	printf("---starting game for admin---\n");
	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	printf("---starting game for users---\n");
	i = 0;
	while (i < q) {
		write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
		points[i] = 0;
		i++;
	}

	printf("---game started---\n");
	while (1) {
		i = 0;
		while (i < q) {

			if (detect_endgame(board)) {
				display_endgame(points, ad_pipes, u_pipes, users, board);
				return (1);
			}
			
			display_gamestate(board, points, ad_pipes, u_pipes, users);
			display_turn(ad_pipes, u_pipes, users, i);

			strncpy(buff, PLAY, MSG_SIZE);
			write(u_pipes[i][FROMSERV][P_WRITE], buff, MSG_SIZE);
			read(u_pipes[i][TOSERV][P_READ], buff, MSG_SIZE);
			points[i] += atoi(buff);

			i++;
		}
		
	}
}
