#ifndef USER_H
#define USER_H

#include <stdint.h>

#define SCE_USER_SERVICE_MAX_USER_NAME_LENGTH 16

typedef struct ps5_user_profile {
    int32_t user_id;
    char username[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
} ps5_user_profile_t;

int user_subsystem_init(void);
int user_get_active_profile(ps5_user_profile_t *profile);
void user_subsystem_fini(void);

#endif /* USER_H */
