#include "protoserv.h"

/*
 * Returns 0 if credentials are incorrect, 1 if admin, 2 if player.
 */

int		login_user(s_credentials credentials, char *login, char *password) {
	char	*logins;
	char	*passwords;
	int	*quantity;
	char	curr_login[MAX_LOGIN_SIZE];
	char	curr_password[MAX_PASSWORD_SIZE];
	int	i;

	logins = shmat(credentials.logins_id, 0, IPC_NOWAIT);
	passwords = shmat(credentials.passwords_id, 0, IPC_NOWAIT);
	quantity = shmat(credentials.quantity_id, 0, IPC_NOWAIT);

	i = 0;
	while (i < (*quantity)) {
		memcpy(curr_login, logins + login_i(i), MAX_LOGIN_SIZE);
		memcpy(curr_password, passwords + password_i(i), MAX_PASSWORD_SIZE);
		printf("CURR LOGIN: %s\n", curr_login);
		printf("CURR PASSWORD: %s\n", curr_password);

		if (!strncmp(curr_login, login, MAX_LOGIN_SIZE)
				&& !strncmp(curr_password, password, MAX_PASSWORD_SIZE)) {
			if (i == 0) { // Admin user is the first in the SHM
				return (1);
			}
			
			return (2);
		}

		i++;
	}

	return (0);
}
