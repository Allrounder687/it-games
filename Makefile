PS5_HOST ?= 192.168.0.208
PS5_PORT ?= 9021
PS5_FTP_PORT ?= 2121

ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/make/toolchain.mk
else
    $(error PS5_PAYLOAD_SDK is undefined)
endif

ELF := it_games.elf
SRCS := src/main.c
OBJS := $(SRCS:.c=.o)
APP_DIR := app_pkg/PPSA99901-app0

CFLAGS := -Wall -Werror -Iinclude
LIBS := -lkernel_sys -lSceSystemService

all: $(ELF)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(ELF) $(OBJS)

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
