PS5_HOST ?= ps5
PS5_PORT ?= 9021

ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/make/toolchain.mk
else
    $(error PS5_PAYLOAD_SDK is undefined)
endif

ELF := it_games.elf
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -Wall -Werror -Iinclude
LIBS := -lkernel_sys -lkernel -lSceSystemService -lSceUserService -lSceVideoOut -lSceNet -lSceSsl -lSceHttp2

all: $(ELF)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(ELF) $(OBJS)

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
