#include "protoserv.h"

int	handle_son(int ns, s_users *users, int pipes[2][2], s_board board);

int			connect_admin(int sd) {
	s_sockaddr_in	user_addr;
	int		fromlen;
	int		ns;
	char		u_login[MSG_SIZE];
	char		u_password[MSG_SIZE];


	printf("En attente de connection de l'admin...\n");
	ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

	if (ns == -1) {
		perror("Erreur accept admin");
		return (-2);
	}
	if (!serv_send(ns, GET_INPUT, "Login:")) {
		return (-2);
	}

	if (recv(ns, u_login, MSG_SIZE, 0) == -1) {
		return (-2);
	}
	
	if (!serv_send(ns, GET_INPUT, "Password:")) {
		return (-2);
	}

	if (recv(ns, u_password, MSG_SIZE, 0) == -1) {
		return (-2);
	}

	if (!strncmp(u_login, "admin", MAX_LOGIN_SIZE)
			&& !strncmp(u_password, "admin", MAX_PASSWORD_SIZE)) {
		if (!serv_send(ns, INFO, "Welcome, admin.")) {
			return (-2);
		}

		return (ns);
	}

	if (!serv_send(ns, STOP_CONNECTION, "Wrong credentials.")) {
		return (-2);
	}

	return (-1);
}

void			close_dual_pipes(int pipes[2][2]) {
	int		i;
	int		j;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			close(pipes[i][j]);
		}
	}
}

int			son_connected(int ad_pipes[2][2], int son_pipes[2][2]) {
	char		buff[MSG_SIZE];
	char		login[MAX_LOGIN_SIZE];


	// wait for credential validation
	read(son_pipes[TOSERV][P_READ], buff, MSG_SIZE);
	if (!strncmp(buff, INVALID, 2)) {
		close_dual_pipes(son_pipes);
		return (0);
	}

	strncpy(login, buff + 2, MAX_LOGIN_SIZE);
	
	strncpy(buff, CONN, MSG_SIZE);
	strcat(buff, login);
	
	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);

	// wait for admin response
	read(ad_pipes[TOSERV][P_READ], buff, MSG_SIZE);
	// send admin response to user
	write(son_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);

	if (!strncmp(buff, VALID, 2)) {
		return (1);
	}

	close_dual_pipes(son_pipes);

	return (0);
}

int			comserv() {
	int		sd;
	int		ns;
	int		fromlen;
	int		retfork;
	int		ad_pipes[2][2];
	int		u_pipes[MAX_USERS][2][2];
	size_t		i;
	s_sockaddr_in	user_addr;
	s_users		*users;
	s_board		board;
	char		buff[MSG_SIZE];


	if (!(sd = create_socket())) {
		return (0);
	}


	ns = -1;
	while (ns < 0) {
		ns = connect_admin(sd);
		
		if (ns == -2) { // error
			return (0);
		}
	}


	users = init_users();
	
	pipe(ad_pipes[FROMSERV]);
	pipe(ad_pipes[TOSERV]);
	retfork = fork();

	if (retfork == -1) {
		free_users(users);
		return (0);
	}

	if (retfork == 0) {
		initial_admin_menu(ns, ad_pipes);
		if (!serv_send(ns, INFO, "Admin phase over. Waiting for players to connect...\n")) {
			return (0);
		}
		admin_connection_menu(ns, ad_pipes);
		admin_menu(ns, ad_pipes);

		close_dual_pipes(ad_pipes);
		exit (EXIT_SUCCESS);
	}

	board = admin_phase(ad_pipes, users);

	printf("---BOARD FINAL---\n%s", get_board(board, 1));
	
	i = 0;
	while (1) {
		printf("En attente de connection d'un client...\n");
		ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

		if (ns == -1) {
			perror("Erreur accept\n");
			free_users(users);
			return (0);
		}

		pipe(u_pipes[i][FROMSERV]);
		pipe(u_pipes[i][TOSERV]);

		printf("CREATION FILS\n");
		retfork = fork();

		if (retfork == -1) {
			free_users(users);
			return (0);
		}

		if (retfork == 0) {
			handle_son(ns, users, u_pipes[i], board);
			exit (EXIT_SUCCESS);
		}

		if (son_connected(ad_pipes, u_pipes[i])) {
			i++;
		}

		if (i == users->quantity) {
			break;
		}
	}

	printf("---START GAME---\n");
	strncpy(buff, END_CONN, MSG_SIZE);
	write(ad_pipes[FROMSERV][P_WRITE], buff, MSG_SIZE);
	printf("---Entering gestpart---\n");
	gestpart(board, ad_pipes, u_pipes, users);

	return (1);
} 


int			handle_son(int ns, s_users *users, int pipes[2][2], s_board board) {
	char		msg[MSG_SIZE];
	char		login[MSG_SIZE];
	char		password[MSG_SIZE];

	if (!serv_send(ns, INFO, "Welcome to the battleship game!\nPlease enter your credentials.")) {
		return (0);
	}


	if (!serv_send(ns, GET_INPUT, "Login:")) {
		return (0);
	}

	recv(ns, login, MSG_SIZE, 0);
	printf("login reçu: '%s'\n", login);

	if (!serv_send(ns, GET_INPUT, "Password:")) {
		return (0);
	}

	recv(ns, password, MSG_SIZE, 0);
	printf("password reçu: '%s'\n", password);


	if (!login_user(users, login, password)) {
		strncpy(msg, INVALID, MSG_SIZE);
		write(pipes[TOSERV][P_WRITE], msg, MSG_SIZE);
		serv_send(ns, STOP_CONNECTION, "Invalid credentials, disconnecting.");
		return (1);
	}
	
	strncpy(msg, VALID, MSG_SIZE);
	strcat(msg, login);
	write(pipes[TOSERV][P_WRITE], msg, MSG_SIZE);

	serv_send(ns, INFO, "Waiting for the admin to validate connection...\n");
	bzero(msg, MSG_SIZE);
	read(pipes[FROMSERV][P_READ], msg, MSG_SIZE);
	if (!strncmp(msg, INVALID, 2)) {
		serv_send(ns, STOP_CONNECTION, "Admin refused connexion.");
		return (1);
	}

	strncpy(msg, "Welcome, ", MSG_SIZE);
	strncat(msg, login, MSG_SIZE - 1);

	if (!serv_send(ns, INFO, msg)) {
		return (0);
	}

	player_menu(ns, pipes, board);
	if (!serv_send(ns, STOP_CONNECTION, "Nothing left to do.\n")) {
		return (0);
	}

	close_dual_pipes(pipes);

	return (1);
}
