#include "general.h"

void process_args(int *argc, char *argv[])
{
	port = PORT_ANY;
	for (int i=0; i < *argc; i++){
		switch (argv[i][0]){
			case '-':
				switch (argv[i][1]) {
					case 'h':
						print_help(argv[0]);
						break;
					case 'v':
						print_version();
						break;
					case 'p':
						port = extract_arg(argv, i, 2, argc); // if the argument doesn't have a number following "-p" (e.g., "-p letters"), then the port is assigned to 0 (i.e., any available port)
						break;
				}
				break;
		}
	}
}

void print_help(char *prog)
{
	printf("Usage: %s [-h] [-v] [-p port]\n", prog);
	printf("\t[ -h ]\t\tPrint this help message and exit\n");
	printf("\t[ -v ]\t\tPrint version information and exit\n");
	printf("\t[ -p port ]\tSpecify the port to listen on; the default is 80\n");
	exit(EXIT_SUCCESS);
}

void print_version()
{
	printf("Araneus Version %hu.%hu.%hu\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	exit(EXIT_SUCCESS);
}

/*
	This function is a little sketchy, but it mostly works
*/
int extract_arg(char *s[], int i, int j, int *argc)
{
	//printf("s[%d]: %s\n", i, s[i]);
	//printf("s[%d][%d]: %c\n", i, j, s[i][j]);
	while (s[i][j] != '\0'){ // go to the end of the string
		if (s[i][j] == ' '){ // if a space is encountered, the argument has been escaped (e.g., "-p\ 80")
			while (s[i][j] != '\0'){
				if (s[i][j] != ' ') // end of whitespace
					return strtoul(&s[i][j], NULL, 0);
				j++;
			}
			return PORT_ANY;
		} else { // argument immediately follows "-p"
			return strtoul(&s[i][j], NULL, 0);
		}
	}
	return ((i < (*argc - 1)) ? (extract_arg(s, i + 1, 0, argc)) : 0); // if there are more arguments, then the argument is in the argv after "-p"; otherwise, return 0
}
