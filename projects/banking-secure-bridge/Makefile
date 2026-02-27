CC := gcc
CFLAGS := -Wall -Wextra -I../../libpolycall-v1/include -Isrc -fPIC -DUSE_CANONICAL_IR=1
LDFLAGS := ../../libpolycall-v1/lib/libpolycall.a -lssl -lcrypto -pthread

TARGET := banking_bridge
SRCS := src/banking_bridge.c src/zero_trust_auth.c src/telemetry_logger.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
