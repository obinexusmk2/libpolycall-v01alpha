"""Protocol compliance sanity checks for authoritative binding module path."""

from pypolycall.core.binding import ProtocolBinding


def test_protocol_binding_sane_defaults() -> None:
    binding = ProtocolBinding()
    assert binding.polycall_host == "localhost"
    assert binding.polycall_port == 8084
    assert binding.__class__.__module__ == "pypolycall.core.binding"


def test_protocol_binding_has_no_bypass_surface() -> None:
    binding = ProtocolBinding()
    assert not hasattr(binding, "direct_execute")
    assert not hasattr(binding, "bypass_protocol")
