#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "it_games.h"
#include "notify.h"
#include "sysinfo.h"
#include "net.h"
#include "user.h"
#include "display.h"
#include "http_server.h"

int sceSystemServiceLaunchWebBrowser(const char *uri, void *reserved);

int main(void) {
    printf("==========================================\n");
    printf("     🎮 %s v%s     \n", IT_GAMES_APP_TITLE, IT_GAMES_VERSION);
    printf("==========================================\n");

    // 1. Initialize User Service & Telemetry
    ps5_user_profile_t user_profile;
    user_subsystem_init();
    user_get_active_profile(&user_profile);

    ps5_sysinfo_t sysinfo;
    sysinfo_get(&sysinfo);

    // 2. Start Embedded HTTP & UI Server (Port 13370)
    http_server_start(IT_GAMES_HTTP_PORT);

    // 3. Send Notification to TV
    notify_send("🎮 Welcome %s to IT Games!\nModel: %s | CPU: %d°C | SoC: %d°C",
        user_profile.username[0] ? user_profile.username : "Player",
        sysinfo.model[0] ? sysinfo.model : "PS5",
        sysinfo.cpu_temp,
        sysinfo.soc_temp);

    // 4. Launch Native Full-Screen 4K PS5 Browser on TV
    char url[64];
    snprintf(url, sizeof(url), "http://127.0.0.1:%d", IT_GAMES_HTTP_PORT);
    printf("[IT Games] Launching 4K Browser UI -> %s\n", url);
    
    sceSystemServiceLaunchWebBrowser(url, NULL);

    // 5. Keep background engine alive to service REST API & downloads
    // Run for 30 minutes or until closed
    for (int i = 0; i < 1800; i++) {
        sleep(1);
    }

    http_server_stop();
    user_subsystem_fini();

    return 0;
}
