#include "polycall.h"
#include "polycall_protocol.h"
#include "polycall_state_machine.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static void test_maybe_round_trip_persistence(void) {
    uint8_t encoded[4] = {0};
    size_t encoded_length = 0;
    polycall_decision_t decoded = POLYCALL_DECISION_NO;

    assert(polycall_protocol_encode_decision(
        POLYCALL_DECISION_MAYBE,
        encoded,
        sizeof(encoded),
        &encoded_length
    ));
    assert(encoded_length == 1);
    assert(encoded[0] == (uint8_t)POLYCALL_DECISION_MAYBE);

    assert(polycall_protocol_decode_decision(encoded, encoded_length, &decoded));
    assert(decoded == POLYCALL_DECISION_MAYBE);
}

static void test_explicit_transition_from_maybe_to_binary(void) {
    polycall_context_t ctx = NULL;
    PolyCall_StateMachine* sm = NULL;

    polycall_config_t config = {0};
    assert(polycall_init_with_config(&ctx, &config) == POLYCALL_SUCCESS);
    assert(polycall_sm_create_with_integrity(ctx, &sm, NULL) == POLYCALL_SM_SUCCESS);

    assert(polycall_sm_add_state(sm, "CHECK", NULL, NULL, false) == POLYCALL_SM_SUCCESS);

    polycall_decision_t decision = POLYCALL_DECISION_NO;
    bool unresolved = false;

    assert(polycall_sm_get_state_decision(sm, 0, &decision) == POLYCALL_SM_SUCCESS);
    assert(decision == POLYCALL_DECISION_MAYBE);

    assert(polycall_sm_set_state_decision(sm, 0, POLYCALL_DECISION_YES) == POLYCALL_SM_SUCCESS);
    assert(polycall_sm_get_state_decision(sm, 0, &decision) == POLYCALL_SM_SUCCESS);
    assert(decision == POLYCALL_DECISION_YES);

    assert(polycall_sm_state_is_unresolved(sm, 0, &unresolved) == POLYCALL_SM_SUCCESS);
    assert(!unresolved);

    assert(polycall_sm_set_state_decision(sm, 0, POLYCALL_DECISION_NO) == POLYCALL_SM_SUCCESS);
    assert(polycall_sm_get_state_decision(sm, 0, &decision) == POLYCALL_SM_SUCCESS);
    assert(decision == POLYCALL_DECISION_NO);

    polycall_sm_destroy(sm);
    polycall_cleanup(ctx);
}

static void test_forced_binary_collapse_policy(void) {
    polycall_context_t ctx = NULL;
    PolyCall_StateMachine* sm = NULL;

    polycall_config_t config = {0};
    assert(polycall_init_with_config(&ctx, &config) == POLYCALL_SUCCESS);
    assert(polycall_sm_create_with_integrity(ctx, &sm, NULL) == POLYCALL_SM_SUCCESS);

    assert(polycall_sm_add_state(sm, "A", NULL, NULL, false) == POLYCALL_SM_SUCCESS);
    assert(polycall_sm_add_state(sm, "B", NULL, NULL, false) == POLYCALL_SM_SUCCESS);

    assert(polycall_sm_force_binary_collapse(sm, POLYCALL_DECISION_YES) ==
           POLYCALL_SM_ERROR_INVALID_TRANSITION);

    assert(polycall_sm_set_binary_collapse_policy(sm, true) == POLYCALL_SM_SUCCESS);
    assert(polycall_sm_force_binary_collapse(sm, POLYCALL_DECISION_YES) == POLYCALL_SM_SUCCESS);

    polycall_decision_t d0 = POLYCALL_DECISION_MAYBE;
    polycall_decision_t d1 = POLYCALL_DECISION_MAYBE;
    assert(polycall_sm_get_state_decision(sm, 0, &d0) == POLYCALL_SM_SUCCESS);
    assert(polycall_sm_get_state_decision(sm, 1, &d1) == POLYCALL_SM_SUCCESS);
    assert(d0 == POLYCALL_DECISION_YES);
    assert(d1 == POLYCALL_DECISION_YES);

    polycall_sm_destroy(sm);
    polycall_cleanup(ctx);
}

int main(void) {
    test_maybe_round_trip_persistence();
    test_explicit_transition_from_maybe_to_binary();
    test_forced_binary_collapse_policy();

    printf("trinary decision tests passed\n");
    return 0;
}
