#include "parse.h"

void parse_http(int *message_length, char *message_buffer, int *message_length_return, char *message_buffer_return){

	state = STATE_REQUEST_METHOD; // set the initial state

	unsigned j = 0; // used for variable length fields that don't follow a certain pattern (e.g., the URI)
	// loop through each character and treat it differently according to the current state of the FSM
	for (unsigned i = 0; i < *message_length; i++) { // TODO convert `( message_buffer[i]); i++;` to `message_ptr = (char *) message_buffer; message_ptr++;` if free() still works?
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
			if(' ' == message_buffer[i]) {
				if(j == i) {
					state = STATE_STATUS_400; // the client sent a malformed request without a valid URI
				} else {
					// state = STATE_REQUEST_HTTP_VERSION;
					*message_length_return = i - j;
					message_buffer_return = message_buffer[j];
					i = *message_length;
				}
			} else {
				
			}
		break;

		case STATE_STATUS_400:
			*message_length_return = sizeof(INVALID_REQUEST_MESSAGE);
			message_buffer_return = malloc(*message_length_return * sizeof(char));
			snprintf(message_buffer_return, *message_length_return, "%s", INVALID_REQUEST_MESSAGE);
			i = *message_length; // break out of the for loop
		break;

		default:
			exit(EXIT_FAILURE);
		break;
		}
	}
}