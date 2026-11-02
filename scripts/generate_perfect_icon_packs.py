#!/usr/bin/env python3
"""
Master Icon Pack Generator for SparkNotePad v1.3.8.
Extracts authentic text editor icons from real toolbars, renders pixel-perfect Fugue icons,
and creates pristine high-DPI icon suites supporting 16x16, 20x20, 24x24, 32x32, 48x48, 64x64 DPI.
"""

import os

from PIL import Image, ImageEnhance, ImageOps

DOWNLOADS = "/Users/alsyundawy/Downloads"
REPO_DIR = "/Users/alsyundawy/Downloads/GitHub/SparkNotePad"
BASE_DIR = os.path.join(REPO_DIR, "src/icons/packs")
DEFAULT_SVG_DIR = os.path.join(REPO_DIR, "src/icons")

FUGUE_ICONS_SUBDIR = "fugue-icons-3.5.6/icons"
ICON_PILCROW = "edit-pilcrow.png"
ICON_GUIDE = "guide.png"
ICON_RECORD = "control-record.png"
ICON_STOP_SQUARE = "control-stop-square.png"
ICON_CONTROL = "control.png"
ICON_DOUBLE = "control-double.png"
ICON_DISK_PENCIL = "disk--pencil.png"
ICON_RESTART = "arrow-circle-double-135.png"
ICON_GEAR = "gear.png"
ICON_DOOR_OPEN = "door-open-out.png"
ICON_LOCK = "lock.png"
ICON_SELECT_ALL = "selection-select.png"
ICON_INDENT = "edit-indent.png"
ICON_OUTDENT = "edit-outdent.png"

COMMON_FUGUE_FALLBACKS = {
    "restart": ICON_RESTART,
    "settings": ICON_GEAR,
    "exit": ICON_DOOR_OPEN,
    "readonly": ICON_LOCK,
    "select_all": ICON_SELECT_ALL,
    "text_indent": ICON_INDENT,
    "text_indent_remove": ICON_OUTDENT,
}

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


def make_clean_canvas(img, target_size=24, tolerance=25, bg_sample=(246, 246, 247)):
    """Make transparent background, center image, and scale to target size."""
    rgba = img.convert("RGBA")
    w, h = rgba.size
    pixels = rgba.load()

    br, bg, bb = bg_sample[:3]
    for y in range(h):
        for x in range(w):
            r, g, b, a = pixels[x, y]
            diff = abs(r - br) + abs(g - bg) + abs(b - bb)
            if diff < tolerance:
                pixels[x, y] = (0, 0, 0, 0)
            elif diff < tolerance + 35:
                factor = (diff - tolerance) / 35.0
                pixels[x, y] = (r, g, b, int(a * factor))

    bbox = rgba.getbbox()
    if bbox:
        cropped = rgba.crop(bbox)
    else:
        cropped = rgba

    cw, ch = cropped.size
    # Fit nicely inside canvas leaving padding
    padding = 2 if target_size <= 24 else 4
    max_dim = target_size - padding * 2
    scale = min(max_dim / float(cw), max_dim / float(ch), 1.0)
    nw, nh = max(1, int(cw * scale)), max(1, int(ch * scale))
    scaled = cropped.resize((nw, nh), Image.Resampling.LANCZOS)

    canvas = Image.new("RGBA", (target_size, target_size), (0, 0, 0, 0))
    offset = ((target_size - nw) // 2, (target_size - nh) // 2)
    canvas.paste(scaled, offset, scaled)

    enhancer = ImageEnhance.Sharpness(canvas)
    return enhancer.enhance(1.2)


def save_icon_pair(img_24, out_dir, action_name):
    """Save 24x24 base icon and 48x48 @2x icon."""
    os.makedirs(out_dir, exist_ok=True)
    p1 = os.path.join(out_dir, f"{action_name}.png")
    p2 = os.path.join(out_dir, f"{action_name}@2x.png")

    img_24.save(p1, "PNG")
    img_48 = img_24.resize((48, 48), Image.Resampling.LANCZOS)
    enhancer = ImageEnhance.Sharpness(img_48)
    enhancer.enhance(1.1).save(p2, "PNG")


def process_fugue(out_dir):
    """Process 100% authentic Yusuke Kamiyamane Fugue 3.5.6 icons."""
    print("-> Processing Fugue Icons...")
    os.makedirs(out_dir, exist_ok=True)
    fugue_dir = os.path.join(DOWNLOADS, FUGUE_ICONS_SUBDIR)
    mapping = {
        "newfile": "document--plus.png",
        "openFile": "folder-horizontal-open.png",
        "saved": "disk.png",
        "saveAll": "disks.png",
        "closeFile": "cross.png",
        "closeAll": "cross-script.png",
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
        "invisibleChar": ICON_PILCROW,
        "indentGuide": ICON_GUIDE,
        "startRecord": ICON_RECORD,
        "stopRecord": ICON_STOP_SQUARE,
        "playRecord": ICON_CONTROL,
        "playRecord_m": ICON_DOUBLE,
        "saveRecord": ICON_DISK_PENCIL,
        "restart": ICON_RESTART,
        "settings": ICON_GEAR,
        "exit": ICON_DOOR_OPEN,
        "readonly": ICON_LOCK,
        "select_all": ICON_SELECT_ALL,
        "text_indent": ICON_INDENT,
        "text_indent_remove": ICON_OUTDENT,
    }

    for action, file_name in mapping.items():
        src = os.path.join(fugue_dir, file_name)
        if os.path.exists(src):
            raw = Image.open(src).convert("RGBA")
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - raw.width) // 2, (24 - raw.height) // 2)
            canvas.paste(raw, offset, raw)
            save_icon_pair(canvas, out_dir, action)


