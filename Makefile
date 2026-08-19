PS5_HOST ?= 192.168.0.208
PS5_PORT ?= 9021
PS5_FTP_PORT ?= 2121

ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/make/toolchain.mk
else
    $(error PS5_PAYLOAD_SDK is undefined)
endif

ELF := it_games.elf
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)
APP_DIR := app_pkg/ITGA00001-app0

CFLAGS := -Wall -Werror -Iinclude
LIBS := -lkernel_sys -lkernel -lSceSystemService -lSceUserService -lSceVideoOut -lSceNet -lSceSsl -lSceHttp2

all: $(ELF)

$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(ELF) $(OBJS)
	rm -rf $(APP_DIR)

app: $(ELF)
	python scripts/build_app_assets.py $(APP_DIR)
	cp $(ELF) $(APP_DIR)/eboot.bin

deploy-app: app
	python scripts/deploy_app.py $(PS5_HOST) $(PS5_FTP_PORT) $(APP_DIR)

test: $(ELF)
	$(PS5_DEPLOY) -h $(PS5_HOST) -p $(PS5_PORT) $^
