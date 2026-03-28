import json
import tempfile
import unittest
from pathlib import Path

from tools.scripts.analyze_components import collect_inventory, render_markdown, write_inventory


class AnalyzeComponentsTests(unittest.TestCase):
    def setUp(self) -> None:
        self.repo_root = Path(__file__).resolve().parents[2]

    def test_collect_inventory_contains_expected_sections(self) -> None:
        inventory = collect_inventory(self.repo_root)
        self.assertIn("core_modules", inventory)
        self.assertIn("headers", inventory)
        self.assertIn("binding_entry_points", inventory)
        self.assertGreater(len(inventory["core_modules"]), 0)
        self.assertGreater(len(inventory["headers"]), 0)

    def test_collect_inventory_is_sorted(self) -> None:
        inventory = collect_inventory(self.repo_root)
        self.assertEqual(inventory["core_modules"], sorted(inventory["core_modules"]))
        self.assertEqual(inventory["headers"], sorted(inventory["headers"]))

    def test_write_inventory_creates_deterministic_outputs(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            output_json = tmp_path / "inventory.json"
            output_md = tmp_path / "inventory.md"

            first = write_inventory(self.repo_root, output_json, output_md)
            first_json = output_json.read_text(encoding="utf-8")
            first_md = output_md.read_text(encoding="utf-8")

            second = write_inventory(self.repo_root, output_json, output_md)
            second_json = output_json.read_text(encoding="utf-8")
            second_md = output_md.read_text(encoding="utf-8")

            self.assertEqual(first, second)
            self.assertEqual(first_json, second_json)
            self.assertEqual(first_md, second_md)
            self.assertEqual(json.loads(first_json), json.loads(second_json))

    def test_render_markdown_includes_sections(self) -> None:
        markdown = render_markdown(collect_inventory(self.repo_root))
        self.assertIn("# Component Inventory", markdown)
        self.assertIn("## Core C Modules", markdown)
        self.assertIn("## Headers", markdown)
        self.assertIn("## Binding Entry Points", markdown)


if __name__ == "__main__":
    unittest.main()
