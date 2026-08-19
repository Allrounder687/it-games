#include "notify.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int sceKernelSendNotificationRequest(int, notify_request_t*, size_t, int);

int notify_send(const char *fmt, ...) {
    notify_request_t req;
    va_list args;

    bzero(&req, sizeof(req));

    va_start(args, fmt);
    vsnprintf(req.message, sizeof(req.message), fmt, args);
    va_end(args);

    return sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}
