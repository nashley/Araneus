#include "general.h"
#include "network.h"
#include "signal.h" // for capturing ctrl+c

static volatile int running = 1;

int main(int argc, char *argv[]);
void signal_handler_int(int signal);
