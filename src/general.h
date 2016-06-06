#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 0

#define PORT_HTTP 80

short unsigned port;

void process_args(int *argc, char *argv[]);

void print_help(char *prog);
void print_version();

int extract_arg(char *s[], int i, int j, int *argc);
