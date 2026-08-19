#ifndef IT_GAMES_LOG_H
#define IT_GAMES_LOG_H

#include <stdio.h>
#include <stdarg.h>

#define IT_GAMES_LOG_DIR "/data/it_games"
#define IT_GAMES_LOG_FILE "/data/it_games/debug.log"

void log_init(void);
void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif // IT_GAMES_LOG_H
