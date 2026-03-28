package org.obinexus.ffi;

/**
 * JNI interface for LibPolyCall native library.
 */
public final class LibPolyCallJNI {
    private static final String MOCK_MODE_PROPERTY = "polycall.ffi.mock";
    private static final boolean nativeLibraryLoaded;

    static {
        boolean loaded;
        try {
            System.loadLibrary("polycall_bridge_jni");
            loaded = true;
        } catch (UnsatisfiedLinkError e) {
            loaded = false;
        }
        nativeLibraryLoaded = loaded;
    }

    private LibPolyCallJNI() {}

    private static native int nativeInit(String host, int port);
    private static native int nativeConnect();
    private static native int nativeAuthenticate(String credentials);
    private static native int nativeExecute(String operation, String params);
    private static native void nativeDisconnect();
    private static native void nativeCleanup();

    public static boolean isNativeLibraryLoaded() {
        return nativeLibraryLoaded;
    }

    public static int init(String host, int port) {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY)) {
            return (host == null || host.isBlank() || port <= 0 || port > 65535)
                ? NativeErrorCode.ERR_INVALID_ARGUMENT
                : NativeErrorCode.OK;
        }
        if (!nativeLibraryLoaded) {
            return NativeErrorCode.ERR_NATIVE_LIBRARY_UNAVAILABLE;
        }
        return nativeInit(host, port);
    }

    public static int connect() {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY)) {
            return NativeErrorCode.OK;
        }
        if (!nativeLibraryLoaded) {
            return NativeErrorCode.ERR_NATIVE_LIBRARY_UNAVAILABLE;
        }
        return nativeConnect();
    }

    public static int authenticate(String credentials) {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY)) {
            return (credentials == null || credentials.isBlank())
                ? NativeErrorCode.ERR_INVALID_ARGUMENT
                : NativeErrorCode.OK;
        }
        if (!nativeLibraryLoaded) {
            return NativeErrorCode.ERR_NATIVE_LIBRARY_UNAVAILABLE;
        }
        return nativeAuthenticate(credentials);
    }

    public static int execute(String operation, String params) {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY)) {
            return (operation == null || operation.isBlank())
                ? NativeErrorCode.ERR_INVALID_ARGUMENT
                : NativeErrorCode.OK;
        }
        if (!nativeLibraryLoaded) {
            return NativeErrorCode.ERR_NATIVE_LIBRARY_UNAVAILABLE;
        }
        return nativeExecute(operation, params);
    }

    public static void disconnect() {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY) || !nativeLibraryLoaded) {
            return;
        }
        nativeDisconnect();
    }

    public static void cleanup() {
        if (Boolean.getBoolean(MOCK_MODE_PROPERTY) || !nativeLibraryLoaded) {
            return;
        }
        nativeCleanup();
    }
}
