#include "http_server.h"
#include "ui_html.h"
#include "notify.h"
#include "sysinfo.h"
#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int g_server_fd = -1;
static pthread_t g_server_thread;
static volatile int g_running = 0;

static void handle_client(int client_fd) {
    char buffer[2048];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        close(client_fd);
        return;
    }
    buffer[bytes_read] = '\0';

    char method[16], path[256];
    sscanf(buffer, "%15s %255s", method, path);

    // 1. API: /api/sysinfo
    if (strncmp(path, "/api/sysinfo", 12) == 0) {
        ps5_sysinfo_t info;
        sysinfo_get(&info);

        ps5_user_profile_t user;
        user_get_active_profile(&user);

        char json[512];
        snprintf(json, sizeof(json),
            "{\"status\":\"ok\",\"gamertag\":\"%s\",\"soc_temp\":%d,\"cpu_temp\":%d,\"cpu_mhz\":%lu,\"model\":\"%s\"}",
            user.username[0] ? user.username : "Player",
            info.soc_temp,
            info.cpu_temp,
            info.cpu_freq_mhz,
            info.model);

        char resp[1024];
        snprintf(resp, sizeof(resp),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: %zu\r\n\r\n%s",
            strlen(json), json);

        write(client_fd, resp, strlen(resp));
    }
    // 2. API: /api/notify
    else if (strncmp(path, "/api/notify", 11) == 0) {
        char *msg_param = strstr(path, "msg=");
        char msg_buf[256] = "🎮 IT Games Alert";
        if (msg_param) {
            strncpy(msg_buf, msg_param + 4, sizeof(msg_buf) - 1);
            // Replace %20 with space
            for (char *p = msg_buf; *p; p++) {
                if (p[0] == '%' && p[1] == '2' && p[2] == '0') {
                    *p = ' ';
                    memmove(p + 1, p + 3, strlen(p + 3) + 1);
                }
            }
        }
        notify_send("%s", msg_buf);

        const char *resp = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 15\r\n\r\n{\"status\":\"ok\"}";
        write(client_fd, resp, strlen(resp));
    }
    // 3. API: /api/download
    else if (strncmp(path, "/api/download", 13) == 0) {
        notify_send("📥 Downloading Game/App to /data/homebrew/...");
        const char *resp = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 15\r\n\r\n{\"status\":\"ok\"}";
        write(client_fd, resp, strlen(resp));
    }
    // 4. Default: Serve 4K Web UI
    else {
        size_t html_len = strlen(g_ui_html);
        char header[256];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n\r\n",
            html_len);

        write(client_fd, header, strlen(header));
        write(client_fd, g_ui_html, html_len);
    }

    close(client_fd);
}

static void *server_worker(void *arg) {
    while (g_running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(g_server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd >= 0) {
            handle_client(client_fd);
        }
    }
    return NULL;
}

int http_server_start(uint16_t port) {
    g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_fd < 0) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(g_server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(g_server_fd);
        g_server_fd = -1;
        return -1;
    }

    if (listen(g_server_fd, 10) < 0) {
        perror("listen");
        close(g_server_fd);
        g_server_fd = -1;
        return -1;
    }

    g_running = 1;
    pthread_create(&g_server_thread, NULL, server_worker, NULL);
    printf("[HTTP Server] Listening on port %u\n", port);
    return 0;
}

void http_server_stop(void) {
    g_running = 0;
    if (g_server_fd >= 0) {
        close(g_server_fd);
        g_server_fd = -1;
    }
    pthread_join(g_server_thread, NULL);
}
