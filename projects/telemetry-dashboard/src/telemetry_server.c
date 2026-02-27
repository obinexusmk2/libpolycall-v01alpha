#include "cir_object.h"
#include "polycall.h"

#include <stdio.h>

void huffman_avl_note(void);
void snapshot_state(const char* guid, const char* state);

int main(void) {
    CIR_Value sample;
    cir_assign_float(&sample, 20.5, "guid-demo-001");
    huffman_avl_note();
    snapshot_state(sample.guid, "Broadcast");
    printf("telemetry-server online using libpolycall %s\n", polycall_get_version());
    return 0;
}
