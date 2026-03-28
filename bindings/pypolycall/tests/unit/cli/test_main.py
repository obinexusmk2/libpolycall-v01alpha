"""CLI Main Module Tests."""

import asyncio
import pytest

from pypolycall.cli.main import CLI


class TestCLI:
    """Test CLI main functionality."""

    def test_cli_initialization(self):
        cli = CLI()
        assert cli is not None
        assert hasattr(cli, "parser")

    def test_info_command(self):
        cli = CLI()
        result = asyncio.run(cli.run(["info"]))
        assert result == 0

    def test_test_command(self):
        cli = CLI()
        result = asyncio.run(cli.run(["test"]))
        assert result == 0

    def test_parser_creation(self):
        cli = CLI()
        parser = cli.parser
        assert parser is not None

        with pytest.raises(SystemExit):
            parser.parse_args(["--help"])
