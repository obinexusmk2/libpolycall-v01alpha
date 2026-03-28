#include "polycall_protocol.h"
#include <assert.h>

void test_consensus_payload_layout(void) {
    polycall_consensus_payload_t payload = {
        .decision = POLYCALL_DECISION_MAYBE,
        .correlation_id = 42,
        .persisted = 1
    };

    assert(payload.decision == POLYCALL_DECISION_MAYBE);
    assert(payload.correlation_id == 42);
    assert(payload.persisted == 1);
}

void test_trinary_decision_values(void) {
    assert(POLYCALL_DECISION_NO == 0x00);
    assert(POLYCALL_DECISION_MAYBE == 0x01);
    assert(POLYCALL_DECISION_YES == 0x02);
}
