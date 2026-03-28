package org.obinexus.integration;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.obinexus.ffi.NativeBinding;
import org.obinexus.ffi.NativeErrorCode;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class RuntimeConnectionTest {
    @BeforeEach
    void enableMockRuntime() {
        System.setProperty("polycall.ffi.mock", "true");
    }

    @AfterEach
    void clearMockRuntime() {
        System.clearProperty("polycall.ffi.mock");
    }

    @Test
    public void testLifecycleInitConnectAuthExecuteSuccess() {
        NativeBinding binding = new NativeBinding();

        assertEquals(NativeErrorCode.OK, binding.init("localhost", 8084));
        assertTrue(binding.isInitialized());

        assertEquals(NativeErrorCode.OK, binding.connect());
        assertTrue(binding.isConnected());

        assertEquals(NativeErrorCode.OK, binding.authenticate("token:test"));
        assertTrue(binding.isAuthenticated());

        assertEquals(NativeErrorCode.OK, binding.execute("system.status", "{\"include_metrics\":true}"));

        binding.disconnect();
        assertFalse(binding.isConnected());
        assertFalse(binding.isAuthenticated());

        binding.cleanup();
        assertFalse(binding.isInitialized());
    }

    @Test
    public void testLifecycleOrderFailuresAreDeterministic() {
        NativeBinding binding = new NativeBinding();

        assertEquals(NativeErrorCode.ERR_NOT_INITIALIZED, binding.connect());
        assertEquals(NativeErrorCode.ERR_NOT_INITIALIZED, binding.authenticate("token:test"));
        assertEquals(NativeErrorCode.ERR_NOT_INITIALIZED, binding.execute("system.status", "{}"));

        assertEquals(NativeErrorCode.OK, binding.init("localhost", 8084));

        assertEquals(NativeErrorCode.ERR_NOT_CONNECTED, binding.authenticate("token:test"));
        assertEquals(NativeErrorCode.ERR_NOT_CONNECTED, binding.execute("system.status", "{}"));

        assertEquals(NativeErrorCode.OK, binding.connect());
        assertEquals(NativeErrorCode.ERR_NOT_AUTHENTICATED, binding.execute("system.status", "{}"));
    }

    @Test
    public void testLifecycleInvalidInputsFailDeterministically() {
        NativeBinding binding = new NativeBinding();

        assertEquals(NativeErrorCode.ERR_INVALID_ARGUMENT, binding.init("", 8084));
        assertEquals(NativeErrorCode.ERR_INVALID_ARGUMENT, binding.init("localhost", -1));

        assertEquals(NativeErrorCode.OK, binding.init("localhost", 8084));
        assertEquals(NativeErrorCode.OK, binding.connect());

        assertEquals(NativeErrorCode.ERR_INVALID_ARGUMENT, binding.authenticate(""));

        assertEquals(NativeErrorCode.OK, binding.authenticate("token:test"));
        assertEquals(NativeErrorCode.ERR_INVALID_ARGUMENT, binding.execute("", "{}"));
    }
}
