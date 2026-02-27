#include "zero_trust_auth.h"

#include <openssl/sha.h>
#include <string.h>
#include <time.h>

static uint16_t crc16(const uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; ++b) {
            crc = (crc & 0x8000) ? (uint16_t)((crc << 1) ^ 0x1021) : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

void polycall_header_seed(PolyCall_Header* header, const uint8_t* payload, size_t payload_len, uint8_t node_type) {
    memset(header, 0, sizeof(*header));
    SHA256(payload, payload_len, header->seed);
    header->session = (uint64_t)time(NULL);
    header->sequence = 1U;
    header->checksum = crc16(payload, payload_len);
    header->node_type = node_type;
}

bool polycall_header_validate(const PolyCall_Header* header, const uint8_t* payload, size_t payload_len) {
    uint8_t expected[32];
    SHA256(payload, payload_len, expected);
    return memcmp(header->seed, expected, sizeof(expected)) == 0 && header->checksum == crc16(payload, payload_len);
}
