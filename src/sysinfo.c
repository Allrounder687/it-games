#include "sysinfo.h"
#include "notify.h"
#include <stdio.h>
#include <string.h>

int  sceKernelGetHwModelName(char *);
int  sceKernelGetHwSerialNumber(char *);
long sceKernelGetCpuFrequency(void);
int  sceKernelGetCpuTemperature(int *);
int  sceKernelGetSocSensorTemperature(int, int *);

int sysinfo_get(ps5_sysinfo_t *info) {
    if (!info) return -1;
    memset(info, 0, sizeof(*info));

    char buf[1000];

    // Model name
    buf[0] = '\0';
    if (sceKernelGetHwModelName(buf) == 0) {
        strncpy(info->model, buf, sizeof(info->model) - 1);
    } else {
        strncpy(info->model, "PlayStation 5", sizeof(info->model) - 1);
    }

    // Serial
    buf[0] = '\0';
    if (sceKernelGetHwSerialNumber(buf) == 0) {
        strncpy(info->serial, buf, sizeof(info->serial) - 1);
    }

    // Temperatures
    sceKernelGetCpuTemperature(&info->cpu_temp);
    sceKernelGetSocSensorTemperature(0, &info->soc_temp);

    // CPU Frequency
    info->cpu_freq_mhz = sceKernelGetCpuFrequency() / (1000 * 1000);

    return 0;
}

void sysinfo_print_and_notify(const ps5_sysinfo_t *info) {
    if (!info) return;

    printf("[IT Games] Model: %s (S/N: %s)\n", info->model, info->serial[0] ? info->serial : "N/A");
    printf("[IT Games] CPU Temp: %d C | SoC Temp: %d C | CPU Freq: %ld MHz\n",
           info->cpu_temp, info->soc_temp, info->cpu_freq_mhz);

    notify_send("🎮 IT Games Initialized!\nModel: %s\nCPU: %d°C | SoC: %d°C",
                info->model, info->cpu_temp, info->soc_temp);
}
