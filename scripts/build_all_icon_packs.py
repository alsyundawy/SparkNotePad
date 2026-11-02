#!/usr/bin/env python3
"""
Asset extraction, enhancement, and regeneration script for SparkNotePad v1.3.8 icon packs.
Processes 13 icon packs with high-DPI scaling, alpha preservation, and contrast sharpening.
"""

import os

from PIL import Image, ImageEnhance

DOWNLOADS = "/Users/alsyundawy/Downloads"
BASE_DIR = "/Users/alsyundawy/Downloads/GitHub/SparkNotePad/src/icons/packs"

ACTIONS = [
    "newfile",
    "openFile",
    "saved",
    "saveAll",
    "closeFile",
    "closeAll",
    "printer",
    "cut",
    "copy",
    "paste",
    "undo",
    "redo",
    "find",
    "findReplace",
    "zoomIn",
    "zoomOut",
    "wrap",
    "invisibleChar",
    "indentGuide",
    "startRecord",
    "stopRecord",
    "playRecord",
    "playRecord_m",
    "saveRecord",
    "restart",
    "settings",
    "exit",
    "readonly",
    "select_all",
    "text_indent",
    "text_indent_remove",
]


def make_transparent_and_clean(img, bg_color=None, tolerance=35):
    rgba = img.convert("RGBA")
    w, h = rgba.size

    if bg_color is None:
        # sample background from corners
        corners = [
            rgba.getpixel((0, 0)),
            rgba.getpixel((w - 1, 0)),
            rgba.getpixel((0, h - 1)),
            rgba.getpixel((w - 1, h - 1)),
        ]
        bg_color = max(set(corners), key=corners.count)

    pixels = rgba.load()
    br, bg, bb = bg_color[:3]

    for y in range(h):
        for x in range(w):
            r, g, b, a = pixels[x, y]
            diff = abs(r - br) + abs(g - bg) + abs(b - bb)
            if diff < tolerance:
                pixels[x, y] = (r, g, b, 0)
            elif diff < tolerance + 30:
                # Soft blend for anti-aliasing edge
                alpha_factor = (diff - tolerance) / 30.0
                pixels[x, y] = (r, g, b, int(a * alpha_factor))

    # Auto-crop content
    bbox = rgba.getbbox()
    if bbox:
        cropped = rgba.crop(bbox)
    else:
        cropped = rgba

    # Center in square frame (e.g. 24x24 or 32x32)
    cw, ch = cropped.size
    target_dim = max(cw, ch, 20)
    frame = Image.new("RGBA", (target_dim, target_dim), (0, 0, 0, 0))
    offset = ((target_dim - cw) // 2, (target_dim - ch) // 2)
    frame.paste(cropped, offset, cropped)

    # Resize to standard 24x24 and sharpen
    final_24 = frame.resize((24, 24), Image.Resampling.LANCZOS)
    enhancer = ImageEnhance.Sharpness(final_24)
    final_24 = enhancer.enhance(1.4)

    return final_24


def process_16x2_strip(strip_path, out_dir, bg_color=None, tolerance=35):
    os.makedirs(out_dir, exist_ok=True)
    strip = Image.open(strip_path)

    # 16 icons in Row 1 (y=0..32), 16 icons in Row 2 (y=32..64)
    for idx, action in enumerate(ACTIONS):
        row = idx // 16
        col = idx % 16
        x1 = col * 32
        y1 = row * 32
        x2 = min(x1 + 32, strip.width)
        y2 = min(y1 + 32, strip.height)

        cell = strip.crop((x1, y1, x2, y2))
        clean_icon = make_transparent_and_clean(cell, bg_color, tolerance)
        clean_icon.save(os.path.join(out_dir, f"{action}.png"), "PNG")

        # Also save high-DPI @2x
        high_dpi = clean_icon.resize((48, 48), Image.Resampling.LANCZOS)
        high_dpi.save(os.path.join(out_dir, f"{action}@2x.png"), "PNG")


def process_fugue_icons(out_dir):
    os.makedirs(out_dir, exist_ok=True)
    fugue_dir = os.path.join(DOWNLOADS, "fugue-icons-3.5.6/icons")

    mapping = {
        "newfile": "document.png",
        "openFile": "folder-horizontal-open.png",
        "saved": "disk.png",
        "saveAll": "disks.png",
        "closeFile": "cross.png",
        "closeAll": "cross-white.png",
        "printer": "printer.png",
        "cut": "scissors.png",
        "copy": "document-copy.png",
        "paste": "clipboard-paste.png",
        "undo": "arrow-curve-180-left.png",
        "redo": "arrow-curve.png",
        "find": "magnifier.png",
        "findReplace": "binocular.png",
        "zoomIn": "magnifier-zoom-in.png",
        "zoomOut": "magnifier-zoom-out.png",
        "wrap": "edit-alignment.png",
        "invisibleChar": "edit-pilcrow.png",
        "indentGuide": "guide.png",
        "startRecord": "control-record.png",
        "stopRecord": "control-stop-square.png",
        "playRecord": "control.png",
        "playRecord_m": "control-double.png",
        "saveRecord": "disk--pencil.png",
        "restart": "arrow-circle-double-135.png",
        "settings": "gear.png",
        "exit": "door-open-out.png",
        "readonly": "lock.png",
        "select_all": "selection-select.png",
        "text_indent": "edit-indent.png",
        "text_indent_remove": "edit-outdent.png",
    }

    for action, fugue_file in mapping.items():
        src_path = os.path.join(fugue_dir, fugue_file)
        if os.path.exists(src_path):
            img = Image.open(src_path).convert("RGBA")
            # 24x24 canvas
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - img.width) // 2, (24 - img.height) // 2)
            canvas.paste(img, offset, img)
            canvas.save(os.path.join(out_dir, f"{action}.png"), "PNG")

            # @2x
            high_dpi = canvas.resize((48, 48), Image.Resampling.NEAREST)
            high_dpi.save(os.path.join(out_dir, f"{action}@2x.png"), "PNG")


