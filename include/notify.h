#ifndef NOTIFY_H
#define NOTIFY_H

#include <stddef.h>

/* Full PS5 notification struct (0xC30 bytes) as used in ps5-app-dumper & ShadowMountPlus */
typedef struct {
    int type;                // 0x00
    int req_id;              // 0x04
    int priority;            // 0x08
    int msg_id;              // 0x0C
    int target_id;           // 0x10 (-1 for broadcast)
    int user_id;             // 0x14
    int unk1;                // 0x18
    int unk2;                // 0x1C
    int app_id;              // 0x20
    int error_num;           // 0x24
    int unk3;                // 0x28
    char use_icon_image_uri; // 0x2C (1 to enable icon URI)
    char message[1024];      // 0x2D
    char uri[1024];          // 0x42D
    char unkstr[1024];       // 0x82D
} SceNotificationRequest;

int notify_subsystem_init(void);
void notify_send(const char *fmt, ...);
void notify_subsystem_fini(void);

#endif /* NOTIFY_H */
