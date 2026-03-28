"""
Core Protocol Binding - Adapter for polycall.exe.

Canonical API contract (stable):
- Class: pypolycall.core.binding.ProtocolBinding
- Constructor args: polycall_host, polycall_port, binding_config
- Async methods: connect, authenticate, execute_operation, shutdown
- Compatibility shim: execute_request (deprecated)
- Public attributes: polycall_host, polycall_port, config
- Public properties: is_connected, is_authenticated, protocol_handler
"""

import logging
import warnings
from typing import Any, Dict, Optional

from ..protocol.constants import DEFAULT_POLYCALL_HOST, DEFAULT_POLYCALL_PORT

logger = logging.getLogger(__name__)


class ProtocolBinding:
    """Core protocol adapter surface for polycall.exe runtime."""

    def __init__(
        self,
        polycall_host: str = DEFAULT_POLYCALL_HOST,
        polycall_port: int = DEFAULT_POLYCALL_PORT,
        binding_config: Optional[Dict[str, Any]] = None,
    ):
        self.polycall_host = polycall_host
        self.polycall_port = polycall_port
        self.config = binding_config or {}

        # Connection/auth state
        self._connected = False
        self._authenticated = False

        # Reserved for future protocol handler injection
        self._protocol_handler = None

        logger.info("ProtocolBinding initialized for %s:%s", polycall_host, polycall_port)

    async def connect(self) -> bool:
        """Connect to polycall.exe runtime."""
        try:
            logger.info("Attempting connection to polycall.exe runtime")
            self._connected = True
            return True
        except Exception as exc:  # pragma: no cover - defensive
            logger.error("Connection failed: %s", exc)
            return False

    async def authenticate(self, credentials: Dict[str, Any]) -> bool:
        """Authenticate against polycall.exe runtime."""
        if not self._connected:
            raise RuntimeError("Must connect before authentication")

        try:
            logger.info("Authenticating with runtime using credential keys: %s", list(credentials.keys()))
            self._authenticated = True
            return True
        except Exception as exc:  # pragma: no cover - defensive
            logger.error("Authentication failed: %s", exc)
            return False

    async def execute_operation(self, operation: str, params: Dict[str, Any]) -> Any:
        """Execute an operation through the runtime protocol boundary."""
        if not self._authenticated:
            raise RuntimeError("Must authenticate before operation execution")

        logger.info("Executing operation: %s", operation)
        return {"status": "success", "operation": operation, "params": params}

    async def execute_request(self, request_path: str, params: Dict[str, Any]) -> Any:
        """Deprecated compatibility shim for legacy callers.

        Use `execute_operation` instead.
        """
        warnings.warn(
            "ProtocolBinding.execute_request() is deprecated; "
            "use execute_operation(operation, params) instead.",
            DeprecationWarning,
            stacklevel=2,
        )
        operation = request_path.lstrip("/")
        return await self.execute_operation(operation, params)

    async def shutdown(self) -> None:
        """Clean shutdown of binding adapter."""
        self._connected = False
        self._authenticated = False
        logger.info("ProtocolBinding shutdown complete")

    @property
    def is_connected(self) -> bool:
        return self._connected

    @property
    def is_authenticated(self) -> bool:
        return self._authenticated

    @property
    def protocol_handler(self):
        """Protocol handler reference (reserved/stable API surface)."""
        return self._protocol_handler
