"""Deprecated compatibility namespace for legacy `pypolycall.pypolycall` imports."""

import warnings

warnings.warn(
    "`pypolycall.pypolycall` is deprecated and will be removed in a future release. "
    "Import from `pypolycall` instead.",
    DeprecationWarning,
    stacklevel=2,
)

from .. import *  # noqa: F401,F403
