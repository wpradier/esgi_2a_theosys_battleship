#include "protoserv.h"

int	handle_son(int ns, s_credentials credentials);

s_credentials		empty_credentials() {
	s_credentials	credentials;

	credentials.logins_id = 0;
	credentials.passwords_id = 0;
	credentials.quantity_id = 0;

	return credentials;
}

s_credentials		init_credentials_shms(int *error) {
	s_credentials	credentials;
	char		*logins;
	char		*passwords;
	int		*quantity;

	*error = 0;

	if ((credentials.logins_id = shmget((key_t)LOGINS_SHM_KEY,
					sizeof(char) * MAX_LOGIN_SIZE * MAX_USERS,
					IPC_CREAT|0700)) == -1) {
		*error = 1;
		perror("Erreur shmget (logins)");
		return empty_credentials();
	}


	if ((credentials.passwords_id = shmget((key_t)PASSWORDS_SHM_KEY,
					sizeof(char) * MAX_PASSWORD_SIZE * MAX_USERS,
					IPC_CREAT|0700)) == -1) {
		*error = 1;
		perror("Erreur shmget (passwords)");
		return empty_credentials();
	}

	if ((credentials.quantity_id = shmget((key_t)QUANTITY_SHM_KEY,
					sizeof(int),
					IPC_CREAT|0700)) == -1) {
		*error = 1;
		perror("Erreur shmget (quantity)");
		return empty_credentials();
	}

	logins = shmat(credentials.logins_id, 0, IPC_NOWAIT);
	passwords = shmat(credentials.passwords_id, 0, IPC_NOWAIT);
	quantity = shmat(credentials.quantity_id, 0, IPC_NOWAIT);

	bzero(logins, sizeof(char) * MAX_LOGIN_SIZE * MAX_USERS);
	bzero(passwords, sizeof(char) * MAX_PASSWORD_SIZE * MAX_USERS);
	*quantity = 0;

	shmdt(logins);
	shmdt(passwords);
	shmdt(quantity);

	return (credentials);
}

int			comserv() {
	int		sd;
	int		ns;
	int		fromlen;
	int		retfork;
	int		error;
	short		start_game;
	s_sockaddr_in	user_addr;
	s_credentials	credentials;



	if (!(sd = create_socket())) {
		return (0);
	}

	credentials = init_credentials_shms(&error);
	if (error) {
		return (0);
	}

	
	printf("logins shm id: %d\n", credentials.logins_id);
	printf("passwords shm id: %d\n", credentials.passwords_id);
	printf("quantity shm id: %d\n", credentials.quantity_id);

	add_user(credentials, "admin", "admin");

	start_game = 0;
	while (1) {
		printf("En attente de connection d'un client...\n");
		ns = accept(sd, (struct sockaddr *)&user_addr, (socklen_t *)&fromlen);

		if (ns == -1) {
			perror("Erreur accept\n");
			return (0);
		}

		printf("CREATION FILS\n");
		retfork = fork();

		if (retfork == 0) {
			handle_son(ns, credentials);
			exit (EXIT_SUCCESS);
		}

		if (start_game) {
			break;
		}
	}

	return (1);
} 


int			handle_son(int ns, s_credentials credentials) {
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


	ret_login = login_user(credentials, login, password);

	if (!ret_login) {
		serv_send(ns, STOP_CONNECTION, "Invalid credentials, disconnecting.");
		return (0);
	}
	

	strncpy(msg, "Welcome, ", MSG_SIZE);
	strncat(msg, login, MSG_SIZE - 1);

	if (!serv_send(ns, INFO, msg)) {
		return (0);
	}

	if (ret_login == 1) {
		printf("---ADMIN---\n");
	}

	if (ret_login == 2) {
		printf("---PLAYER---\n");
	}

	return (1);
}
