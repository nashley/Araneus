#include "general.h"

void process_args(int *argc, char *argv[])
{
	port = PORT_HTTP;
	for (int i=0; i < *argc; i++){
		if (argv[i][0]=='-'){
			if (argv[i][1]=='h')
				print_help(argv[0]);
			if (argv[i][1]=='v')
				print_version();
			if (argv[i][1]=='p')
				//printf("\nPort: %s\n", extract_arg(argv, i, 2)); // start off by trying the same argv (but starting with the character after 'p')
				port = extract_arg(argv, i, 2, argc); // if the argument doesn't have a number following "-p" (e.g., "-p letters"), then the port is assigned to 0 (i.e., any available port)
		}
	}
}

void print_help(char *prog)
{
	printf("Usage: %s [-h] [-v]\n", prog);
	printf("\t-h\tPrint this help message and exit\n");
	printf("\t-v\tPrint version information and exit\n");
	exit(0);
}

void print_version()
{
	printf("Araneus Version %hu.%hu\n", VERSION_MAJOR, VERSION_MINOR);
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
			return 0;
		} else { // argument immediately follows "-p"
			return strtoul(&s[i][j], NULL, 0);
		}
	}
	return ((i < (*argc - 1)) ? (extract_arg(s, i + 1, 0, argc)) : 0); // if there are more arguments, then the argument is in the argv after "-p"; otherwise, return 0
}
