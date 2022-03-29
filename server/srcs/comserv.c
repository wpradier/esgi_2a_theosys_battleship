#include "protoserv.h"

int	handle_son(int ns, s_users *users);

int			connect_admin(int sd) {
	s_sockaddr_in	user_addr;
	int		fromlen;
	int		ns;
	char		u_login[MSG_SIZE];
	char		u_password[MSG_SIZE];


	printf("En attente de connection de l'admin...\n");
	ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

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

int			comserv() {
	int		sd;
	int		ns;
	int		fromlen;
	int		retfork;
	int		ad_pipe[2];
	//int		u_pipes[MAX_USERS][2];
	s_sockaddr_in	user_addr;
	s_users		*users;
	s_board		board;


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
	
	pipe(ad_pipe);
	retfork = fork();

	if (retfork == -1) {
		free_users(users);
		return (0);
	}

	if (retfork == 0) {
		initial_admin_menu(ns, ad_pipe[1]);
		if (!serv_send(ns, INFO, "Admin phase over. Waiting for game to start...\n")) {
			return (0);
		}
		while (1);
	}

	board = admin_phase(ad_pipe[0], users);

	printf("---BOARD FINAL---\n%s", get_board(board, 1));
	while (1) {
		printf("En attente de connection d'un client...\n");
		ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

		if (ns == -1) {
			perror("Erreur accept\n");
			free_users(users);
			return (0);
		}

		printf("CREATION FILS\n");
		retfork = fork();

		if (retfork == -1) {
			free_users(users);
			return (0);
		}

		if (retfork == 0) {
			handle_son(ns, users);
			exit (EXIT_SUCCESS);
		}
	}

	return (1);
} 


int			handle_son(int ns, s_users *users) {
	char		msg[MSG_SIZE];
	char		login[MSG_SIZE];
	char		password[MSG_SIZE];
	int		ret_login;

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


	ret_login = login_user(users, login, password);

	if (!ret_login) {
		serv_send(ns, STOP_CONNECTION, "Invalid credentials, disconnecting.");
		return (0);
	}
	

	strncpy(msg, "Welcome, ", MSG_SIZE);
	strncat(msg, login, MSG_SIZE - 1);

	if (!serv_send(ns, INFO, msg)) {
		return (0);
	}

	if (!serv_send(ns, STOP_CONNECTION, "Nothing left to do.\n")) {
		return (0);
	}

	return (1);
}
