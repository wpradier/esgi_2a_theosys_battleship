#include "protoserv.h"

int		validate_value_min(char c, char min) {
	if ((c >= 'a' && c <= 'f') || (c >= min && c <= '9')) {
		return (1);
	}

	return (0);
}

int		initial_admin_menu(int admin_ns, int serv_fd) {
	char	message[MSG_SIZE];
	char	response[MSG_SIZE];
	char	login[MAX_LOGIN_SIZE];
	char	password[MAX_PASSWORD_SIZE];
	char	*sep;
	char	w;
	char	h;
	char	x;
	char	y;


	while (1) {
		strncpy(message, "---ADMIN MENU---\n", MSG_SIZE);
		strncat(message, "Please input board size (<width>:<height>), must have at least 5:5, and at most f:f\n", MSG_SIZE - 1);
		if (!serv_send(admin_ns, GET_INPUT, message)) {
			return (0);
		}

		if (recv(admin_ns, response, MSG_SIZE, 0) == -1) {
			return (0);
		}

		if (!(sep = strchr(response, ':'))) {
			strncpy(message, "Missing ':'\n", MSG_SIZE);
			if (!serv_send(admin_ns, INFO, message)) {
				return (0);
			}
			continue;
		}

		w = response[0];
		h = response[2];

		if (!validate_value_min(w, '5') || !validate_value_min(h, '5')) {
			strncpy(message, "Invalid values\n", MSG_SIZE);
			if (!serv_send(admin_ns, INFO, message)) {
				return (0);
			}
			continue;
		}

		break;
	}

	snprintf(message, MSG_SIZE, "01%c%c", w, h); // 01 = board_size

	write(serv_fd, message, MSG_SIZE);
	while (1) {
		strncpy(message, "---ADMIN MENU---\n", MSG_SIZE);
		strncat(message, "0: Quit menu and start waiting for players\n", MSG_SIZE - 1);
		strncat(message, "1: Add player credentials\n", MSG_SIZE - 1);
		strncat(message, "2: Add boat\n", MSG_SIZE - 1);
		if (!serv_send(admin_ns, GET_INPUT, message)) {
			return (0);
		}

		if (recv(admin_ns, response, MSG_SIZE, 0) == -1) {
			return (0);
		}

		if (response[0] == '0') {
			snprintf(message, MSG_SIZE, "00"); // end admin menu phase
			write(serv_fd, message, MSG_SIZE);
			return (1);
		}
		
		if (response[0] == '1') {
			if (!serv_send(admin_ns, GET_INPUT, "Login:\n")) {
				return (0);
			}
			if (recv(admin_ns, response, MSG_SIZE, 0) == -1) {
				return (0);
			}
			strncpy(login, response, MAX_LOGIN_SIZE);

			if (!serv_send(admin_ns, GET_INPUT, "Password:\n")) {
				return (0);
			}
			if (recv(admin_ns, response, MSG_SIZE, 0) == -1) {
				return (0);
			}
			strncpy(password, response, MAX_LOGIN_SIZE);

			strncpy(message, CREDENTIALS, MSG_SIZE);
			strncat(message, login, MSG_SIZE - 1);
			strncat(message, ":", MSG_SIZE - 1);
			strncat(message, password, MSG_SIZE - 1);

			write(serv_fd, message, MSG_SIZE);

			if (!serv_send(admin_ns, INFO, "User created!\n")) {
				return (0);
			}
			continue;
		}

		if (response[0] == '2') {
			if (!serv_send(admin_ns, GET_INPUT, "<x>:<y>\n")) {
				return (0);
			}
			if (recv(admin_ns, response, MSG_SIZE, 0) == -1) {
				return (0);
			}

			if (!(sep = strchr(response, ':'))) {
				strncpy(message, "Missing ':'\n", MSG_SIZE);
				if (!serv_send(admin_ns, INFO, message)) {
					return (0);
				}
				continue;
			}

			x = response[0];
			y = response[2];

			if (!validate_value_min(x, '0') || !validate_value_min(y, '0')) {
				if (!serv_send(admin_ns, INFO, "Invalid values\n")) {
					return (0);
				}
				continue;
			}

			snprintf(message, MSG_SIZE, "03%c%c", x, y);

			write(serv_fd, message, MSG_SIZE);

			continue;
		}
	}
}