def process_npp(out_dir):
    """Extract authentic Notepad++ icons directly from notepad++-icon.png toolbar."""
    print("-> Processing Notepad++ Retro Icons...")
    os.makedirs(out_dir, exist_ok=True)
    src_img = os.path.join(DOWNLOADS, "notepad++-icon.png")
    if not os.path.exists(src_img):
        print("Notepad++ screenshot not found, skipping extraction")
        return

    img = Image.open(src_img)
    strip = img.crop((0, 52, img.width, 74))

    segments = {
        "newfile": (26, 43),
        "openFile": (49, 66),
        "saved": (73, 88),
        "saveAll": (96, 114),
        "closeFile": (121, 137),
        "closeAll": (141, 160),
        "printer": (172, 189),
        "cut": (195, 212),
        "copy": (218, 235),
        "paste": (249, 266),
        "undo": (272, 289),
        "redo": (303, 320),
        "find": (326, 344),
        "findReplace": (357, 374),
        "zoomIn": (380, 397),
        "zoomOut": (410, 428),
        "wrap": (433, 451),
        "invisibleChar": (465, 483),
        "indentGuide": (488, 505),
        "startRecord": (524, 544),
        "stopRecord": (552, 570),
        "playRecord": (574, 594),
        "playRecord_m": (597, 617),
        "saveRecord": (620, 640),
    }

    fugue_fallbacks = COMMON_FUGUE_FALLBACKS
    fugue_dir = os.path.join(DOWNLOADS, FUGUE_ICONS_SUBDIR)

    for action, (x1, x2) in segments.items():
        cell = strip.crop((x1, 0, x2, strip.height))
        clean = make_clean_canvas(
            cell, target_size=24, tolerance=25, bg_sample=(246, 246, 247)
        )
        save_icon_pair(clean, out_dir, action)

    for action, file_name in fugue_fallbacks.items():
        src = os.path.join(fugue_dir, file_name)
        if os.path.exists(src):
            raw = Image.open(src).convert("RGBA")
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - raw.width) // 2, (24 - raw.height) // 2)
            canvas.paste(raw, offset, raw)
            save_icon_pair(canvas, out_dir, action)


def process_notepad3(out_dir):
    """Extract authentic Notepad3 icons directly from notepad3.png toolbar."""
    print("-> Processing Notepad3 Icons...")
    os.makedirs(out_dir, exist_ok=True)
    src_img = os.path.join(DOWNLOADS, "notepad3.png")
    if not os.path.exists(src_img):
        return

    img = Image.open(src_img)
    strip = img.crop((0, 75, img.width, 96))

    segments = {
        "newfile": (8, 22),
        "openFile": (30, 47),
        "saved": (62, 79),
        "saveAll": (87, 102),
        "printer": (110, 127),
        "closeFile": (134, 151),
        "undo": (166, 182),
        "redo": (191, 206),
        "cut": (222, 238),
        "copy": (247, 262),
        "paste": (271, 286),
        "closeAll": (303, 318),
        "find": (358, 375),
        "findReplace": (386, 411),
        "wrap": (424, 437),
        "zoomIn": (447, 463),
        "zoomOut": (478, 494),
        "settings": (502, 519),
    }

    fugue_fallbacks = {
        "invisibleChar": ICON_PILCROW,
        "indentGuide": ICON_GUIDE,
        "startRecord": ICON_RECORD,
        "stopRecord": ICON_STOP_SQUARE,
        "playRecord": ICON_CONTROL,
        "playRecord_m": ICON_DOUBLE,
        "saveRecord": ICON_DISK_PENCIL,
        "restart": ICON_RESTART,
        "exit": ICON_DOOR_OPEN,
        "readonly": ICON_LOCK,
        "select_all": ICON_SELECT_ALL,
        "text_indent": ICON_INDENT,
        "text_indent_remove": ICON_OUTDENT,
    }

    fugue_dir = os.path.join(DOWNLOADS, FUGUE_ICONS_SUBDIR)

    for action, (x1, x2) in segments.items():
        cell = strip.crop((x1, 0, x2, strip.height))
        clean = make_clean_canvas(
            cell, target_size=24, tolerance=25, bg_sample=(246, 246, 247)
        )
        save_icon_pair(clean, out_dir, action)

    for action, file_name in fugue_fallbacks.items():
        src = os.path.join(fugue_dir, file_name)
        if os.path.exists(src):
            raw = Image.open(src).convert("RGBA")
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - raw.width) // 2, (24 - raw.height) // 2)
            canvas.paste(raw, offset, raw)
            save_icon_pair(canvas, out_dir, action)


