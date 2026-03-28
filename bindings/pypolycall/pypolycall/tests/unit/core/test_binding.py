"""Legacy in-package test retained as a smoke check only."""

from pypolycall.core.binding import ProtocolBinding


def test_binding_import_path_is_authoritative() -> None:
    binding = ProtocolBinding()
    assert binding.__class__.__module__ == "pypolycall.core.binding"
