#include "parse.h"

void parse_http(int message_length, char *message_buffer, off_t *message_length_return, char **message_buffer_return){

	if (message_buffer_return == NULL)  {
		state = STATE_STATUS_500;	// couldn't malloc() enough space for the pointer
	} else {
		state = STATE_REQUEST_METHOD;	// set the initial state
	}

	unsigned j = 0; // used for variable length fields that don't follow a certain pattern (e.g., the URI)
	// loop through each character and treat it differently according to the current state of the FSM
	for (unsigned i = 0; i < message_length; i++) {
		switch(state) {

		case STATE_REQUEST_METHOD:
			if ('G' == message_buffer[i])
				state = STATE_REQUEST_METHOD_G;
			else 
				state = STATE_STATUS_400;
		break;
		case STATE_REQUEST_METHOD_G:
			if ('E' == message_buffer[i])
				state = STATE_REQUEST_METHOD_GE;
			else 
				state = STATE_STATUS_400;
		break;
		case STATE_REQUEST_METHOD_GE:
			if ('T' == message_buffer[i])
				state = STATE_REQUEST_METHOD_GET;
			else 
				state = STATE_STATUS_400;
		break;
		case STATE_REQUEST_METHOD_GET:
			if (' ' == message_buffer[i])
				state = STATE_REQUEST_URI;
			else 
				state = STATE_STATUS_400;
		break;

		case STATE_REQUEST_URI:
			if ('*' == message_buffer[i]) {
				state = STATE_REQUEST_URI_STAR;
			} else {
				j = i; // save the current position in the message
				state = STATE_REQUEST_URI_LOOP;
			}
		break;

		case STATE_REQUEST_URI_LOOP:
			if (' ' == message_buffer[i]) {
				if (j == i) {
					state = STATE_STATUS_400; // the client sent a malformed request without a valid URI
				} else {
					unsigned file_path_length = i - j + 1; // string is not null terminated yet
					char *file_path = malloc((file_path_length) * sizeof(char));
					memcpy(file_path, message_buffer + j, file_path_length); // *message_buffer_return = message_buffer + j;

					memset(file_path + file_path_length - 1, '\0', 1); // null terminate the string so we can use it

					struct stat st;
					if (stat(file_path, &st) != 0) { // TODO if an error occurred (we're assuming the file was not found)
						fprintf(stderr, "Couldn't open %s\n", file_path);
						state = STATE_STATUS_404;
						free(file_path);
						continue;
					}
					*message_length_return = st.st_size;

					FILE *fp = fopen(file_path, "r");
					*message_buffer_return = malloc(*message_length_return * sizeof(char));
					if(*message_buffer_return == NULL) {
						state = STATE_STATUS_500;
						continue;
					}
					fread(*message_buffer_return, sizeof(char), *message_length_return, fp);
					fclose(fp);


					free(file_path);
					return;
					// state = STATE_REQUEST_HTTP_VERSION;
				}
			} else {
				continue; // move to the next character
			}
		break;

		case STATE_STATUS_400:
			*message_length_return = sizeof(MSG_400); // already null terminated
			*message_buffer_return = malloc(*message_length_return * sizeof(char));
			snprintf(*message_buffer_return, *message_length_return, "%s", MSG_400); // TODO necessary? if message_buffer_return is never malloc()ed, then we can just point it to message_buffer and free that
			
			return;
		break;

		case STATE_STATUS_404:
			*message_length_return = sizeof(MSG_404); // already null terminated
			*message_buffer_return = malloc(*message_length_return * sizeof(char));
			snprintf(*message_buffer_return, *message_length_return, "%s", MSG_404); // TODO necessary? if message_buffer_return is never malloc()ed, then we can just point it to message_buffer and free that
			
			return;
		break;

		case STATE_STATUS_500:
			*message_length_return = sizeof(MSG_500); // already null terminated
			*message_buffer_return = malloc(*message_length_return * sizeof(char));
			snprintf(*message_buffer_return, *message_length_return, "%s", MSG_500); // TODO necessary? if message_buffer_return is never malloc()ed, then we can just point it to message_buffer and free that
			
			return;
		break;

		default:
			fprintf(stderr, "\nAn invalid state was encountered. This should never happen.\n");
			free(*message_buffer_return);
			free(message_buffer_return);
			exit(EXIT_FAILURE);
		break;
		}
	}
}