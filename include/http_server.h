#ifndef IT_GAMES_HTTP_SERVER_H
#define IT_GAMES_HTTP_SERVER_H

#include <stdint.h>

#define IT_GAMES_HTTP_PORT 13370

// Start embedded HTTP server in background thread
int http_server_start(uint16_t port);

// Stop HTTP server
void http_server_stop(void);

#endif // IT_GAMES_HTTP_SERVER_H
