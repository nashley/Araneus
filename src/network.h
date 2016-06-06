#include <sys/socket.h> // for opening and connecting to network sockets
#include <sys/types.h> // not required for Linux; may be required for BSD or embedded systems
#include <netinet/ip.h> // for IPPROTO_*, the socket type
#include <unistd.h> // for closing (socket) file descriptors
#include <stdio.h> // for printing output
#include <stdlib.h> // for malloc/free

#define BACKLOG_LIMIT 10 // the maximum number of connections sitting in the listen queue (before they're accepted)
#define MAX_HEADER_LENGTH 8190 // the maximum length of the header; see http://stackoverflow.com/a/6160643

#define TRUE 1

enum {
	// generic
	network_error_none,

	// init
	network_error_requesting,
	network_error_binding,
	network_error_listening,

	//process
	network_error_accepting,
	network_error_recieving,
	network_error_sending
} network_error;

// The socket file descriptor
int sfd;

// The address to bind to
struct sockaddr_in bind_address;

int network_init(int port);

int network_process();

int network_shutdown();
