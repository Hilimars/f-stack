APP = dpdk_custom_stack
SRCS = src/main.c

DPDK_PATH = /root/task10/dpdk-22.07
PKGCONF = pkg-config
CFLAGS += -O3 -Wall -Wextra $(shell pkg-config --cflags libdpdk)
LDFLAGS += $(shell pkg-config --libs libdpdk)

all:
	$(CC) $(CFLAGS) $(SRCS) -o build/$(APP) $(LDFLAGS)

clean:
	rm -f build/$(APP)


