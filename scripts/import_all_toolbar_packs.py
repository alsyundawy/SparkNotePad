#!/usr/bin/env python3
"""Import 30 Toolbar Icon Packs from local downloads into SparkNotePad:

- Generates 6 multi-resolution folders (16, 20, 24, 32, 48, 64)
- Preserves all 40 original PNG filenames
- Adds canonical SparkNotePad toolbar action aliases
- Quantizes PNG assets to 8-bit palette mode with alpha
- Generates updated src/toolbar_icons.qrc entries
"""

import os
import sys

from PIL import Image, ImageOps

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_PACKS_DIR = "/Users/alsyundawy/Downloads/toolbar-iconpacks"
DEST_PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")
QRC_FILE = os.path.join(REPO_ROOT, "src", "toolbar_icons.qrc")

SIZES = ["16x16", "20x20", "24x24", "32x32", "48x48", "64x64"]

CANONICAL_MAP = {
    "newfile.png": "new-file.png",
    "openFile.png": "open-folder.png",
    "folder_go.png": "open-folder-alt.png",
    "saved.png": "save.png",
    "saveAll.png": "save-as.png",
    "closeFile.png": "close-file.png",
    "closeAll.png": "close-all.png",
    "cut.png": "cut.png",
    "copy.png": "copy.png",
    "paste.png": "paste.png",
    "undo.png": "undo.png",
    "redo.png": "redo.png",
    "find.png": "zoom-reset.png",
    "findReplace.png": "split-view.png",
    "zoomIn.png": "zoom-in.png",
    "zoomOut.png": "zoom-out.png",
    "printer.png": "print.png",
    "settings.png": "settings.png",
    "cog.png": "preferences.png",
    "bin.png": "trash.png",
    "restart.png": "reload.png",
    "startRecord.png": "record.png",
    "stopRecord.png": "stop.png",
    "playRecord.png": "play.png",
    "saveRecord.png": "record-stop.png",
    "playRecord_m.png": "lightning.png",
    "readonly.png": "lock.png",
    "select_all.png": "select-all.png",
    "text_indent.png": "indent.png",
    "indentGuide.png": "code.png",
    "invisibleChar.png": "star.png",
    "wrap.png": "align-left.png",
    "exit.png": "close-file.png",
}


def quantize_and_save(img: Image.Image, dest_path: str):
    q = img.quantize(colors=256, method=Image.Quantize.FASTOCTREE)
    q.save(dest_path, format="PNG", optimize=True, compress_level=9)


def process_pack(pack_name: str):
    src_pack = os.path.join(SRC_PACKS_DIR, pack_name)
    dest_pack = os.path.join(DEST_PACKS_DIR, pack_name)
    os.makedirs(dest_pack, exist_ok=True)

    src_png_1x = os.path.join(src_pack, "PNG", "@1x")
    if not os.path.exists(src_png_1x):
        print(f"Skipping {pack_name}: no PNG/@1x folder found")
        return []

    pack_qrc_entries = []

    for sz in SIZES:
        src_sz_dir = os.path.join(src_png_1x, sz)
        dest_sz_dir = os.path.join(dest_pack, sz)
        os.makedirs(dest_sz_dir, exist_ok=True)

        if not os.path.exists(src_sz_dir):
            continue

        # Load all available source icons in this size
        src_icons = {}
        for fname in os.listdir(src_sz_dir):
            if fname.endswith(".png"):
                full_src = os.path.join(src_sz_dir, fname)
                try:
                    im = Image.open(full_src).convert("RGBA")
                    src_icons[fname] = im
                    # Save quantized original
                    dest_file = os.path.join(dest_sz_dir, fname)
                    quantize_and_save(im, dest_file)
                    pack_qrc_entries.append(f"icons/packs/{pack_name}/{sz}/{fname}")
                except (OSError, ValueError) as e:
                    print(f"Error reading {full_src}: {e}")

        # Generate canonical mapped icons
        for canon_name, src_name in CANONICAL_MAP.items():
            if canon_name not in src_icons and src_name in src_icons:
                dest_file = os.path.join(dest_sz_dir, canon_name)
                quantize_and_save(src_icons[src_name], dest_file)
                pack_qrc_entries.append(f"icons/packs/{pack_name}/{sz}/{canon_name}")

        # Generate text_indent_remove by mirroring text_indent/indent
        if "indent.png" in src_icons:
            mirrored = ImageOps.mirror(src_icons["indent.png"])
            dest_file = os.path.join(dest_sz_dir, "text_indent_remove.png")
            quantize_and_save(mirrored, dest_file)
            pack_qrc_entries.append(
                f"icons/packs/{pack_name}/{sz}/text_indent_remove.png"
            )

    return pack_qrc_entries


def main():
    if not os.path.exists(SRC_PACKS_DIR):
        print(f"Error: {SRC_PACKS_DIR} does not exist!")
        sys.exit(1)

    packs = sorted([d for d in os.listdir(SRC_PACKS_DIR) if not d.startswith(".")])
    print(f"Found {len(packs)} packs in {SRC_PACKS_DIR}")

    all_new_qrc_entries = []

    for i, pack_name in enumerate(packs, 1):
        print(f"[{i:02d}/{len(packs)}] Processing {pack_name}...")
        entries = process_pack(pack_name)
        all_new_qrc_entries.extend(entries)
        print(f"   -> Added {len(entries)} files for {pack_name}")

    print(f"\nTotal new icon files generated: {len(all_new_qrc_entries)}")

    # Update src/toolbar_icons.qrc
    print(f"Updating {QRC_FILE}...")
    with open(QRC_FILE, "r", encoding="utf-8") as f:
        content = f.read()

    # Parse existing entries
    existing_lines = content.splitlines()
    existing_files = set()
    for line in existing_lines:
        line_s = line.strip()
        if line_s.startswith("<file>") and line_s.endswith("</file>"):
            existing_files.add(line_s[6:-7])

    added_count = 0
    new_lines = []
    # Find insertion point before </qresource>
    for line in existing_lines:
        if line.strip() == "</qresource>":
            for entry in sorted(all_new_qrc_entries):
                if entry not in existing_files:
                    new_lines.append(f"        <file>{entry}</file>")
                    existing_files.add(entry)
                    added_count += 1
        new_lines.append(line)

    with open(QRC_FILE, "w", encoding="utf-8") as f:
        f.write("\n".join(new_lines) + "\n")

    print(f"Successfully added {added_count} new entries to {QRC_FILE}")


if __name__ == "__main__":
    main()
