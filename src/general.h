#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	Taken from semver.org:

	Given a version number MAJOR.MINOR.PATCH, increment the:

		MAJOR version when you make incompatible API changes,
		MINOR version when you add functionality in a backwards-compatible manner, and
		PATCH version when you make backwards-compatible bug fixes.
	
	Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.
*/
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 1

#define PORT_ANY 0

short unsigned port;

void process_args(int *argc, char *argv[]);

void print_help(char *prog);
void print_version();

int extract_arg(char *s[], int i, int j, int *argc);
