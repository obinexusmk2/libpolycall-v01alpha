"""
PyPolyCall Core Layer
Protocol binding components for polycall.exe runtime
"""

from .binding import ProtocolBinding

# Conditional imports for graceful degradation
try:
    from .protocol import ProtocolHandler, MessageTypes, StateTransitions, TrinaryDecision
except ImportError:
    ProtocolHandler = None
    MessageTypes = None
    StateTransitions = None
    TrinaryDecision = None

try:
    from .telemetry import TelemetryObserver, MetricsCollector
except ImportError:
    TelemetryObserver = None
    MetricsCollector = None

__all__ = [
    "ProtocolBinding",
    "ProtocolHandler", 
    "MessageTypes",
    "StateTransitions",
    "TrinaryDecision",
    "TelemetryObserver",
    "MetricsCollector"
]
