package org.obinexus.cli.commands;

import org.obinexus.core.ProtocolHandler;
import org.obinexus.core.TelemetryObserver;
import picocli.CommandLine;

import java.time.Instant;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.Callable;

@CommandLine.Command(
    name = "telemetry",
    description = "Monitor protocol telemetry",
    subcommands = {TelemetryCommand.ConsensusCommand.class}
)
public class TelemetryCommand implements Callable<Integer> {

    @CommandLine.Option(names = {"--observe"}, description = "Enable observation mode")
    private boolean observe;

    @CommandLine.Option(names = {"--duration"}, description = "Observation duration in seconds", defaultValue = "60")
    private int duration;

    @Override
    public Integer call() throws Exception {
        System.out.println("Protocol telemetry monitoring");
        if (observe) {
            System.out.println("Observing for " + duration + " seconds...");
            Thread.sleep(duration * 1000L);
        }
        System.out.println("Telemetry monitoring completed");
        return 0;
    }

    @CommandLine.Command(name = "consensus", description = "Emit trinary telemetry consensus event")
    static class ConsensusCommand implements Callable<Integer> {

        @CommandLine.Option(
            names = {"--state"},
            required = true,
            description = "Consensus state: ${COMPLETION-CANDIDATES}")
        private String state;

        @CommandLine.Option(names = {"--session"}, required = true, description = "Session identifier")
        private String sessionId;

        @CommandLine.Option(names = {"--ack-status"}, defaultValue = "pending", description = "Ack status")
        private String ackStatus;

        @CommandLine.Option(names = {"--persisted"}, defaultValue = "false", description = "Persistence status")
        private boolean persisted;

        @CommandLine.Option(names = {"--storage-key"}, defaultValue = "", description = "Persistence storage key")
        private String storageKey;

        @CommandLine.Option(names = {"--host"}, defaultValue = "localhost", description = "polycall.exe host")
        private String host;

        @CommandLine.Option(names = {"--port"}, defaultValue = "8084", description = "polycall.exe port")
        private int port;

        @Override
        public Integer call() {
            final String normalizedAckStatus = ackStatus.trim().toLowerCase();
            Map<String, Object> payload = new LinkedHashMap<>();
            payload.put("event_type", "telemetry.consensus");
            TelemetryObserver.ConsensusState consensusState = TelemetryObserver.ConsensusState.fromCliValue(state);
            payload.put("state", consensusState.name());
            payload.put("session_id", sessionId);
            payload.put("ack_status", normalizedAckStatus);
            payload.put("persisted", persisted);
            payload.put("storage_key", storageKey.isBlank() ? null : storageKey);
            payload.put("timestamp", Instant.now().toString());

            TelemetryObserver observer = new TelemetryObserver();
            observer.recordConsensus(consensusState, payload);

            ProtocolHandler protocolHandler = new ProtocolHandler(host, port);
            protocolHandler.emitTelemetryConsensus(payload);

            System.out.println("telemetry consensus payload:");
            payload.forEach((key, value) -> System.out.println("  " + key + ": " + value));
            return 0;
        }
    }
}
