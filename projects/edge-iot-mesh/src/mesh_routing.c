#include <stdio.h>

void route_message(const char* from, const char* to) {
    printf("route %s -> %s via libpolycall edge mesh\n", from, to);
}
