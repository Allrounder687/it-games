#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "it_games.h"
#include "notify.h"
#include "sysinfo.h"
#include "user.h"
#include "http_server.h"

int sceSystemServiceLaunchWebBrowser(const char *uri, void *reserved);

int main(void) {
    // 1. Send immediate toast on TV to confirm execution
    notify_send("🎮 IT Games Engine Active!\nStarting 4K Interactive UI...");

    // 2. Initialize UserService & Telemetry
    ps5_user_profile_t user_profile;
    user_subsystem_init();
    user_get_active_profile(&user_profile);

    ps5_sysinfo_t sysinfo;
    sysinfo_get(&sysinfo);

    // 3. Start Embedded HTTP server on port 13370
    http_server_start(IT_GAMES_HTTP_PORT);

    // 4. Send detailed telemetry notification
    notify_send("🎮 Welcome %s!\nPS5 %s | CPU: %d°C | SoC: %d°C",
        user_profile.username[0] ? user_profile.username : "Player",
        sysinfo.model[0] ? sysinfo.model : "PS5",
        sysinfo.cpu_temp,
        sysinfo.soc_temp);

    // 5. Open Web Browser on TV
    char url[128];
    snprintf(url, sizeof(url), "http://127.0.0.1:%d", IT_GAMES_HTTP_PORT);
    printf("[IT Games] Launching browser to %s\n", url);
    sceSystemServiceLaunchWebBrowser(url, NULL);

    // Also sleep 5s to keep process alive and service requests
    for (int i = 0; i < 300; i++) {
        sleep(1);
    }

    http_server_stop();
    user_subsystem_fini();

    return 0;
}
