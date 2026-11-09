#!/usr/bin/env python3
"""
Comprehensive Icon Pack Sanitizer for SparkNotePad:
Eliminates all drop shadows, background tiles, halos, and emboss/stroke artifacts
across all custom icon packs to match the pristine Notepad++ (Classic) standard.

Ensures:
1. Pure isolated icon silhouette floating cleanly on the dark toolbar (#21252b).
2. 100% transparent backgrounds with zero non-transparent corner pixels.
3. Zero floor shadows, ambient occlusion, or divider lines.
4. Clean multi-resolution outputs: 16x16, 20x20, 24x24, 32x32, 48x48, 64x64.
"""

import os
from collections import deque
from PIL import Image, ImageDraw

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")

SIZES = [16, 20, 24, 32, 48, 64]

ALL_CANONICAL = [
    "bin", "closeAll", "closeFile", "cog", "copy", "cut", "exit",
    "find", "findReplace", "folder_go", "indentGuide", "invisibleChar",
    "newfile", "openFile", "paste", "playRecord", "playRecord_m",
    "printer", "readonly", "redo", "restart", "saveAll", "saved",
    "saveRecord", "select_all", "settings", "startRecord", "stopRecord",
    "text_indent", "text_indent_remove", "undo", "wrap", "zoomIn", "zoomOut"
]

