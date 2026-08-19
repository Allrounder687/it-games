#include "net.h"
#include <stdio.h>
#include <string.h>

int sceNetInit(void);
int sceNetPoolCreate(const char*, int, int);
int sceNetPoolDestroy(int);

int sceSslInit(size_t);
int sceSslTerm(int);

int sceHttp2Init(int, int, size_t, int);
int sceHttp2Term(int);

int sceHttp2CreateTemplate(int, const char*, int, int);
int sceHttp2DeleteTemplate(int);

int sceHttp2CreateRequestWithURL(int, const char*, const char*, uint64_t);
int sceHttp2DeleteRequest(int);

int sceHttp2SendRequest(int, const void*, size_t);
int sceHttp2GetStatusCode(int, int*);
int sceHttp2ReadData(int, void *, size_t);

static int g_net_pool_id   = -1;
static int g_ssl_ctx_id    = -1;
static int g_http_ctx_id   = -1;
static int g_template_id   = -1;

int net_subsystem_init(const char *user_agent) {
    if (sceNetInit()) {
        // May already be initialized by Payload Manager. We'll continue anyway.
    }

    g_net_pool_id = sceNetPoolCreate("it_games_net", 64 * 1024, 0);
    if (g_net_pool_id < 0) {
        perror("sceNetPoolCreate");
        return -1;
    }

    g_ssl_ctx_id = sceSslInit(256 * 1024);
    if (g_ssl_ctx_id < 0) {
        perror("sceSslInit");
        return -1;
    }

    g_http_ctx_id = sceHttp2Init(g_net_pool_id, g_ssl_ctx_id, 256 * 1024, 1);
    if (g_http_ctx_id < 0) {
        perror("sceHttp2Init");
        return -1;
    }

    const char *agent = user_agent ? user_agent : "ITGames-PS5/0.2.0";
    g_template_id = sceHttp2CreateTemplate(g_http_ctx_id, agent, 3, 1);
    if (g_template_id < 0) {
        perror("sceHttp2CreateTemplate");
        return -1;
    }

    return 0;
}

int net_http_get(const char *url, char *out_buf, size_t max_len, int *status_code) {
    if (g_template_id < 0 || !url) {
        return -1;
    }

    int req_id = sceHttp2CreateRequestWithURL(g_template_id, "GET", url, 0);
    if (req_id < 0) {
        perror("sceHttp2CreateRequestWithURL");
        return -1;
    }

    if (sceHttp2SendRequest(req_id, NULL, 0) != 0) {
        perror("sceHttp2SendRequest");
        sceHttp2DeleteRequest(req_id);
        return -1;
    }

    int status = 0;
    if (sceHttp2GetStatusCode(req_id, &status) == 0 && status_code) {
        *status_code = status;
    }

    int total_read = 0;
    if (out_buf && max_len > 0) {
        char chunk[1024];
        int bytes = 0;
        out_buf[0] = '\0';

        while ((bytes = sceHttp2ReadData(req_id, chunk, sizeof(chunk) - 1)) > 0) {
            if ((size_t)(total_read + bytes) < max_len) {
                memcpy(out_buf + total_read, chunk, bytes);
                total_read += bytes;
                out_buf[total_read] = '\0';
            } else {
                break;
            }
        }
    }

    sceHttp2DeleteRequest(req_id);
    return total_read;
}

void net_subsystem_fini(void) {
    if (g_template_id >= 0) {
        sceHttp2DeleteTemplate(g_template_id);
        g_template_id = -1;
    }
    if (g_http_ctx_id >= 0) {
        sceHttp2Term(g_http_ctx_id);
        g_http_ctx_id = -1;
    }
    if (g_ssl_ctx_id >= 0) {
        sceSslTerm(g_ssl_ctx_id);
        g_ssl_ctx_id = -1;
    }
    if (g_net_pool_id >= 0) {
        sceNetPoolDestroy(g_net_pool_id);
        g_net_pool_id = -1;
    }
}
