#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "it_games.h"
#include "notify.h"
#include "sysinfo.h"
#include "user.h"
#include "log.h"

int main(void) {
    // 1. Initialize Log Subsystem on PS5 SSD (/data/it_games/debug.log)
    log_init();
    log_info("Starting %s v%s", IT_GAMES_APP_TITLE, IT_GAMES_VERSION);

    // 2. Initialize User Profile
    ps5_user_profile_t profile;
    memset(&profile, 0, sizeof(profile));
    int user_ret = user_subsystem_init();
    log_info("UserService Init ret=%d", user_ret);

    int prof_ret = user_get_active_profile(&profile);
    log_info("UserProfile Query ret=%d (User ID: %d, Username: '%s')",
             prof_ret, profile.user_id, profile.username);

    // 3. Query Hardware & Thermal Telemetry
    ps5_sysinfo_t sysinfo;
    memset(&sysinfo, 0, sizeof(sysinfo));
    int sys_ret = sysinfo_get(&sysinfo);
    log_info("SysInfo Query ret=%d (Model: %s, CPU: %d°C, SoC: %d°C)",
             sys_ret, sysinfo.model, sysinfo.cpu_temp, sysinfo.soc_temp);

    // 4. Send Official PlayStation 5 Pop-up Notification
    notify_send("🎮 IT Games Online!\nUser: %s | PS5 %s | CPU: %d°C",
        profile.username[0] ? profile.username : "Player",
        sysinfo.model[0] ? sysinfo.model : "Console",
        sysinfo.cpu_temp);

    log_info("Main payload thread sleeping to allow notification delivery...");
    sleep(5);

    user_subsystem_fini();
    log_info("IT Games payload execution finished successfully.");
    return 0;
}
