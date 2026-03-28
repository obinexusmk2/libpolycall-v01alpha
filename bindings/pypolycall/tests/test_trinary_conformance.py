import asyncio

from pypolycall.core.protocol import ProtocolHandler, TrinaryDecision, MessageTypes


def test_maybe_round_trip_payload():
    handler = ProtocolHandler("localhost", 8084)
    message = asyncio.run(handler.send_consensus_echo(TrinaryDecision.MAYBE, correlation_id=42))

    assert message["type"] == MessageTypes.CONSENSUS_ECHO
    assert message["state"] == TrinaryDecision.MAYBE.value
    assert message["persisted"] is True
    assert handler.consensus_state == TrinaryDecision.MAYBE
    assert handler.maybe_persisted is True


def test_consensus_acknowledgment_semantics():
    handler = ProtocolHandler("localhost", 8084)
    ack = asyncio.run(handler.acknowledge_consensus(TrinaryDecision.MAYBE, correlation_id=99, persisted=True))

    assert ack["type"] == MessageTypes.CONSENSUS_ACK
    assert ack["correlation_id"] == 99
    assert ack["state"] == TrinaryDecision.MAYBE.value
    assert ack["persisted"] is True
