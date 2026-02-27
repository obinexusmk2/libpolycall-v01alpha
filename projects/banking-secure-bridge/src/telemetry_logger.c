#include "telemetry_logger.h"

#include <stdio.h>
#include <time.h>

void telemetry_log_transition(const char* state, const PolyCall_Header* header, const char* detail) {
    const time_t now = time(NULL);
    printf("[%ld] state=%s session=%llu sequence=%u node=%u detail=%s\n",
           (long)now,
           state,
           (unsigned long long)header->session,
           header->sequence,
           header->node_type,
           detail);
}
