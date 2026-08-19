#include "it_games.h"
#include "notify.h"
#include "sysinfo.h"
#include "net.h"

int main(void) {
    printf("[IT Games] Starting IT Games v%s...\n", IT_GAMES_VERSION);

    // 1. Gather hardware telemetry
    ps5_sysinfo_t info;
    sysinfo_get(&info);
    sysinfo_print_and_notify(&info);

    // 2. Initialize networking subsystem
    printf("[IT Games] Initializing Network Subsystem...\n");
    if (net_subsystem_init("ITGames-PS5/" IT_GAMES_VERSION) == 0) {
        printf("[IT Games] Network Subsystem initialized successfully.\n");
        notify_send("🌐 IT Games Network Online!");
    } else {
        printf("[IT Games] Network Subsystem initialization skipped or failed.\n");
    }

    // 3. Cleanup network on exit
    net_subsystem_fini();

    printf("[IT Games] Payload executed cleanly.\n");
    return 0;
}
