#ifndef SYSINFO_H
#define SYSINFO_H

typedef struct ps5_sysinfo {
    char model[128];
    char serial[128];
    int cpu_temp;
    int soc_temp;
    long cpu_freq_mhz;
} ps5_sysinfo_t;

int sysinfo_get(ps5_sysinfo_t *info);
void sysinfo_print_and_notify(const ps5_sysinfo_t *info);

#endif /* SYSINFO_H */
