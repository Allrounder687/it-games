#include "user.h"
#include <stdio.h>
#include <string.h>

int sceUserServiceInitialize(void*);
int sceUserServiceTerminate(void);
int sceUserServiceGetInitialUser(int32_t*);
int sceUserServiceGetUserName(int32_t, char*, size_t);

static int g_user_service_initialized = 0;

int user_subsystem_init(void) {
    if (g_user_service_initialized) return 0;

    int ret = sceUserServiceInitialize(NULL);
    if (ret == 0) {
        g_user_service_initialized = 1;
        return 0;
    }
    return ret;
}

int user_get_active_profile(ps5_user_profile_t *profile) {
    if (!profile) return -1;
    memset(profile, 0, sizeof(*profile));

    if (!g_user_service_initialized) {
        if (user_subsystem_init() != 0) {
            strncpy(profile->username, "Player", sizeof(profile->username) - 1);
            return -1;
        }
    }

    int32_t uid = 0;
    if (sceUserServiceGetInitialUser(&uid) == 0) {
        profile->user_id = uid;
        if (sceUserServiceGetUserName(uid, profile->username, sizeof(profile->username)) != 0) {
            strncpy(profile->username, "PlayStation User", sizeof(profile->username) - 1);
        }
    } else {
        profile->user_id = -1;
        strncpy(profile->username, "PlayStation User", sizeof(profile->username) - 1);
    }

    return 0;
}

void user_subsystem_fini(void) {
    if (g_user_service_initialized) {
        sceUserServiceTerminate();
        g_user_service_initialized = 0;
    }
}
