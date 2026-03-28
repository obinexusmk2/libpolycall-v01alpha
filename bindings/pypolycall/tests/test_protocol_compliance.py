"""Protocol compliance tests for the canonical PyPolyCall API."""

import asyncio
import pytest

from pypolycall.core import ProtocolBinding
from pypolycall.protocol.constants import DEFAULT_POLYCALL_HOST, DEFAULT_POLYCALL_PORT


class TestProtocolCompliance:
    """Test protocol compliance requirements."""

    def test_binding_uses_canonical_defaults(self):
        binding = ProtocolBinding()
        assert binding.polycall_host == DEFAULT_POLYCALL_HOST
        assert binding.polycall_port == DEFAULT_POLYCALL_PORT

    def test_no_bypass_allowed(self):
        binding = ProtocolBinding()
        assert not hasattr(binding, "direct_execute")
        assert not hasattr(binding, "bypass_protocol")

    def test_adapter_pattern_surface(self):
        binding = ProtocolBinding()
        assert hasattr(binding, "protocol_handler")
        assert binding.protocol_handler is None

    def test_connection_required_for_execute_operation(self):
        binding = ProtocolBinding()
        assert not binding.is_connected

        with pytest.raises(RuntimeError, match="authenticate"):
            asyncio.run(binding.execute_operation("test", {}))

    def test_backward_compat_execute_request_shim(self):
        binding = ProtocolBinding()
        asyncio.run(binding.connect())
        asyncio.run(binding.authenticate({"user": "test"}))

        with pytest.deprecated_call(match="execute_request"):
            result = asyncio.run(binding.execute_request("/test", {"ok": True}))

        assert result["operation"] == "test"
