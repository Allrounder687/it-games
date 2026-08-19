PS5_HOST ?= 192.168.0.208
PS5_PORT ?= 9021
PS5_FTP_PORT ?= 2121

ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/make/toolchain.mk
else
    $(error PS5_PAYLOAD_SDK is undefined)
endif

ELF := it_games.elf
SRCS := src/main.c src/log.c src/notify.c src/sysinfo.c src/user.c
OBJS := $(SRCS:.c=.o)

CFLAGS := -Wall -Werror -O2 -Iinclude
LIBS := -lkernel_sys -lkernel -lSceUserService

all: $(ELF)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	strip --strip-all $@

clean:
	rm -f $(ELF) $(OBJS)

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
