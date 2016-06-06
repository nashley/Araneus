/*
	Made by Nick Ashley
	Released under the MIT license
*/

#include "main.h"

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler_int);
	
	// Process the command line arguments
	process_args(&argc, argv);

	// Indicate which port the server will be requesting
	printf("Asking for ");
	if (port == 0)
		printf("any available port");
	else
		printf("port %d", port);
	printf("...\n");

	switch (network_init(port)) {
		case network_error_none:
			printf("Network initialization was successful.\n");
			break;
		case network_error_requesting:
			perror("There was an error while requesting a network socket");
			break;
		case network_error_binding:
			perror("There was an error while binding to the network socket");
			break;
		case network_error_listening:
			perror("There was an error while listening on the network socket");
			break;
		default:
			perror("There was an error while initializing the network");
			break;
	}

	printf("Handling connections...\n");
	while (running) {
		switch(network_process()) {
			case network_error_none:
				// success; no need to tell the user
				break;
			default:
				perror("There was an error while handling the network connection");
				break;
		}
	}

	switch(network_shutdown()) {
		case network_error_none:
			printf("Network shutdown was successful.\n");
			break;
		default:
			perror("There was an error while shutting down the network");
			break;
	}
}

void signal_handler_int(int signal) {
	running = 0;
	//network_shutdown(); //TODO remove after debug; replace with flush_buffer or something and then call network_shutdown; also remove running variable
}