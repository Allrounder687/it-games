#include "it_games.h"
#include "notify.h"
#include "sysinfo.h"
#include "user.h"
#include "display.h"
#include "net.h"

int main(void) {
    printf("==========================================\n");
    printf("     🎮 %s v%s     \n", IT_GAMES_APP_TITLE, IT_GAMES_VERSION);
    printf("==========================================\n");

    // Early notification to test execution
    notify_send("🎮 IT Games Booting...");

    // 1. Initialize PlayStation User Service
    ps5_user_profile_t user_profile;
    user_subsystem_init();
    user_get_active_profile(&user_profile);
    printf("[IT Games] Active User: %s (UID: 0x%08X)\n", user_profile.username, user_profile.user_id);

    // 2. Query Display & Video Output
    ps5_display_info_t display_info;
    display_subsystem_init(&display_info);
    printf("[IT Games] Display Resolution: %dx%d (Active: %s)\n",
           display_info.width, display_info.height,
           display_info.is_active ? "Yes" : "Fallback");

    // 3. Gather Hardware Telemetry
    ps5_sysinfo_t sys_info;
    sysinfo_get(&sys_info);
    printf("[IT Games] Model: %s (S/N: %s)\n", sys_info.model, sys_info.serial[0] ? sys_info.serial : "N/A");
    printf("[IT Games] CPU: %d C | SoC: %d C | Clock: %ld MHz\n",
           sys_info.cpu_temp, sys_info.soc_temp, sys_info.cpu_freq_mhz);

    // 4. Send Welcome Pop-Up Notification to the TV
    notify_send("🎮 Welcome %s to IT Games!\nModel: %s (%dx%d)\nCPU: %d°C | SoC: %d°C",
                user_profile.username, sys_info.model,
                display_info.width, display_info.height,
                sys_info.cpu_temp, sys_info.soc_temp);

    // 5. Initialize Network Subsystem
    printf("[IT Games] Initializing Network Subsystem...\n");
    if (net_subsystem_init("ITGames-PS5/" IT_GAMES_VERSION) == 0) {
        printf("[IT Games] Network Online (HTTP/2 + SSL ready).\n");
        notify_send("🌐 IT Games Network Online!\nReady for game catalog sync.");
    } else {
        printf("[IT Games] Network Subsystem skipped or offline.\n");
    }

    // 6. Cleanup subsystems
    net_subsystem_fini();
    display_subsystem_fini(&display_info);
    user_subsystem_fini();

    printf("[IT Games] Core systems executed cleanly. Ready.\n");
    
    // Prevent immediate exit so the notification daemon can process the IPC message
    sleep(3);
    
    return 0;
}
