"""Protocol Binding Integration Tests."""

import asyncio

from pypolycall.core.binding import ProtocolBinding


class TestProtocolBindingIntegration:
    """Test protocol binding integration."""

    def test_binding_lifecycle(self):
        binding = ProtocolBinding()

        connected = asyncio.run(binding.connect())
        assert connected is True
        assert binding.is_connected is True

        auth_result = asyncio.run(binding.authenticate({"user": "test"}))
        assert auth_result is True
        assert binding.is_authenticated is True

        result = asyncio.run(binding.execute_operation("test_op", {"param": "value"}))
        assert result is not None
        assert result["status"] == "success"

        asyncio.run(binding.shutdown())
        assert binding.is_connected is False
        assert binding.is_authenticated is False

    def test_binding_initialization(self):
        binding = ProtocolBinding(polycall_host="test-host", polycall_port=9999)

        assert binding.polycall_host == "test-host"
        assert binding.polycall_port == 9999
        assert not binding.is_connected
        assert not binding.is_authenticated