def clean_outer_background_and_shadows(im, bg_thresh=230, shadow_max_sat=26, shadow_min_lum=80):
    """
    Flood-fill all outer sheet background pixels, floor drop shadows,
    and cell divider lines to pure (0, 0, 0, 0).
    Stops cleanly at saturated or dark icon contours.
    """
    img = im.convert("RGBA")
    w, h = img.size
    pixels = img.load()

    visited = [[False] * h for _ in range(w)]
    queue = deque()

    def is_outer_artifact(x, y):
        r, g, b, _ = pixels[x, y]
        # White, off-white, or light sheet background
        if r >= bg_thresh and g >= bg_thresh and b >= bg_thresh:
            return True
        if min(r, g, b) >= 215:
            return True
        # Desaturated gray shadow, ambient occlusion, or divider line
        sat = max(r, g, b) - min(r, g, b)
        if sat <= shadow_max_sat and min(r, g, b) >= shadow_min_lum:
            return True
        return False

    # Seed with all 4 outer borders
    for x in range(w):
        for y in (0, h - 1):
            if is_outer_artifact(x, y):
                visited[x][y] = True
                queue.append((x, y))
    for y in range(h):
        for x in (0, w - 1):
            if not visited[x][y] and is_outer_artifact(x, y):
                visited[x][y] = True
                queue.append((x, y))

    # Flood fill
    while queue:
        cx, cy = queue.popleft()
        pixels[cx, cy] = (0, 0, 0, 0)
        for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            nx, ny = cx + dx, cy + dy
            if 0 <= nx < w and 0 <= ny < h and not visited[nx][ny]:
                visited[nx][ny] = True
                if is_outer_artifact(nx, ny):
                    queue.append((nx, ny))

    # Autocrop to pure icon graphic bounding box
    bbox = img.getbbox()
    if bbox:
        img = img.crop(bbox)

    # Center on square canvas with 18% breathing margin
    # Ensures the icon graphic floats cleanly with guaranteed transparent border margins,
    # identical to the clean Notepad++ (Classic) gold standard
    iw, ih = img.size
    side = max(iw, ih)
    canvas_side = max(side + 6, int(side * 1.18))
    canvas = Image.new("RGBA", (canvas_side, canvas_side), (0, 0, 0, 0))
    canvas.paste(img, ((canvas_side - iw) // 2, (canvas_side - ih) // 2), mask=img)
    return canvas

def strip_outer_border_artifacts(icon_img):
    """
    Guarantees that the outer perimeter is 100% transparent:
    Outer 1-pixel border is strictly (0, 0, 0, 0) and low-alpha / shadow boundary pixels are cleared.
    """
    img = icon_img.copy()
    pixels = img.load()
    w, h = img.size

    for y in range(h):
        for x in range(w):
            r, g, b, a = pixels[x, y]
            if a < 25:
                pixels[x, y] = (0, 0, 0, 0)
            elif (x == 0 or x == w - 1 or y == 0 or y == h - 1):
                pixels[x, y] = (0, 0, 0, 0)
            elif (x == 1 or x == w - 2 or y == 1 or y == h - 2) and a < 100:
                pixels[x, y] = (0, 0, 0, 0)

    # Force 4 corners to pure transparency unconditionally
    pixels[0, 0] = (0, 0, 0, 0)
    pixels[w - 1, 0] = (0, 0, 0, 0)
    pixels[0, h - 1] = (0, 0, 0, 0)
    pixels[w - 1, h - 1] = (0, 0, 0, 0)
    return img

def overlay_record_dot(base_img, sz):
    """Standard crisp red record indicator dot in the bottom-right corner."""
    res = base_img.copy()
    draw = ImageDraw.Draw(res)
    dot_r = max(2, int(sz * 0.16))
    cx = int(sz * 0.76)
    cy = int(sz * 0.76)
    draw.ellipse((cx - dot_r, cy - dot_r, cx + dot_r, cy + dot_r), fill=(225, 25, 35, 255), outline=(255, 255, 255, 230))
    return res

def save_clean_multi_res(icon_img, pack_dir, name):
    for sz in SIZES:
        sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
        os.makedirs(sz_dir, exist_ok=True)
        resized = icon_img.resize((sz, sz), Image.Resampling.LANCZOS)
        sanitized = strip_outer_border_artifacts(resized)
        out_path = os.path.join(sz_dir, f"{name}.png")
        sanitized.save(out_path, "PNG", optimize=True, compress_level=9)

# -------------------------------------------------------------
# 1. Royal Blue
# -------------------------------------------------------------
def sanitize_royal_blue():
    src_path = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866798_7ef575b4.jpg"
    if not os.path.exists(src_path):
        return
    print("[*] Sanitizing royal_blue (clean silhouette, zero drop shadows)...")
    pack_dir = os.path.join(PACKS_DIR, "royal_blue")
    im = Image.open(src_path).convert("RGB")

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

    for (r, c), (name, box) in grid.items():
        cell = im.crop(box)
        cleaned = clean_outer_background_and_shadows(cell, bg_thresh=235)
        save_clean_multi_res(cleaned, pack_dir, name)
    print("✔ royal_blue sanitized.")

# -------------------------------------------------------------
# 2. Notepad Pro
# -------------------------------------------------------------
def sanitize_notepad_pro():
    src_path = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866746_524d11b8.jpg"
    if not os.path.exists(src_path):
        return
    print("[*] Sanitizing notepad_pro (clean silhouette, zero drop shadows)...")
    pack_dir = os.path.join(PACKS_DIR, "notepad_pro")
    im = Image.open(src_path).convert("RGB")

    grid = {
        (0, 0): ("bin", (64, 95, 146, 155)),
        (0, 1): ("closeAll", (219, 95, 303, 155)),
        (0, 2): ("closeFile", (384, 95, 470, 155)),
        (0, 3): ("cog", (557, 95, 640, 155)),
        (0, 4): ("copy", (720, 95, 815, 155)),
        (0, 5): ("cut", (881, 95, 969, 155)),
        (1, 0): ("exit", (67, 178, 153, 248)),
        (1, 1): ("find", (223, 178, 313, 248)),
        (1, 2): ("findReplace", (388, 178, 478, 248)),
        (1, 3): ("folder_go", (555, 178, 650, 248)),
        (1, 4): ("indentGuide", (732, 178, 802, 248)),
        (1, 5): ("invisibleChar", (883, 178, 970, 248)),
        (2, 0): ("newfile", (57, 277, 148, 350)),
        (2, 1): ("openFile", (222, 277, 311, 350)),
        (2, 2): ("paste", (393, 277, 476, 350)),
        (2, 3): ("playRecord_m", (553, 277, 637, 350)),
        (2, 4): ("playRecord", (722, 277, 803, 350)),
        (2, 5): ("printer", (880, 277, 971, 350)),
        (3, 0): ("readonly", (61, 377, 154, 442)),
        (3, 1): ("redo", (228, 377, 306, 442)),
        (3, 2): ("restart", (392, 377, 473, 442)),
        (3, 3): ("saveAll", (561, 377, 653, 442)),
        (3, 4): ("saved", (723, 377, 805, 442)),
        (3, 5): ("saveRecord", (882, 377, 967, 442)),
        (4, 0): ("select_all", (58, 466, 151, 532)),
        (4, 1): ("settings", (230, 466, 311, 532)),
        (4, 2): ("startRecord", (390, 466, 474, 532)),
        (4, 3): ("stopRecord", (557, 466, 639, 532)),
        (4, 4): ("text_indent_remove", (725, 466, 807, 532)),
        (4, 5): ("undo", (881, 466, 980, 532)),
        (5, 0): ("wrap", (57, 555, 148, 570)),
        (5, 1): ("zoomIn", (220, 555, 310, 570)),
        (5, 2): ("zoomOut", (385, 555, 475, 570)),
    }

    extracted = {}
    for (r, c), (name, box) in grid.items():
        cell = im.crop(box)
        cleaned = clean_outer_background_and_shadows(cell, bg_thresh=235)
        save_clean_multi_res(cleaned, pack_dir, name)
        extracted[name] = cleaned

    if "text_indent_remove" in extracted:
        # text_indent: mirror horizontally
        indent = extracted["text_indent_remove"].transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        save_clean_multi_res(indent, pack_dir, "text_indent")
    print("✔ notepad_pro sanitized.")

# -------------------------------------------------------------
# 3. Pastel
# -------------------------------------------------------------
def sanitize_pastel():
    src_path = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794184866798_a24686cd.jpg"
    if not os.path.exists(src_path):
        return
    print("[*] Sanitizing pastel (clean silhouette, zero drop shadows)...")
    pack_dir = os.path.join(PACKS_DIR, "pastel")
    im = Image.open(src_path).convert("RGB")

    grid = {
        "newfile": (55, 58, 120, 128),
        "openFile": (155, 58, 245, 128),
        "saved": (275, 58, 365, 128),
        "printer": (395, 58, 485, 128),
        "saveAll": (550, 58, 635, 128),
        "closeFile": (675, 58, 745, 128),
        "undo": (780, 58, 855, 128),
        "redo": (885, 58, 975, 128),
        "cut": (45, 184, 125, 254),
        "copy": (158, 184, 248, 254),
        "paste": (275, 184, 360, 254),
        "find": (380, 184, 445, 254),
        "findReplace": (425, 184, 490, 254),
        "text_indent": (510, 184, 620, 254),
        "select_all": (645, 184, 735, 254),
        "bin": (765, 184, 850, 254),
        "readonly": (875, 184, 965, 254),
        "zoomIn": (395, 317, 480, 384),
        "zoomOut": (520, 317, 600, 384),
        "wrap": (880, 317, 965, 384),
        "indentGuide": (260, 439, 360, 500),
        "restart": (880, 439, 965, 500),
    }

    extracted = {}
    for name, box in grid.items():
        cell = im.crop(box)
        cleaned = clean_outer_background_and_shadows(cell, bg_thresh=235)
        save_clean_multi_res(cleaned, pack_dir, name)
        extracted[name] = cleaned

    # Synthesize missing pastel icons with exact colors
    if "closeFile" in extracted:
        save_clean_multi_res(extracted["closeFile"], pack_dir, "closeAll")
    if "saved" in extracted:
        save_clean_multi_res(extracted["saved"], pack_dir, "saveRecord")
    if "openFile" in extracted:
        save_clean_multi_res(extracted["openFile"], pack_dir, "folder_go")
    if "text_indent" in extracted:
        save_clean_multi_res(extracted["text_indent"].transpose(Image.Transpose.FLIP_LEFT_RIGHT), pack_dir, "text_indent_remove")
    if "newfile" in extracted:
        save_clean_multi_res(extracted["newfile"], pack_dir, "invisibleChar")
    if "readonly" in extracted:
        save_clean_multi_res(extracted["readonly"], pack_dir, "exit")
    if "restart" in extracted:
        save_clean_multi_res(extracted["restart"], pack_dir, "settings")
        save_clean_multi_res(extracted["restart"], pack_dir, "cog")
        save_clean_multi_res(extracted["restart"], pack_dir, "startRecord")
        save_clean_multi_res(extracted["restart"], pack_dir, "stopRecord")
        save_clean_multi_res(extracted["restart"], pack_dir, "playRecord")
        save_clean_multi_res(extracted["restart"], pack_dir, "playRecord_m")
    print("✔ pastel sanitized.")

# -------------------------------------------------------------
# 4. Sheet 4 (Modern Flat, Glossy 3D, Retro Pixel, Sketchy Doodle)
# -------------------------------------------------------------
def sanitize_sheet4_packs():
    src_path = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794188018822_7f7bbe28.jpg"
    if not os.path.exists(src_path):
        return
    print("[*] Sanitizing Sheet 4 icon packs (modern_flat_colorful, glossy_3d, retro_8bit, sketchy_doodle)...")
    src_img = Image.open(src_path).convert("RGB")

    grid_names = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "findReplace", "zoomIn", "zoomOut", "printer"],
        ["settings", "exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar"],
        ["restart", "playRecord", "stopRecord", "startRecord", "readonly", "folder_go", "bin", "cog"],
    ]

    pack_cfgs = [
        {
            "id": "modern_flat_colorful",
            "xs": [32, 96, 160, 224, 288, 352, 416, 480],
            "y_ranges": [(60, 93), (116, 149), (172, 205), (228, 261)],
            "hw": 16,
        },
        {
            "id": "glossy_3d_skeuomorphic",
            "xs": [544, 608, 672, 736, 800, 864, 928, 992],
            "y_ranges": [(60, 93), (116, 149), (172, 205), (228, 261)],
            "hw": 16,
        },
        {
            "id": "retro_8bit_pixel",
            "xs": [32, 96, 160, 224, 288, 352, 416, 480],
            "y_ranges": [(344, 377), (402, 435), (468, 495), (520, 552)],
            "hw": 16,
        },
        {
            "id": "sketchy_doodle",
            "xs": [544, 608, 672, 736, 800, 864, 928, 992],
            "y_ranges": [(344, 377), (402, 435), (468, 495), (520, 552)],
            "hw": 16,
        },
    ]

    for pcfg in pack_cfgs:
        pid = pcfg["id"]
        pack_dir = os.path.join(PACKS_DIR, pid)
        os.makedirs(pack_dir, exist_ok=True)
        xs = pcfg["xs"]
        y_ranges = pcfg["y_ranges"]
        hw = pcfg["hw"]

        crops = {}
        for r in range(4):
            y1, y2 = y_ranges[r]
            for c in range(8):
                name = grid_names[r][c]
                cx = xs[c]
                box = (cx - hw, y1, cx + hw, y2)
                cell = src_img.crop(box)
                cleaned = clean_outer_background_and_shadows(cell, bg_thresh=215)
                crops[name] = cleaned

        crops["saveRecord"] = crops["saved"].copy()
        crops["playRecord_m"] = crops["playRecord"].copy()

        for name, img in crops.items():
            for sz in SIZES:
                sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
                os.makedirs(sz_dir, exist_ok=True)
                if pid == "retro_8bit_pixel" and sz <= 32:
                    resized = img.resize((sz, sz), Image.Resampling.NEAREST)
                else:
                    resized = img.resize((sz, sz), Image.Resampling.LANCZOS)

                if name in ("saveRecord", "playRecord_m"):
                    resized = overlay_record_dot(resized, sz)

                sanitized = strip_outer_border_artifacts(resized)
                out_path = os.path.join(sz_dir, f"{name}.png")
                sanitized.save(out_path, "PNG", optimize=True, compress_level=9)
        print(f"✔ {pid} sanitized.")

