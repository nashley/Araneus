#include "network.h"

int network_init(int port)
{
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET for IPv4 or AF_INET6 for IPv6, and SOCK_STREAM for reliable 2 way connection; see man sock
	if (sfd == -1)
		return network_error_requesting; // error creating socket

	bind_address.sin_port = htons(port);
	bind_address.sin_family = AF_INET; // IPv4
	bind_address.sin_addr.s_addr = INADDR_ANY; // Listen to any interface; can use inet_addr("127.0.0.1") to listen to specific interface instead

	int on = TRUE; // turn SO_REUSEADDR on so that addresses can be reused
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (bind(sfd, (struct sockaddr *) &bind_address, sizeof(bind_address)) == -1)
		return network_error_binding; // error binding socket


	if (listen(sfd, BACKLOG_LIMIT) == -1)
		return network_error_listening;
	return network_error_none; // no error
}

int network_process()
{
	struct sockaddr_storage remote_address; // the address of the remote connection
	int remote_sfd; // the socket file descriptor for communicating with the remote connection
	unsigned int addr_size = sizeof(remote_address); // the size of the information for the remote connection; changed by accept() to reflect the size used; never more than the value originally passed to accept()
	
	remote_sfd = accept(sfd, (struct sockaddr *)&remote_address, &addr_size);
	
	if(remote_sfd == -1)
		return network_error_accepting; // error accepting network connection

	void *message_buffer = malloc(MAX_HEADER_LENGTH * sizeof(char));
	int message_length = recv(remote_sfd, message_buffer, MAX_HEADER_LENGTH, 0); // receives a message from the remote: recv(socket, message, message_length, flags);
	if (message_length == -1) // check to see if actual message_length is larger than specified message_length
		return network_error_recieving; // error receiving from remote

	if (send(remote_sfd, message_buffer, message_length, 0) == -1) // send a message to the remote: send(socket, message, message_length, flags);
		return network_error_sending; // error sending to remote

	close(remote_sfd); // finished talking to remote; close connection
	free(message_buffer);

	return network_error_none;
}

int network_shutdown()
{
	return close(sfd); // close socket/stop listening for connections
}