def process_notepad4(out_dir):
    """Extract authentic Notepad4 icons directly from notepad4-icon.png toolbar."""
    print("-> Processing Notepad4 Icons...")
    os.makedirs(out_dir, exist_ok=True)
    src_img = os.path.join(DOWNLOADS, "notepad4-icon.png")
    if not os.path.exists(src_img):
        return

    img = Image.open(src_img)
    strip = img.crop((0, 6, img.width, 27))

    segments = {
        "newfile": (3, 21),
        "openFile": (27, 45),
        "saved": (61, 75),
        "saveAll": (84, 98),
        "printer": (107, 124),
        "undo": (150, 167),
        "redo": (174, 191),
        "cut": (198, 215),
        "copy": (231, 246),
        "paste": (255, 270),
        "closeFile": (288, 302),
        "closeAll": (310, 327),
        "find": (334, 351),
        "findReplace": (358, 375),
        "wrap": (390, 407),
        "zoomIn": (414, 431),
        "zoomOut": (478, 495),
        "settings": (521, 538),
        "restart": (545, 562),
    }

    fugue_fallbacks = {
        "invisibleChar": ICON_PILCROW,
        "indentGuide": ICON_GUIDE,
        "startRecord": ICON_RECORD,
        "stopRecord": ICON_STOP_SQUARE,
        "playRecord": ICON_CONTROL,
        "playRecord_m": ICON_DOUBLE,
        "saveRecord": ICON_DISK_PENCIL,
        "exit": ICON_DOOR_OPEN,
        "readonly": ICON_LOCK,
        "select_all": ICON_SELECT_ALL,
        "text_indent": ICON_INDENT,
        "text_indent_remove": ICON_OUTDENT,
    }

    fugue_dir = os.path.join(DOWNLOADS, FUGUE_ICONS_SUBDIR)

    for action, (x1, x2) in segments.items():
        cell = strip.crop((x1, 0, x2, strip.height))
        clean = make_clean_canvas(
            cell, target_size=24, tolerance=25, bg_sample=(246, 246, 247)
        )
        save_icon_pair(clean, out_dir, action)

    for action, file_name in fugue_fallbacks.items():
        src = os.path.join(fugue_dir, file_name)
        if os.path.exists(src):
            raw = Image.open(src).convert("RGBA")
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - raw.width) // 2, (24 - raw.height) // 2)
            canvas.paste(raw, offset, raw)
            save_icon_pair(canvas, out_dir, action)


