#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

void log_init(void) {
    mkdir(IT_GAMES_LOG_DIR, 0777);
    FILE *fp = fopen(IT_GAMES_LOG_FILE, "a");
    if (fp) {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_str[32];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(fp, "\n\n=== [IT GAMES BOOT %s] ===\n", time_str);
        fflush(fp);
        fclose(fp);
    }
}

void log_info(const char *fmt, ...) {
    FILE *fp = fopen(IT_GAMES_LOG_FILE, "a");
    if (!fp) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

    fprintf(fp, "[%s] [INFO] ", time_str);
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}

void log_error(const char *fmt, ...) {
    FILE *fp = fopen(IT_GAMES_LOG_FILE, "a");
    if (!fp) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

    fprintf(fp, "[%s] [ERROR] ", time_str);
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}
