CC := gcc
CFLAGS := -Wall -Wextra -I../../libpolycall-v1/include -Isrc -fPIC -DUSE_CANONICAL_IR=1
LDFLAGS := ../../libpolycall-v1/lib/libpolycall.a -lssl -lcrypto -pthread

TARGET := telemetry_server
SRCS := src/telemetry_server.c src/cir_object.c src/huffman_avl.c src/state_machine.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
