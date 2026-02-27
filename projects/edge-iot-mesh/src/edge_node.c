#include "polycall_security.h"

#include <stdio.h>

void route_message(const char* from, const char* to);
void trigger_if_changed(const char* key, const char* new_value);

int main(void) {
    PolyCall_Header header = {0};
    header.node_type = 0x03;
    route_message("US", "UK");
    trigger_if_changed("temperature", "22.4");
    printf("edge node ready node_type=%u\n", header.node_type);
    return 0;
}
