package org.obinexus.ffi;

/**
 * Shared error codes between native bridge and Java JNI layer.
 */
public final class NativeErrorCode {
    private NativeErrorCode() {}

    public static final int OK = 0;
    public static final int ERR_INVALID_ARGUMENT = 1;
    public static final int ERR_NOT_INITIALIZED = 2;
    public static final int ERR_CONNECTION_FAILED = 3;
    public static final int ERR_NOT_CONNECTED = 4;
    public static final int ERR_AUTH_FAILED = 5;
    public static final int ERR_NOT_AUTHENTICATED = 6;
    public static final int ERR_EXECUTION_FAILED = 7;
    public static final int ERR_NOT_IMPLEMENTED = 100;
    public static final int ERR_NATIVE_LIBRARY_UNAVAILABLE = 101;

    public static String describe(int code) {
        return switch (code) {
            case OK -> "OK";
            case ERR_INVALID_ARGUMENT -> "Invalid argument";
            case ERR_NOT_INITIALIZED -> "Runtime not initialized";
            case ERR_CONNECTION_FAILED -> "Connection failed";
            case ERR_NOT_CONNECTED -> "Not connected";
            case ERR_AUTH_FAILED -> "Authentication failed";
            case ERR_NOT_AUTHENTICATED -> "Not authenticated";
            case ERR_EXECUTION_FAILED -> "Execution failed";
            case ERR_NOT_IMPLEMENTED -> "Not implemented";
            case ERR_NATIVE_LIBRARY_UNAVAILABLE -> "Native library unavailable";
            default -> "Unknown native error";
        };
    }
}
