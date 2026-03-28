"""Regression tests for command imports and CLI command registration."""

import importlib


def test_commands_module_exports_only_existing_command_classes():
    commands = importlib.import_module("pypolycall.pypolycall.cli.commands")

    expected = {
        "InfoCommand",
        "TestCommand",
        "ConnectCommand",
        "ConfigCommand",
        "TelemetryCommand",
    }

    assert set(commands.__all__) == expected
    for symbol in commands.__all__:
        assert hasattr(commands, symbol)


def test_nested_cli_parser_build_and_registration_without_import_errors():
    cli_main = importlib.import_module("pypolycall.pypolycall.cli.main")
    cli = cli_main.CLI()

    parser = cli.create_parser()
    assert parser is not None

    registered = set(cli.command_registry.list_commands())
    assert registered == {"info", "test", "connect", "config", "telemetry"}
