package org.obinexus.integration;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.obinexus.testsupport.FixtureLoader;

import java.io.IOException;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class RuntimeConnectionTest {

    @Test
    public void testDaemonDeferredFixtureTimeoutRetryAckContract() throws IOException {
        Map<String, Object> fixture = FixtureLoader.load("fixtures/daemon/deferred_maybe.yaml");

        assertEquals("1.0", fixture.get("schema_version"));
        assertEquals("daemon", fixture.get("domain"));
        assertEquals("MAYBE", fixture.get("decision"));
        assertEquals("deferred", fixture.get("status"));

        Map<String, Object> expected = (Map<String, Object>) fixture.get("expected");
        assertEquals(60000, expected.get("timeout_ms"));
        assertEquals(4, expected.get("retry_count"));
        assertEquals(Boolean.TRUE, expected.get("ack_required"));
    }

    @Test
    public void testTelemetryDeferredFixtureTimeoutRetryAckContract() throws IOException {
        Map<String, Object> fixture = FixtureLoader.load("fixtures/telemetry/deferred_maybe.yaml");

        assertEquals("MAYBE", fixture.get("decision"));

        Map<String, Object> expected = (Map<String, Object>) fixture.get("expected");
        assertEquals(30000, expected.get("timeout_ms"));
        assertEquals(3, expected.get("retry_count"));
        assertEquals(Boolean.TRUE, expected.get("ack_required"));
    }
}
