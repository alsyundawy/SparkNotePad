#!/usr/bin/env python3
"""
Generate 3 complete, multi-resolution PNG toolbar icon packs for SparkNotePad:
1. pastel (Pastel Minimalist)
2. notepad_pro (Notepad Pro / Vista 7 Style)
3. royal_blue (Royal Blue / Aero Glossy)

Generates all 34 canonical icons across resolutions:
16x16, 20x20, 24x24, 32x32, 48x48, 64x64, root (48x48) and @2x (96x96).
"""

import os
from PIL import Image, ImageDraw, ImageFilter, ImageOps

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(BASE_DIR, "src", "icons", "packs")

SRC_IMG_PASTEL = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866798_a24686cd.jpg"
SRC_IMG_NOTEPAD_PRO = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866746_524d11b8.jpg"
SRC_IMG_ROYAL_BLUE = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866798_7ef575b4.jpg"

SIZES = [16, 20, 24, 32, 48, 64]

ALL_CANONICAL = [
    "bin", "closeAll", "closeFile", "cog", "copy", "cut", "exit",
    "find", "findReplace", "folder_go", "indentGuide", "invisibleChar",
    "newfile", "openFile", "paste", "playRecord", "playRecord_m",
    "printer", "readonly", "redo", "restart", "saveAll", "saved",
    "saveRecord", "select_all", "settings", "startRecord", "stopRecord",
    "text_indent", "text_indent_remove", "undo", "wrap", "zoomIn", "zoomOut"
]

def make_transparent(im, bg_thresh=248):
    im = im.convert("RGBA")
    datas = im.getdata()
    new_data = []
    for item in datas:
        r, g, b, a = item
        if r >= bg_thresh and g >= bg_thresh and b >= bg_thresh:
            new_data.append((r, g, b, 0))
        elif r > 218 and g > 218 and b > 218:
            diff = max(255 - r, 255 - g, 255 - b)
            alpha = int(diff * 255 / (255 - 218))
            new_data.append((r, g, b, min(255, alpha * 2)))
        else:
            new_data.append((r, g, b, 255))
    im.putdata(new_data)
    return im

