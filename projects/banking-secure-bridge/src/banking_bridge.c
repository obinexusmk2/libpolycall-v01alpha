#include "telemetry_logger.h"
#include "zero_trust_auth.h"
#include "polycall.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    const char* request = "GET_ACCOUNT:10001";
    PolyCall_Header header;
    polycall_header_seed(&header, (const uint8_t*)request, strlen(request), 0x01);

    telemetry_log_transition("Parse", &header, "Received COBOL-shaped request");
    if (!polycall_header_validate(&header, (const uint8_t*)request, strlen(request))) {
        telemetry_log_transition("Validate", &header, "GUID validation failed");
        return 1;
    }

    telemetry_log_transition("Marshal", &header, "Canonical payload ready");
    printf("banking-bridge: approved request %s\n", request);
    printf("libpolycall version: %s\n", polycall_get_version());
    return 0;
}