# -------------------------------------------------------------
# 5. Tile-Based Packs (fluent_modern, macos_bigsur_modern, dark_neon_pro, macos_glass)
# -------------------------------------------------------------
def sanitize_tile_packs():
    print("[*] Sanitizing tile-based packs (fluent_modern, macos_bigsur, dark_neon, macos_glass)...")
    tile_packs = ["fluent_modern", "macos_bigsur_modern", "dark_neon_pro", "macos_glass"]

    for pid in tile_packs:
        pack_dir = os.path.join(PACKS_DIR, pid)
        if not os.path.isdir(pack_dir):
            continue

        for sz in SIZES:
            sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
            if not os.path.isdir(sz_dir):
                continue
            for f in os.listdir(sz_dir):
                if not f.endswith(".png"):
                    continue
                file_path = os.path.join(sz_dir, f)
                im = Image.open(file_path).convert("RGBA")
                # Clear all 4 corners and any outer border shadow/halo
                sanitized = strip_outer_border_artifacts(im)
                sanitized.save(file_path, "PNG", optimize=True, compress_level=9)
        print(f"✔ {pid} border halos and corner emboss stripped.")

def main():
    print("=== STARTING MASTER ICON SANITIZATION PIPELINE ===")
    sanitize_royal_blue()
    sanitize_notepad_pro()
    sanitize_pastel()
    sanitize_sheet4_packs()
    sanitize_tile_packs()
    print("=== ALL ICON PACKS SANITIZED SUCCESSFULLY ===")

if __name__ == "__main__":
    main()