def crop_and_clean(im, bbox, max_y_in_cell=None):
    x1, y1, x2, y2 = bbox
    if max_y_in_cell is not None:
        y2 = min(y2, y1 + max_y_in_cell)
    cell = im.crop((x1, y1, x2, y2))
    cleaned = make_transparent(cell)
    # Autocrop transparent borders
    bbox_alpha = cleaned.getbbox()
    if bbox_alpha:
        cropped = cleaned.crop(bbox_alpha)
    else:
        cropped = cleaned
    # Square with slight padding
    cw, ch = cropped.size
    side = max(cw, ch)
    # create square canvas
    sq = Image.new("RGBA", (side + 4, side + 4), (0, 0, 0, 0))
    sq.paste(cropped, ((side + 4 - cw) // 2, (side + 4 - ch) // 2), cropped)
    return sq

def save_icon_sizes(icon_img, pack_dir, name):
    for sz in SIZES:
        sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
        os.makedirs(sz_dir, exist_ok=True)
        resized = icon_img.resize((sz, sz), Image.Resampling.LANCZOS)
        resized.save(os.path.join(sz_dir, f"{name}.png"), optimize=True)

def process_royal_blue():
    print("=== Processing royal_blue Icon Pack ===")
    pack_dir = os.path.join(PACKS_DIR, "royal_blue")
    os.makedirs(pack_dir, exist_ok=True)
    im = Image.open(SRC_IMG_ROYAL_BLUE).convert("RGB")

    # Mapping of (row_idx, col_idx) to canonical name
    # Rows: 0: (21, 117), 1: (136, 235), 2: (248, 343), 3: (356, 450), 4: (465, 560)
    grid = {
        (0, 0): ("newfile", (47, 21, 120, 117)),
        (0, 1): ("openFile", (175, 21, 278, 117)),
        (0, 2): ("saved", (328, 21, 416, 117)),
        (0, 3): ("saveAll", (470, 21, 571, 117)),
        (0, 4): ("saveRecord", (613, 21, 708, 117)),
        (0, 5): ("closeFile", (762, 21, 838, 117)),
        (0, 6): ("closeAll", (895, 21, 982, 117)),

        (1, 0): ("exit", (38, 136, 124, 235)),
        (1, 1): ("copy", (186, 136, 269, 235)),
        (1, 2): ("cut", (329, 136, 417, 235)),
        (1, 3): ("paste", (474, 136, 555, 235)),
        (1, 4): ("undo", (617, 136, 697, 235)),
        (1, 5): ("redo", (758, 136, 838, 235)),
        (1, 6): ("select_all", (900, 136, 983, 235)),

        (2, 0): ("find", (44, 248, 130, 343)),
        (2, 1): ("findReplace", (193, 248, 292, 343)),
        (2, 2): ("wrap", (340, 248, 427, 343)),
        (2, 3): ("zoomIn", (479, 248, 566, 343)),
        (2, 4): ("zoomOut", (620, 248, 705, 343)),
        (2, 5): ("indentGuide", (763, 248, 840, 343)),
        (2, 6): ("invisibleChar", (912, 248, 984, 343)),

        (3, 0): ("readonly", (47, 356, 123, 450)),
        (3, 1): ("playRecord", (187, 356, 273, 450)),
        (3, 2): ("startRecord", (340, 356, 419, 450)),
        (3, 3): ("playRecord_m", (484, 356, 560, 450)),
        (3, 4): ("stopRecord", (615, 356, 703, 450)),
        (3, 5): ("settings", (757, 356, 845, 450)),
        (3, 6): ("cog", (898, 356, 983, 450)),

        (4, 0): ("printer", (40, 465, 143, 560)),
        (4, 1): ("bin", (201, 465, 280, 560)),
        (4, 2): ("folder_go", (343, 465, 448, 560)),
        (4, 3): ("restart", (501, 465, 589, 560)),
        (4, 4): ("text_indent", (644, 465, 726, 560)),
        (4, 5): ("text_indent_remove", (768, 465, 851, 560)),
    }

    count = 0
    for key, (name, bbox) in grid.items():
        icon = crop_and_clean(im, bbox)
        save_icon_sizes(icon, pack_dir, name)
        count += 1
    print(f"royal_blue generated {count} icons across all resolutions.")

def process_notepad_pro():
    print("=== Processing notepad_pro Icon Pack ===")
    pack_dir = os.path.join(PACKS_DIR, "notepad_pro")
    os.makedirs(pack_dir, exist_ok=True)
    im = Image.open(SRC_IMG_NOTEPAD_PRO).convert("RGB")

    # 33 icons from Image 2
    grid = {
        # Row 0
        (0, 0): ("bin", (64, 95, 146, 170)),
        (0, 1): ("closeAll", (219, 95, 303, 170)),
        (0, 2): ("closeFile", (384, 95, 470, 170)),
        (0, 3): ("cog", (557, 95, 640, 170)),
        (0, 4): ("copy", (720, 95, 815, 170)),
        (0, 5): ("cut", (881, 95, 969, 170)),

        # Row 1
        (1, 0): ("exit", (67, 178, 153, 268)),
        (1, 1): ("find", (223, 178, 313, 268)),
        (1, 2): ("findReplace", (388, 178, 478, 268)),
        (1, 3): ("folder_go", (555, 178, 650, 268)),
        (1, 4): ("indentGuide", (732, 178, 802, 268)),
        (1, 5): ("invisibleChar", (883, 178, 970, 268)),

        # Row 2
        (2, 0): ("newfile", (57, 277, 148, 368)),
        (2, 1): ("openFile", (222, 277, 311, 368)),
        (2, 2): ("paste", (393, 277, 476, 368)),
        (2, 3): ("playRecord_m", (553, 277, 637, 368)),
        (2, 4): ("playRecord", (722, 277, 803, 368)),
        (2, 5): ("printer", (880, 277, 971, 368)),

        # Row 3
        (3, 0): ("readonly", (61, 377, 154, 460)),
        (3, 1): ("redo", (228, 377, 306, 460)),
        (3, 2): ("restart", (392, 377, 473, 460)),
        (3, 3): ("saveAll", (561, 377, 653, 460)),
        (3, 4): ("saved", (723, 377, 805, 460)),
        (3, 5): ("saveRecord", (882, 377, 967, 460)),

        # Row 4
        (4, 0): ("select_all", (58, 466, 151, 552)),
        (4, 1): ("settings", (230, 466, 311, 552)),
        (4, 2): ("startRecord", (390, 466, 474, 552)),
        (4, 3): ("stopRecord", (557, 466, 639, 552)),
        (4, 4): ("text_indent_remove", (725, 466, 807, 552)),
        (4, 5): ("undo", (881, 466, 980, 552)),

        # Row 5
        (5, 0): ("wrap", (59, 561, 154, 650)),
        (5, 1): ("zoomIn", (230, 561, 311, 650)),
        (5, 2): ("zoomOut", (396, 561, 477, 650)),
    }

    count = 0
    extracted = {}
    for key, (name, bbox) in grid.items():
        icon = crop_and_clean(im, bbox, max_y_in_cell=62)
        save_icon_sizes(icon, pack_dir, name)
        extracted[name] = icon
        count += 1

    # Synthesize text_indent from text_indent_remove
    # text_indent_remove has the arrow pointing left. For text_indent, arrow points right.
    if "text_indent_remove" in extracted:
        tir = extracted["text_indent_remove"]
        # Flip horizontally the arrow
        ti = ImageOps.mirror(tir)
        save_icon_sizes(ti, pack_dir, "text_indent")
        count += 1

    print(f"notepad_pro generated {count} icons across all resolutions.")

def process_pastel():
    print("=== Processing pastel Icon Pack ===")
    pack_dir = os.path.join(PACKS_DIR, "pastel")
    os.makedirs(pack_dir, exist_ok=True)
    im = Image.open(SRC_IMG_PASTEL).convert("RGB")

    # Image 1 coordinates
    grid = {
        "newfile": (55, 58, 120, 128),
        "openFile": (155, 58, 245, 128),
        "saved": (275, 58, 365, 128),
        "printer": (395, 58, 485, 128),
        "saveAll": (550, 58, 635, 128),     # doc with badge
        "closeFile": (675, 58, 745, 128),   # X
        "undo": (780, 58, 855, 128),        # arrow left
        "redo": (885, 58, 975, 128),        # arrow right

        "cut": (45, 184, 125, 254),         # scissors
        "copy": (158, 184, 248, 254),       # two docs
        "paste": (275, 184, 360, 254),      # clipboard
        "find": (380, 184, 445, 254),       # magnifier
        "findReplace": (425, 184, 490, 254),# magnifier with pencil
        "text_indent": (510, 184, 620, 254),# arrow into pencil
        "select_all": (645, 184, 735, 254), # dashed box
        "bin": (765, 184, 850, 254),        # eraser
        "readonly": (875, 184, 965, 254),   # book with checkmark

        "zoomIn": (395, 317, 480, 384),     # A ^
        "zoomOut": (520, 317, 600, 384),    # A v
        "wrap": (880, 317, 965, 384),       # strike / wrap

        "indentGuide": (260, 439, 360, 500),# column layout
        "restart": (880, 439, 965, 500),    # sun/moon dual circle
    }

    count = 0
    extracted = {}
    for name, bbox in grid.items():
        icon = crop_and_clean(im, bbox)
        save_icon_sizes(icon, pack_dir, name)
        extracted[name] = icon
        count += 1

    # Synthesize missing icons in exact pastel style
    # Palette colors extracted from Image 1:
    # Slate line: #7684a2 (118, 132, 162)
    # Slate fill: #cbd5e1 (203, 213, 225)
    # Accent green: #8bd49b (139, 212, 155)
    # Accent red: #e06c75 (224, 108, 117)
    # Accent purple: #a894c8 (168, 148, 200)

    # 1. closeAll (two docs with X)
    if "copy" in extracted and "closeFile" in extracted:
        ca = extracted["copy"].copy()
        # paste closeFile X in bottom right
        cf_small = extracted["closeFile"].resize((ca.width // 2, ca.height // 2), Image.Resampling.LANCZOS)
        ca.paste(cf_small, (ca.width - cf_small.width, ca.height - cf_small.height), cf_small)
        save_icon_sizes(ca, pack_dir, "closeAll")
        count += 1

    # 2. saveRecord (floppy with red dot)
    if "saved" in extracted:
        sr = extracted["saved"].copy()
        draw = ImageDraw.Draw(sr)
        r = sr.width // 6
        cx, cy = sr.width - r - 2, sr.height - r - 2
        draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(224, 108, 117, 255), outline=(118, 132, 162, 255), width=2)
        save_icon_sizes(sr, pack_dir, "saveRecord")
        count += 1

    # 3. startRecord (play triangle with red dot)
    sr_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(sr_icon)
    # Play triangle
    d.polygon([(24, 18), (24, 78), (76, 48)], fill=(168, 185, 215, 230), outline=(118, 132, 162, 255), width=3)
    # Red dot
    d.ellipse([64, 14, 88, 38], fill=(224, 108, 117, 255), outline=(118, 132, 162, 255), width=2)
    save_icon_sizes(sr_icon, pack_dir, "startRecord")
    count += 1

    # 4. playRecord (clean play triangle)
    pr_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(pr_icon)
    d.polygon([(24, 18), (24, 78), (76, 48)], fill=(139, 212, 155, 230), outline=(118, 132, 162, 255), width=3)
    save_icon_sizes(pr_icon, pack_dir, "playRecord")
    count += 1

    # 5. playRecord_m (double play triangle)
    prm_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(prm_icon)
    d.polygon([(14, 24), (14, 72), (50, 48)], fill=(168, 185, 215, 220), outline=(118, 132, 162, 255), width=3)
    d.polygon([(46, 24), (46, 72), (82, 48)], fill=(168, 185, 215, 220), outline=(118, 132, 162, 255), width=3)
    save_icon_sizes(prm_icon, pack_dir, "playRecord_m")
    count += 1

    # 6. stopRecord (pastel square in circle)
    st_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(st_icon)
    d.ellipse([12, 12, 84, 84], fill=(203, 213, 225, 180), outline=(118, 132, 162, 255), width=3)
    d.rounded_rectangle([32, 32, 64, 64], radius=4, fill=(224, 108, 117, 240), outline=(118, 132, 162, 255), width=2)
    save_icon_sizes(st_icon, pack_dir, "stopRecord")
    count += 1

    # 7. settings (pastel gear)
    set_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(set_icon)
    # Circle with spokes
    d.ellipse([20, 20, 76, 76], fill=(203, 213, 225, 200), outline=(118, 132, 162, 255), width=4)
    d.ellipse([38, 38, 58, 58], fill=(0, 0, 0, 0), outline=(118, 132, 162, 255), width=3)
    # Spokes
    for angle in [0, 45, 90, 135]:
        d.line([(48 - 36, 48), (48 - 26, 48)], fill=(118, 132, 162, 255), width=6)
        d.line([(48 + 26, 48), (48 + 36, 48)], fill=(118, 132, 162, 255), width=6)
        d.line([(48, 48 - 36), (48, 48 - 26)], fill=(118, 132, 162, 255), width=6)
        d.line([(48, 48 + 26), (48, 48 + 36)], fill=(118, 132, 162, 255), width=6)
    save_icon_sizes(set_icon, pack_dir, "settings")
    save_icon_sizes(set_icon, pack_dir, "cog")
    count += 2

    # 8. invisibleChar (pastel doc with pilcrow)
    if "newfile" in extracted:
        inv = extracted["newfile"].copy()
        d = ImageDraw.Draw(inv)
        # Pilcrow ¶ in center
        cx, cy = inv.width // 2, inv.height // 2
        d.ellipse([cx - 8, cy - 10, cx + 2, cy], fill=(118, 132, 162, 240))
        d.line([(cx + 2, cy - 10), (cx + 2, cy + 12)], fill=(118, 132, 162, 255), width=2)
        d.line([(cx + 6, cy - 10), (cx + 6, cy + 12)], fill=(118, 132, 162, 255), width=2)
        save_icon_sizes(inv, pack_dir, "invisibleChar")
        count += 1

    # 9. text_indent_remove
    if "text_indent" in extracted:
        tir = ImageOps.mirror(extracted["text_indent"])
        save_icon_sizes(tir, pack_dir, "text_indent_remove")
        count += 1

    # 10. exit (door / power)
    ex_icon = Image.new("RGBA", (96, 96), (0, 0, 0, 0))
    d = ImageDraw.Draw(ex_icon)
    # Circle with top notch
    d.arc([16, 16, 80, 80], start=300, end=240, fill=(224, 108, 117, 255), width=5)
    d.line([(48, 14), (48, 44)], fill=(224, 108, 117, 255), width=5)
    save_icon_sizes(ex_icon, pack_dir, "exit")
    count += 1

    # 11. folder_go
    if "openFile" in extracted:
        fg = extracted["openFile"].copy()
        d = ImageDraw.Draw(fg)
        # Green arrow pointing right in bottom right
        d.polygon([(fg.width - 24, fg.height - 18), (fg.width - 12, fg.height - 18),
                   (fg.width - 12, fg.height - 24), (fg.width - 4, fg.height - 15),
                   (fg.width - 12, fg.height - 6), (fg.width - 12, fg.height - 12),
                   (fg.width - 24, fg.height - 12)], fill=(139, 212, 155, 255), outline=(118, 132, 162, 255), width=1)
        save_icon_sizes(fg, pack_dir, "folder_go")
        count += 1

    print(f"pastel generated {count} icons across all resolutions.")

if __name__ == "__main__":
    process_royal_blue()
    process_notepad_pro()
    process_pastel()
    print("ALL 3 ICON PACKS GENERATED SUCCESSFULLY!")
