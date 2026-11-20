#!/usr/bin/env python3
"""
Generate and synchronize localized DOCNOTE and CHANGELOG files for SparkNotePad v1.5.0.
Synchronizes:
  - doc/DOCNOTE_id.txt, doc/DOCNOTE_en.txt
  - doc/CHANGELOG_id.txt, doc/CHANGELOG_en.txt
  - src/doc/ and root directory copies.
"""

from pathlib import Path


def main():
    repo_root = Path(__file__).resolve().parent.parent
    doc_dir = repo_root / "doc"
    src_doc_dir = repo_root / "src" / "doc"
    src_doc_dir.mkdir(parents=True, exist_ok=True)

    with open(doc_dir / "DOCNOTE_id.txt", "r", encoding="utf-8") as f:
        docnote_id = f.read()
    with open(doc_dir / "DOCNOTE_en.txt", "r", encoding="utf-8") as f:
        docnote_en = f.read()
    with open(doc_dir / "CHANGELOG_id.txt", "r", encoding="utf-8") as f:
        changelog_id = f.read()
    with open(doc_dir / "CHANGELOG_en.txt", "r", encoding="utf-8") as f:
        changelog_en = f.read()

    files = {
        src_doc_dir / "DOCNOTE_id.txt": docnote_id,
        src_doc_dir / "DOCNOTE_en.txt": docnote_en,
        src_doc_dir / "CHANGELOG_id.txt": changelog_id,
        src_doc_dir / "CHANGELOG_en.txt": changelog_en,
        repo_root / "DOCNOTE_id.txt": docnote_id,
        repo_root / "DOCNOTE_en.txt": docnote_en,
        repo_root / "CHANGELOG_id.txt": changelog_id,
        repo_root / "CHANGELOG_en.txt": changelog_en,
    }

    for path, content in files.items():
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"Synchronized: {path}")

    master_docnote = docnote_id + "\n\n---\n\n" + docnote_en
    master_changelog = changelog_en

    for p in [
        src_doc_dir / "DOCNOTE.txt",
        doc_dir / "DOCNOTE.txt",
        repo_root / "DOCNOTE.txt",
    ]:
        with open(p, "w", encoding="utf-8") as f:
            f.write(master_docnote)
        print(f"Updated master: {p}")

    for p in [
        src_doc_dir / "CHANGELOG.txt",
        doc_dir / "CHANGELOG.txt",
        repo_root / "CHANGELOG.txt",
    ]:
        with open(p, "w", encoding="utf-8") as f:
            f.write(master_changelog)
        print(f"Updated master: {p}")


if __name__ == "__main__":
    main()
