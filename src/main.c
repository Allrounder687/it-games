#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct notify_request {
    char useless1[45];
    char message[3075];
} notify_request_t;

int sceKernelSendNotificationRequest(int, notify_request_t*, size_t, int);
int sceSystemServiceLaunchWebBrowser(const char *uri, void *reserved);

int main(void) {
    notify_request_t req;
    bzero(&req, sizeof(req));
    strncpy(req.message, "🎮 IT Games Engine Online!\nWelcome to PS5 Homebrew!", sizeof(req.message) - 1);
    sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);

    sleep(1);

    // Launch web browser
    sceSystemServiceLaunchWebBrowser("https://google.com", NULL);

    sleep(3);
    return 0;
}
