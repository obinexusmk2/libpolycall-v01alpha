#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc((size_t)size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, (size_t)size, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}

static bool contains_key_value(const char* payload, const char* key, const char* value) {
    return strstr(payload, key) != NULL && strstr(payload, value) != NULL;
}

static void assert_required_contract(const char* payload) {
    assert(strstr(payload, "schema_version") != NULL);
    assert(strstr(payload, "scenario_id") != NULL);
    assert(strstr(payload, "domain") != NULL);
    assert(strstr(payload, "decision") != NULL);
    assert(strstr(payload, "status") != NULL);
    assert(strstr(payload, "includes") != NULL);
    assert(strstr(payload, "input") != NULL);
    assert(strstr(payload, "expected") != NULL);
}

void test_hitl_happy_yes_fixture(void) {
    char* payload = read_file("../../fixtures/consensus/happy_yes.json");
    assert(payload != NULL);

    assert_required_contract(payload);
    assert(contains_key_value(payload, "decision", "YES"));

    free(payload);
}

void test_hitl_rejection_no_fixture(void) {
    char* payload = read_file("../../fixtures/telemetry/rejection_no.json");
    assert(payload != NULL);

    assert_required_contract(payload);
    assert(contains_key_value(payload, "decision", "NO"));

    free(payload);
}

void test_hitl_deferred_maybe_fixture(void) {
    char* payload = read_file("../../fixtures/daemon/deferred_maybe.yaml");
    assert(payload != NULL);

    assert_required_contract(payload);
    assert(contains_key_value(payload, "decision", "MAYBE"));
    assert(strstr(payload, "timeout_ms") != NULL);
    assert(strstr(payload, "retry_count") != NULL);
    assert(strstr(payload, "ack_required") != NULL);

    free(payload);
}
