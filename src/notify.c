#include "notify.h"
#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int sceKernelSendNotificationRequest(int device, SceNotificationRequest *req, size_t size, int blocking);

int notify_subsystem_init(void) {
    return 0;
}

void notify_send(const char *fmt, ...) {
    SceNotificationRequest noti;
    memset(&noti, 0, sizeof(noti));

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(noti.message, sizeof(noti.message), fmt, ap);
    va_end(ap);

    noti.type = 0;
    noti.use_icon_image_uri = 1;
    noti.target_id = -1;
    strncpy(noti.uri, "cxml://psnotification/tex_icon_system", sizeof(noti.uri) - 1);

    int ret = sceKernelSendNotificationRequest(0, &noti, sizeof(noti), 0);
    log_info("Notification Sent (ret=%d): \"%s\"", ret, noti.message);
}

void notify_subsystem_fini(void) {
}
