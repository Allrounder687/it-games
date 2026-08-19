#ifndef NOTIFY_H
#define NOTIFY_H

#include <stddef.h>

typedef struct notify_request {
    char useless1[45];
    char message[3075];
} notify_request_t;

int notify_send(const char *fmt, ...);

#endif /* NOTIFY_H */
