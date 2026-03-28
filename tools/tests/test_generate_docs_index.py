import json
import tempfile
import unittest
from pathlib import Path

from tools.scripts.generate_docs_index import (
    build_index,
    collect_docs,
    render_markdown,
    write_index,
)


class GenerateDocsIndexTests(unittest.TestCase):
    def setUp(self) -> None:
        self.repo_root = Path(__file__).resolve().parents[2]

    def test_collect_docs_returns_sorted_markdown_paths(self) -> None:
        docs = collect_docs(self.repo_root)
        self.assertEqual(docs, sorted(docs))
        self.assertTrue(any(path.endswith("docs/USAGE.md") for path in docs))

    def test_build_index_reads_decision_fixture_with_maybe_unresolved(self) -> None:
        index = build_index(self.repo_root)
        grouped = index["decision_states_by_status"]

        self.assertIn("maybe", grouped)
        self.assertIn("unresolved", grouped)
        self.assertGreaterEqual(len(grouped["unresolved"]), 1)

    def test_write_index_is_deterministic(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            output_md = tmp_path / "docs_index.md"
            output_json = tmp_path / "docs_index.json"

            first = write_index(self.repo_root, output_md, output_json)
            first_md = output_md.read_text(encoding="utf-8")
            first_json = output_json.read_text(encoding="utf-8")

            second = write_index(self.repo_root, output_md, output_json)
            second_md = output_md.read_text(encoding="utf-8")
            second_json = output_json.read_text(encoding="utf-8")

            self.assertEqual(first, second)
            self.assertEqual(first_md, second_md)
            self.assertEqual(first_json, second_json)
            self.assertEqual(json.loads(first_json), json.loads(second_json))

    def test_render_markdown_mentions_fixture(self) -> None:
        markdown = render_markdown(build_index(self.repo_root))
        self.assertIn("# Documentation Index", markdown)
        self.assertIn("Decision States by Status", markdown)
        self.assertIn("Fixture source", markdown)


if __name__ == "__main__":
    unittest.main()
