#include "protoserv.h"

s_users		*init_users() {
	s_users	*users;
	int	i;

	users = malloc(sizeof(s_users));
	users->logins = malloc(sizeof(char*) * MAX_USERS);
	users->passwords = malloc(sizeof(char*) * MAX_USERS);

	i = 0;
	while (i < MAX_USERS) {
		users->logins[i] = calloc(MAX_LOGIN_SIZE, sizeof(char));
		users->passwords[i] = calloc(MAX_PASSWORD_SIZE, sizeof(char));
		i++;
	}

	users->quantity = 0;

	return (users);
}
