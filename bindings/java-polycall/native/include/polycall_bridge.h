#ifndef POLYCALL_BRIDGE_H
#define POLYCALL_BRIDGE_H

/**
 * LibPolyCall Trial v1 Java Bridge
 * Native interface for Java PolyCall binding
 */

#ifdef __cplusplus
extern "C" {
#endif

// Consistent bridge/JNI error codes.
#define POLYCALL_OK 0
#define POLYCALL_ERR_INVALID_ARGUMENT 1
#define POLYCALL_ERR_NOT_INITIALIZED 2
#define POLYCALL_ERR_CONNECTION_FAILED 3
#define POLYCALL_ERR_NOT_CONNECTED 4
#define POLYCALL_ERR_AUTH_FAILED 5
#define POLYCALL_ERR_NOT_AUTHENTICATED 6
#define POLYCALL_ERR_EXECUTION_FAILED 7
#define POLYCALL_ERR_NOT_IMPLEMENTED 100

// Function prototypes for polycall.exe FFI
int polycall_init(const char* host, int port);
int polycall_connect(void);
int polycall_authenticate(const char* credentials);
int polycall_execute(const char* operation, const char* params, char** result);
void polycall_disconnect(void);
void polycall_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif // POLYCALL_BRIDGE_H
