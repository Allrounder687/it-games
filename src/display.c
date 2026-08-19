#include "display.h"
#include <stdio.h>
#include <string.h>

int sceVideoOutOpen(int, int, int, const void*);
int sceVideoOutClose(int);

typedef struct {
    int width;
    int height;
    int fps;
    int format;
} SceVideoOutResolutionStatus;

int sceVideoOutGetResolutionStatus(int, SceVideoOutResolutionStatus*);

int display_subsystem_init(ps5_display_info_t *disp) {
    if (!disp) return -1;
    memset(disp, 0, sizeof(*disp));
    disp->handle = -1;

    // Bus 0xFF (main display), Type 0, Index 0
    // int handle = sceVideoOutOpen(0xFF, 0, 0, NULL); // REMOVED: This can crash if Payload Manager owns the screen
    int handle = -1;
    if (handle < 0) {
        // Fallback standard 1080p profile
        disp->width = 1920;
        disp->height = 1080;
        disp->is_active = 0;
        return -1;
    }

    disp->handle = handle;
    disp->is_active = 1;

    SceVideoOutResolutionStatus status;
    memset(&status, 0, sizeof(status));

    if (sceVideoOutGetResolutionStatus(handle, &status) == 0 && status.width > 0 && status.height > 0) {
        disp->width = status.width;
        disp->height = status.height;
    } else {
        disp->width = 1920;
        disp->height = 1080;
    }

    return 0;
}

void display_subsystem_fini(ps5_display_info_t *disp) {
    if (disp && disp->handle >= 0) {
        sceVideoOutClose(disp->handle);
        disp->handle = -1;
        disp->is_active = 0;
    }
}
