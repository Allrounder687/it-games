#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct ps5_display_info {
    int handle;
    int width;
    int height;
    int is_active;
} ps5_display_info_t;

int display_subsystem_init(ps5_display_info_t *disp);
void display_subsystem_fini(ps5_display_info_t *disp);

#endif /* DISPLAY_H */
