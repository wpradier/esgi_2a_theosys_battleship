#include "protoserv.h"

void			shmdt_all(char *a, char *b, int *c) {
	shmdt(a);
	shmdt(b);
	shmdt(c);
}

int                     add_user(s_credentials credentials, char *login, char *password) {
	char		*logins;
	char		*passwords;
	int		*quantity;
	int		i;


	logins = shmat(credentials.logins_id, 0, IPC_NOWAIT);
	passwords = shmat(credentials.passwords_id, 0, IPC_NOWAIT);
	quantity = shmat(credentials.quantity_id, 0, IPC_NOWAIT);

	if ((*quantity) >= MAX_USERS) {
		printf("Cannot add more users");
		shmdt_all(logins, passwords, quantity);
		return (0);
	}

	i = *quantity;

	strncpy(&logins[login_i(i)], login, MAX_LOGIN_SIZE);
	strncpy(&passwords[password_i(i)], password, MAX_PASSWORD_SIZE);

	(*quantity)++;

	shmdt_all(logins, passwords, quantity);

	return (1);
}
