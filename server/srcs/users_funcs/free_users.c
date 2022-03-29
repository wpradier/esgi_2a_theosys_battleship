#include "protoserv.h"

void		free_users(s_users *users) {
	int	i;

	i = 0;
	while (i < MAX_USERS) {
		free(users->logins[i]);
		free(users->passwords[i]);
	}

	free(users->logins);
	free(users->passwords);

	free(users);
}