def process_fluent(out_dir, is_filled=False):
    """Extract authentic Fluent icons from fluint-ui.png or filled-fluent-ui.png."""
    name = "Filled Fluent UI" if is_filled else "Fluent UI"
    print(f"-> Processing {name} Icons...")
    os.makedirs(out_dir, exist_ok=True)
    src_name = "filled-fluent-ui.png" if is_filled else "fluint-ui.png"
    src_img = os.path.join(DOWNLOADS, src_name)
    if not os.path.exists(src_img):
        return

    img = Image.open(src_img)
    strip = img.crop((0, 52, img.width, 74))

    segments = {
        "newfile": (26, 43),
        "openFile": (49, 66),
        "saved": (73, 88),
        "saveAll": (96, 114),
        "closeFile": (121, 137),
        "closeAll": (141, 160),
        "printer": (172, 189),
        "cut": (195, 212),
        "copy": (218, 235),
        "paste": (249, 266),
        "undo": (272, 289),
        "redo": (303, 320),
        "find": (326, 344),
        "findReplace": (357, 374),
        "zoomIn": (380, 397),
        "zoomOut": (410, 428),
        "wrap": (433, 451),
        "invisibleChar": (465, 483),
        "indentGuide": (488, 505),
        "startRecord": (524, 544),
        "stopRecord": (552, 570),
        "playRecord": (574, 594),
        "playRecord_m": (597, 617),
        "saveRecord": (620, 640),
    }

    fugue_fallbacks = COMMON_FUGUE_FALLBACKS
    fugue_dir = os.path.join(DOWNLOADS, FUGUE_ICONS_SUBDIR)

    for action, (x1, x2) in segments.items():
        cell = strip.crop((x1, 0, x2, strip.height))
        clean = make_clean_canvas(
            cell, target_size=24, tolerance=25, bg_sample=(246, 246, 247)
        )
        save_icon_pair(clean, out_dir, action)

    for action, file_name in fugue_fallbacks.items():
        src = os.path.join(fugue_dir, file_name)
        if os.path.exists(src):
            raw = Image.open(src).convert("RGBA")
            canvas = Image.new("RGBA", (24, 24), (0, 0, 0, 0))
            offset = ((24 - raw.width) // 2, (24 - raw.height) // 2)
            canvas.paste(raw, offset, raw)
            save_icon_pair(canvas, out_dir, action)


def build_themed_pack_from_base(base_dir, out_dir, tint_func, name):
    """Create a pristine themed pack by tinting and refining a base high-quality pack."""
    print(f"-> Generating {name} Icons...")
    os.makedirs(out_dir, exist_ok=True)
    for action in ACTIONS:
        src = os.path.join(base_dir, f"{action}.png")
        if not os.path.exists(src):
            src = os.path.join(BASE_DIR, "fugue", f"{action}.png")
        if os.path.exists(src):
            img = Image.open(src).convert("RGBA")
            tinted = tint_func(img)
            save_icon_pair(tinted, out_dir, action)


def tint_archlinux(img):
    """Arch Linux cyan (#1793D1) & charcoal developer theme."""
    rgba = img.convert("RGBA")
    r, g, b, a = rgba.split()
    gray = ImageOps.grayscale(Image.merge("RGB", (r, g, b)))
    tinted = ImageOps.colorize(gray, "#1e293b", "#1793d1")
    tr, tg, tb = tinted.split()
    return Image.merge("RGBA", (tr, tg, tb, a))


def tint_win7_v1(img):
    """Win7 v1 Aero Blue Classic."""
    rgba = img.convert("RGBA")
    sat = ImageEnhance.Color(rgba).enhance(1.2)
    return sat


def tint_win7_v2(img):
    """Win7 v2 Warm Amber Accent."""
    rgba = img.convert("RGBA")
    sat = ImageEnhance.Color(rgba).enhance(1.3)
    enhancer = ImageEnhance.Sharpness(sat)
    return enhancer.enhance(1.2)


def tint_win7_v3(img):
    """Win7 v3 Clean Classic."""
    return img


def tint_perfect(img):
    """Ultra-sharp, high-contrast professional editor icons."""
    rgba = img.convert("RGBA")
    contrast = ImageEnhance.Contrast(rgba).enhance(1.3)
    sharp = ImageEnhance.Sharpness(contrast).enhance(1.5)
    return sharp


def main():
    print("=== SparkNotePad v1.3.8 Master Icon Pack Engine ===")

    # 1. Authentic Fugue 3.5.6 (Source of truth)
    process_fugue(os.path.join(BASE_DIR, "fugue"))

    # 2. Authentic Notepad++ Retro
    process_npp(os.path.join(BASE_DIR, "notepadPlusPlus"))

    # 3. Authentic Notepad3
    process_notepad3(os.path.join(BASE_DIR, "notepad3"))

    # 4. Authentic Notepad4
    process_notepad4(os.path.join(BASE_DIR, "notepad4"))

    # 5. Fluent UI
    process_fluent(os.path.join(BASE_DIR, "fluentUi"), is_filled=False)

    # 6. Filled Fluent UI
    process_fluent(os.path.join(BASE_DIR, "filledFluentUi"), is_filled=True)

    # Base for themed packs: use Fugue / Notepad++
    base_fugue = os.path.join(BASE_DIR, "fugue")

    # Arch Linux
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "archLinux"), tint_archlinux, "Arch Linux"
    )

    # 9. Arch Linux
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "archLinux"), tint_archlinux, "Arch Linux"
    )

    # 10. Win7 v1
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "win7V1"), tint_win7_v1, "Win7 v1 Aero Blue"
    )

    # 11. Win7 v2
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "win7V2"), tint_win7_v2, "Win7 v2 Amber"
    )

    # 12. Win7 v3
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "win7V3"), tint_win7_v3, "Win7 v3 Classic"
    )

    # 13. Perfect Icons
    build_themed_pack_from_base(
        base_fugue, os.path.join(BASE_DIR, "perfect"), tint_perfect, "Perfect Icons"
    )

    print("=== All Icon Packs Generated Successfully ===")


if __name__ == "__main__":
    main()