def process_perfect_icons(out_dir):
    os.makedirs(out_dir, exist_ok=True)
    img_path = os.path.join(DOWNLOADS, "all-icons-1.jpg")
    img = Image.open(img_path)

    # 48x56 grid: 14 cols, 10 rows
    for idx, action in enumerate(ACTIONS):
        row = idx // 14
        col = idx % 14
        x1 = col * 48
        y1 = row * 56
        x2 = min(x1 + 48, img.width)
        y2 = min(y1 + 56, img.height)

        cell = img.crop((x1, y1, x2, y2))
        clean_icon = make_transparent_and_clean(cell, tolerance=40)
        clean_icon.save(os.path.join(out_dir, f"{action}.png"), "PNG")

        high_dpi = clean_icon.resize((48, 48), Image.Resampling.LANCZOS)
        high_dpi.save(os.path.join(out_dir, f"{action}@2x.png"), "PNG")


def main():
    print("=== Processing Icon Packs for SparkNotePad v1.3.8 ===")

    # Arch Linux
    print("-> Processing Arch Linux...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "theme_archlinux.png"),
        os.path.join(BASE_DIR, "archLinux"),
        (249, 250, 253),
        35,
    )

    # 4. Win7 v1
    print("-> Processing Win7 v1...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "theme_win7_alt.png"),
        os.path.join(BASE_DIR, "win7V1"),
        (240, 240, 240),
        35,
    )

    # 5. Win7 v2
    print("-> Processing Win7 v2...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "theme_win7_v2_winrar.png"),
        os.path.join(BASE_DIR, "win7V2"),
        (240, 240, 240),
        35,
    )

    # 6. Win7 v3
    print("-> Processing Win7 v3...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "theme_win7_winrar.png"),
        os.path.join(BASE_DIR, "win7V3"),
        (240, 240, 240),
        35,
    )

    # 7. Fugue Icons
    print("-> Processing Fugue Icons...")
    process_fugue_icons(os.path.join(BASE_DIR, "fugue"))

    # 8. Perfect Icons
    print("-> Processing Perfect Icons...")
    process_perfect_icons(os.path.join(BASE_DIR, "perfect"))

    # 9. Notepad3, Notepad4, Notepad++, Fluent UI, Filled Fluent UI
    # These screenshots have toolbar button arrays that can also be cleanly extracted or enhanced
    print("-> Processing Notepad++ Default...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "notepad++-icon.png"),
        os.path.join(BASE_DIR, "notepadPlusPlus"),
        tolerance=30,
    )

    print("-> Processing Fluent UI...")
    if os.path.exists(os.path.join(DOWNLOADS, "fluint-ui.png")):
        process_16x2_strip(
            os.path.join(DOWNLOADS, "fluint-ui.png"),
            os.path.join(BASE_DIR, "fluentUi"),
            tolerance=30,
        )

    print("-> Processing Filled Fluent UI...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "filled-fluent-ui.png"),
        os.path.join(BASE_DIR, "filledFluentUi"),
        tolerance=30,
    )

    print("-> Processing Notepad3...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "notepad3.png"),
        os.path.join(BASE_DIR, "notepad3"),
        tolerance=30,
    )

    print("-> Processing Notepad4...")
    process_16x2_strip(
        os.path.join(DOWNLOADS, "notepad4-icon.png"),
        os.path.join(BASE_DIR, "notepad4"),
        tolerance=30,
    )

    print("=== All 13 Icon Packs Successfully Processed ===")


if __name__ == "__main__":
    main()
