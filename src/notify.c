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
    // Use vsnprintf carefully. If it crashes, we'll know.
    vsnprintf(req.message, sizeof(req.message) - 1, fmt, args);
    va_end(args);
    
    // Ensure null termination
    req.message[sizeof(req.message) - 1] = '\0';

    return sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}
