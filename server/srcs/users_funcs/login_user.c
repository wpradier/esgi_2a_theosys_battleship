#include "protoserv.h"


int		login_user(s_users *users, char *login, char *password) {
	int	i;
	int	quantity;

	i = 0;
	quantity = users->quantity;
	
	while (i < quantity) {
		if (!strncmp(users->logins[i], login, MAX_LOGIN_SIZE)
				&& !strncmp(users->passwords[i], password, MAX_PASSWORD_SIZE)) {
			return (1);
		}
		
		i++;
	}

	return (0);
}
