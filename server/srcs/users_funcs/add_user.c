#include "protoserv.h"


int                     add_user(s_users *users, char *login, char *password) {
	size_t		i;

	i = users->quantity;

	if (i >= MAX_USERS) {
		printf("Cannot add more users\n");
		return (0);
	}


	strncpy(users->logins[i], login, MAX_LOGIN_SIZE);
	strncpy(users->passwords[i], password, MAX_PASSWORD_SIZE);

	users->quantity += 1;

	return (1);
}
