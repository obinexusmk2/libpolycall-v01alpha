CC := gcc
CFLAGS := -Wall -Wextra -I../../libpolycall-v1/include -Isrc -fPIC -DUSE_CANONICAL_IR=1
LDFLAGS := ../../libpolycall-v1/lib/libpolycall.a -lssl -lcrypto -pthread

TARGET := edge_node
SRCS := src/edge_node.c src/trigger_update.c src/mesh_routing.c src/conflict_resolution.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
