package org.obinexus.ffi;

/**
 * Native binding interface for polycall.exe FFI.
 *
 * Maintains lifecycle guards so callers get deterministic protocol errors
 * before JNI dispatch where possible.
 */
public class NativeBinding {
    private boolean initialized;
    private boolean connected;
    private boolean authenticated;

    public int init(String host, int port) {
        int code = LibPolyCallJNI.init(host, port);
        if (code == NativeErrorCode.OK) {
            initialized = true;
            connected = false;
            authenticated = false;
        }
        return code;
    }

    public int connect() {
        if (!initialized) {
            return NativeErrorCode.ERR_NOT_INITIALIZED;
        }
        int code = LibPolyCallJNI.connect();
        if (code == NativeErrorCode.OK) {
            connected = true;
            authenticated = false;
        }
        return code;
    }

    public int authenticate(String credentials) {
        if (!initialized) {
            return NativeErrorCode.ERR_NOT_INITIALIZED;
        }
        if (!connected) {
            return NativeErrorCode.ERR_NOT_CONNECTED;
        }

        int code = LibPolyCallJNI.authenticate(credentials);
        if (code == NativeErrorCode.OK) {
            authenticated = true;
        }
        return code;
    }

    public int execute(String operation, String params) {
        if (!initialized) {
            return NativeErrorCode.ERR_NOT_INITIALIZED;
        }
        if (!connected) {
            return NativeErrorCode.ERR_NOT_CONNECTED;
        }
        if (!authenticated) {
            return NativeErrorCode.ERR_NOT_AUTHENTICATED;
        }

        return LibPolyCallJNI.execute(operation, params);
    }

    public void disconnect() {
        LibPolyCallJNI.disconnect();
        connected = false;
        authenticated = false;
    }

    public void cleanup() {
        LibPolyCallJNI.cleanup();
        initialized = false;
        connected = false;
        authenticated = false;
    }

    public boolean isInitialized() {
        return initialized;
    }

    public boolean isConnected() {
        return connected;
    }

    public boolean isAuthenticated() {
        return authenticated;
    }
}
