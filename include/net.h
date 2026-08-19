#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <stdint.h>

int net_subsystem_init(const char *user_agent);
int net_http_get(const char *url, char *out_buf, size_t max_len, int *status_code);
void net_subsystem_fini(void);

#endif /* NET_H */
