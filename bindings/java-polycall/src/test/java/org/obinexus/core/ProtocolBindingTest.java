package org.obinexus.core;

import org.junit.jupiter.api.Test;
import org.obinexus.testsupport.FixtureLoader;

import java.io.IOException;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class ProtocolBindingTest {

    @Test
    public void testProtocolBindingInitialization() {
        ProtocolBinding binding = new ProtocolBinding("localhost", 8084);
        assertNotNull(binding);
        assertFalse(binding.isConnected());
        assertFalse(binding.isAuthenticated());
    }

    @Test
    public void testConsensusHappyFixtureContract() throws IOException {
        Map<String, Object> fixture = FixtureLoader.load("fixtures/consensus/happy_yes.json");

        assertEquals("1.0", fixture.get("schema_version"));
        assertEquals("consensus", fixture.get("domain"));
        assertEquals("YES", fixture.get("decision"));
        assertEquals("approved", fixture.get("status"));
        assertTrue(fixture.containsKey("input"));
        assertTrue(fixture.containsKey("expected"));
    }

    @Test
    public void testTelemetryRejectionFixtureContract() throws IOException {
        Map<String, Object> fixture = FixtureLoader.load("fixtures/telemetry/rejection_no.json");

        assertEquals("1.0", fixture.get("schema_version"));
        assertEquals("telemetry", fixture.get("domain"));
        assertEquals("NO", fixture.get("decision"));
        assertEquals("rejected", fixture.get("status"));

        Map<String, Object> expected = (Map<String, Object>) fixture.get("expected");
        assertEquals(Boolean.FALSE, expected.get("ack_received"));
        assertEquals("HITL_REJECTED", expected.get("error_code"));
    }
}
