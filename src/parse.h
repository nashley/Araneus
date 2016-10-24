#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INVALID_REQUEST_MESSAGE "HTTP/1.1 400 Bad Request"

//#define INVALID_REQUEST_MESSAGE "HTTP/1.1 400 Bad Request\nContent-Type: text/html; charset=UTF-8\nContent-Length: 1555\nDate: Mon, 24 Oct 2016 13:54:16 GMT\n\n<!DOCTYPE html>\n<html lang=en>\n  <meta charset=utf-8>\n  <meta name=viewport content=\"initial-scale=1, minimum-scale=1, width=device-width\">\n  <title>Error 400 (Bad Request)!!1</title>\n  <style>\n    *{margin:0;padding:0}html,code{font:15px/22px arial,sans-serif}html{background:#fff;color:#222;padding:15px}body{margin:7% auto 0;max-width:390px;min-height:180px;padding:30px 0 15px}* > body{background:url(//www.google.com/images/errors/robot.png) 100% 5px no-repeat;padding-right:205px}p{margin:11px 0 22px;overflow:hidden}ins{color:#777;text-decoration:none}a img{border:0}@media screen and (max-width:772px){body{background:none;margin-top:0;max-width:none;padding-right:0}}#logo{background:url(//www.google.com/images/branding/googlelogo/1x/googlelogo_color_150x54dp.png) no-repeat;margin-left:-5px}@media only screen and (min-resolution:192dpi){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat 0% 0%/100% 100%;-moz-border-image:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) 0}}@media only screen and (-webkit-min-device-pixel-ratio:2){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat;-webkit-background-size:100% 100%}}#logo{display:inline-block;height:54px;width:150px}\n  </style>\n  <a href=//www.google.com/><span id=logo aria-label=Google></span></a>\n  <p><b>400.</b> <ins>That’s an error.</ins>\n  <p>Your client has issued a malformed or illegal request.  <ins>That’s all we know.</ins>"

enum state_enum {
	STATE_REQUEST_METHOD,
	STATE_REQUEST_METHOD_G,
	STATE_REQUEST_METHOD_GE,
	STATE_REQUEST_METHOD_GET,

	STATE_REQUEST_URI,
	STATE_REQUEST_URI_STAR,
	STATE_REQUEST_URI_LOOP,

	STATE_REQUEST_HTTP_VERSION,

	STATE_STATUS_400
} state;

void parse_http(int *message_length, char *message_buffer, int *message_length_return, char *message_buffer_return); // TODO make the message_length not a pointer