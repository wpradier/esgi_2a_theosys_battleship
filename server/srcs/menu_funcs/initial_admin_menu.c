#include "protoserv.h"

int		parse_value(char c) {
	if (c >= 'a' && c <= 'f') {
		return (c - 87); // hex
	}
	
	if (c >= '0' && c <= '9') {
		return (c - 48);
	}

	return (-1);
}

int		initial_admin_menu(int admin_ns, s_users *users, int serv_fd) {
	char	message[MSG_SIZE];
	char	response[MSG_SIZE];
	char	login[MAX_LOGIN_SIZE];
	char	password[MAX_PASSWORD_SIZE];
	char	*sep;
	int	w;
	int	h;
	int	x;
	int	y;
	s_board	board;


	while (1) {
		strncpy(message, "---ADMIN MENU---\n", MSG_SIZE);
		strncat(message, "Please input board size (<width>:<height>), must have at least 5:5, and at most f:f\n", MSG_SIZE);
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
		w = parse_value(response[0]);
		h = parse_value(response[2]);

		if (w == -1 || h == -1) {
			strncpy(message, "Unrecognized values\n", MSG_SIZE);
			if (!serv_send(admin_ns, INFO, message)) {
				return (0);
			}
			continue;
		}

		break;
	}

	snprintf(message, MSG_SIZE, "01%d%d", w, h); // 01 = board_size

	write(serv_fd, message, MSG_SIZE);
	while (1) {
		strncpy(message, "---ADMIN MENU---\n", MSG_SIZE);
		strncat(message, "0: Quit menu and start waiting for players\n");
		strncat(message, "1: Add player credentials\n");
		strncat(message, "2: Add boat\n");
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

			x = parse_value(response[0]);
			y = parse_value(response[2]);

			if (x == -1 || y == - 1) {
				if (!serv_send(admin_ns, INFO, "Invalid values\n")) {
					return (0);
				}
				continue;
			}

			snprintf(message, MSG_SIZE, "03%d%d", x, y);

			write(serv_fd, message, MSG_SIZE);

			continue;
		}
	}
}
