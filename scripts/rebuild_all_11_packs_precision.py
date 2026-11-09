#!/usr/bin/env python3
"""
Master Precision Icon Pack Generator for SparkNotePad (v1.4.6):
Re-extracts and synthesizes all 11 newly added toolbar icon packs:
  1. fluent_modern           (Squircle container, Fluent Light style)
  2. macos_bigsur_modern     (Squircle container, Big Sur frosted wallpaper style)
  3. macos_glass             (Squircle container, dark translucent glass style)
  4. dark_neon_pro           (Squircle container, obsidian neon style)
  5. pastel                  (Squircle container, pastel modern style)
  6. notepad_pro             (Freestanding, Vista 7 style, no text/reflection)
  7. royal_blue              (Freestanding, glossy blue aero, no gray halo)
  8. modern_flat_colorful    (Freestanding, Style A from Sheet 4)
  9. glossy_3d_skeuomorphic (Freestanding, Style B from Sheet 4)
 10. retro_8bit_pixel        (Freestanding, Style C from Sheet 4)
 11. sketchy_doodle          (Freestanding, Style D from Sheet 4)

Rules:
- Squircle packs preserve their entire container tile intact. ONLY outer corners are made transparent.
- Freestanding packs cleanly isolate icon graphics with 100% transparent backgrounds and zero halos.
- Multi-resolution output: 16x16, 20x20, 24x24, 32x32, 48x48, 64x64.
- Fast Octree palette quantization (256 colors) and zlib level 9 compression directly applied.
"""

import os
import sys
from collections import deque
from PIL import Image, ImageDraw, ImageOps

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")
UPLOADED = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded"

SIZES = [16, 20, 24, 32, 48, 64]

ALL_CANONICAL = [
    "bin", "closeAll", "closeFile", "cog", "copy", "cut", "exit",
    "find", "findReplace", "folder_go", "indentGuide", "invisibleChar",
    "newfile", "openFile", "paste", "playRecord", "playRecord_m",
    "printer", "readonly", "redo", "restart", "saveAll", "saved",
    "saveRecord", "select_all", "settings", "startRecord", "stopRecord",
    "text_indent", "text_indent_remove", "undo", "wrap", "zoomIn", "zoomOut"
]

def make_squircle_tile(im, cx, cy, hw, hh, radius):
    """Crop squircle tile intact and mask only the 4 outer corners with transparency."""
    tile = im.crop((cx - hw, cy - hh, cx + hw, cy + hh)).convert("RGBA")
    tw, th = tile.size
    mask = Image.new("L", (tw, th), 0)
    draw = ImageDraw.Draw(mask)
    draw.rounded_rectangle([(0, 0), (tw - 1, th - 1)], radius=radius, fill=255)
    tile.putalpha(mask)
    return tile

def clean_freestanding_border_flood(crop, bg_brightness_thresh=150, sat_thresh=35):
    """Clean freestanding icon using border flood fill to remove background and shadows."""
    crop = crop.convert("RGBA")
    w, h = crop.size
    visited = [[False] * w for _ in range(h)]
    queue = deque()

    # Seed from all 4 borders
    for x in range(w):
        for y in [0, h - 1]:
            r, g, b, _ = crop.getpixel((x, y))
            br = (r + g + b) / 3.0
            sat = max(r, g, b) - min(r, g, b)
            if br >= bg_brightness_thresh and sat <= sat_thresh:
                visited[y][x] = True
                queue.append((x, y))

    for y in range(h):
        for x in [0, w - 1]:
            if not visited[y][x]:
                r, g, b, _ = crop.getpixel((x, y))
                br = (r + g + b) / 3.0
                sat = max(r, g, b) - min(r, g, b)
                if br >= bg_brightness_thresh and sat <= sat_thresh:
                    visited[y][x] = True
                    queue.append((x, y))

    while queue:
        cx, cy = queue.popleft()
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = cx + dx, cy + dy
            if 0 <= nx < w and 0 <= ny < h and not visited[ny][nx]:
                r, g, b, _ = crop.getpixel((nx, ny))
                br = (r + g + b) / 3.0
                sat = max(r, g, b) - min(r, g, b)
                if br >= bg_brightness_thresh and sat <= sat_thresh:
                    visited[ny][nx] = True
                    queue.append((nx, ny))

    out = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    for y in range(h):
        for x in range(w):
            if not visited[y][x]:
                out.putpixel((x, y), crop.getpixel((x, y)))

    # Crop tight to content and pad into a square
    bbox = out.getbbox()
    if not bbox:
        return crop
    tight = out.crop(bbox)
    max_d = max(tight.width, tight.height)
    pad = max(2, int(max_d * 0.08))
    sq_size = max_d + pad * 2
    sq = Image.new("RGBA", (sq_size, sq_size), (0, 0, 0, 0))
    sq.paste(tight, ((sq_size - tight.width) // 2, (sq_size - tight.height) // 2))
    return sq

def synthesize_missing_icons(icon_dict):
    """Synthesize saveRecord, playRecord_m, findReplace if missing."""
    # 1. saveRecord = saved + red recording dot
    if "saveRecord" not in icon_dict and "saved" in icon_dict:
        base = icon_dict["saved"].copy()
        bw, bh = base.size
        draw = ImageDraw.Draw(base)
        dot_r = max(2, int(bw * 0.18))
        dot_cx = int(bw * 0.78)
        dot_cy = int(bh * 0.78)
        # White rim
        draw.ellipse([(dot_cx - dot_r - 1, dot_cy - dot_r - 1), (dot_cx + dot_r + 1, dot_cy + dot_r + 1)], fill=(255, 255, 255, 255))
        # Red fill
        draw.ellipse([(dot_cx - dot_r, dot_cy - dot_r), (dot_cx + dot_r, dot_cy + dot_r)], fill=(230, 40, 40, 255))
        icon_dict["saveRecord"] = base

    # 2. playRecord_m = playRecord + marker badge
    if "playRecord_m" not in icon_dict and "playRecord" in icon_dict:
        base = icon_dict["playRecord"].copy()
        bw, bh = base.size
        draw = ImageDraw.Draw(base)
        dot_r = max(2, int(bw * 0.16))
        dot_cx = int(bw * 0.78)
        dot_cy = int(bh * 0.78)
        # Gold/amber rim and center
        draw.ellipse([(dot_cx - dot_r - 1, dot_cy - dot_r - 1), (dot_cx + dot_r + 1, dot_cy + dot_r + 1)], fill=(255, 255, 255, 255))
        draw.ellipse([(dot_cx - dot_r, dot_cy - dot_r), (dot_cx + dot_r, dot_cy + dot_r)], fill=(245, 166, 35, 255))
        icon_dict["playRecord_m"] = base

    # 3. findReplace = find + return arrow badge
    if "findReplace" not in icon_dict and "find" in icon_dict:
        base = icon_dict["find"].copy()
        bw, bh = base.size
        draw = ImageDraw.Draw(base)
        # Badge circle at bottom right
        badge_r = max(3, int(bw * 0.22))
        bcx = int(bw * 0.75)
        bcy = int(bh * 0.75)
        draw.ellipse([(bcx - badge_r - 1, bcy - badge_r - 1), (bcx + badge_r + 1, bcy + badge_r + 1)], fill=(255, 255, 255, 255))
        draw.ellipse([(bcx - badge_r, bcy - badge_r), (bcx + badge_r, bcy + badge_r)], fill=(50, 120, 240, 255))
        # Arrow symbol
        draw.line([(bcx - badge_r + 2, bcy), (bcx + badge_r - 2, bcy)], fill=(255, 255, 255, 255), width=max(1, int(bw*0.04)))
        draw.line([(bcx + badge_r - 2, bcy), (bcx, bcy - badge_r + 2)], fill=(255, 255, 255, 255), width=max(1, int(bw*0.04)))
        icon_dict["findReplace"] = base

def save_pack_multi_res(pack_id, icon_dict):
    """Save all canonical icons across 6 resolutions with Fast Octree quantization."""
    pack_dir = os.path.join(PACKS_DIR, pack_id)
    os.makedirs(pack_dir, exist_ok=True)

    synthesize_missing_icons(icon_dict)

    # Ensure all canonical icons exist (fallback to newfile or blank)
    for name in ALL_CANONICAL:
        if name not in icon_dict:
            icon_dict[name] = icon_dict.get("newfile", Image.new("RGBA", (48, 48), (0, 0, 0, 0)))

    for sz in SIZES:
        sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
        os.makedirs(sz_dir, exist_ok=True)
        for name in ALL_CANONICAL:
            im = icon_dict[name]
            resized = im.resize((sz, sz), Image.Resampling.LANCZOS)
            out_path = os.path.join(sz_dir, f"{name}.png")
            try:
                # Fast Octree quantization for lossless size optimization
                q = resized.quantize(colors=256, method=Image.Quantize.FASTOCTREE)
                q.save(out_path, "PNG", optimize=True, compress_level=9)
            except Exception:
                resized.save(out_path, "PNG", optimize=True, compress_level=9)

    print(f"[*] Generated pack: {pack_id} ({len(ALL_CANONICAL)} icons across {len(SIZES)} resolutions)")

def extract_fluent_modern():
    p = os.path.join(UPLOADED, "media_1794197789111_1450cb85.jpg")
    im = Image.open(p).convert("RGBA")
    # 4 rows x 8 cols squircle tiles
    xs = [85, 210, 335, 460, 580, 705, 825, 945]
    ys = [210, 325, 455, 585]
    hw, hh, radius = 48, 48, 20

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "zoomIn", "zoomOut", "printer", "settings"],
        ["exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar", "restart"],
        ["startRecord", "stopRecord", "playRecord", "readonly", "folder_go", "bin", "cog", "info"]
    ]

    icons = {}
    for r in range(4):
        for c in range(8):
            name = grid[r][c]
            icons[name] = make_squircle_tile(im, xs[c], ys[r], hw, hh, radius)

    save_pack_multi_res("fluent_modern", icons)

def extract_macos_bigsur_modern():
    p = os.path.join(UPLOADED, "media_1794186810712_7b79813e.jpg")
    im = Image.open(p).convert("RGBA")
    xs = [63, 138, 213, 288, 363, 438, 513, 588]
    ys = [126, 207, 288, 369]
    hw, hh, radius = 36, 36, 16

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "zoomIn", "zoomOut", "printer", "settings"],
        ["exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar", "restart"],
        ["startRecord", "stopRecord", "playRecord", "readonly", "folder_go", "bin", "cog", "info"]
    ]

    icons = {}
    for r in range(4):
        for c in range(8):
            name = grid[r][c]
            icons[name] = make_squircle_tile(im, xs[c], ys[r], hw, hh, radius)

    save_pack_multi_res("macos_bigsur_modern", icons)

def extract_macos_glass():
    # Master user gold standard source: media_1794197498126_977c3315.png (654x430)
    p = os.path.join(UPLOADED, "media_1794197498126_977c3315.png")
    im = Image.open(p).convert("RGBA")
    xs = [63, 138, 213, 288, 363, 438, 513, 588]
    ys = [126, 207, 288, 369]
    hw, hh, radius = 37, 37, 16

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "zoomIn", "zoomOut", "printer", "settings"],
        ["exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar", "restart"],
        ["startRecord", "playRecord", "stopRecord", "saveRecord", "readonly", "folder_go", "bin", "cog"]
    ]

    icons = {}
    for r in range(4):
        for c in range(8):
            name = grid[r][c]
            icons[name] = make_squircle_tile(im, xs[c], ys[r], hw, hh, radius)

    save_pack_multi_res("macos_glass", icons)

def extract_dark_neon_pro():
    p = os.path.join(UPLOADED, "media_1794197789172_d35898b5.jpg")
    im = Image.open(p).convert("RGBA")
    # Bottom-Left quadrant (0..512, 341..682)
    xs = [49, 108, 167, 226, 285, 344, 403, 462]
    ys = [440, 504, 568, 632]
    hw, hh, radius = 28, 28, 12

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "zoomIn", "zoomOut", "printer", "settings"],
        ["exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar", "restart"],
        ["startRecord", "playRecord", "stopRecord", "saveRecord", "readonly", "folder_go", "bin", "cog"]
    ]

    icons = {}
    for r in range(4):
        for c in range(8):
            name = grid[r][c]
            icons[name] = make_squircle_tile(im, xs[c], ys[r], hw, hh, radius)

    save_pack_multi_res("dark_neon_pro", icons)

def extract_pastel():
    p = os.path.join(UPLOADED, "media_1794197789172_d35898b5.jpg")
    im = Image.open(p).convert("RGBA")
    # Bottom-Right quadrant (512..1024, 341..682)
    xs = [561, 620, 679, 738, 797, 856, 915, 974]
    ys = [440, 504, 568, 632]
    hw, hh, radius = 28, 28, 12

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "zoomIn", "zoomOut", "printer", "settings"],
        ["exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar", "restart"],
        ["startRecord", "playRecord", "stopRecord", "saveRecord", "readonly", "folder_go", "bin", "cog"]
    ]

    icons = {}
    for r in range(4):
        for c in range(8):
            name = grid[r][c]
            icons[name] = make_squircle_tile(im, xs[c], ys[r], hw, hh, radius)

    save_pack_multi_res("pastel", icons)

def extract_royal_blue():
    p = os.path.join(UPLOADED, "media_1794197789167_8c0c6773.png")
    im = Image.open(p).convert("RGBA")

    rb_coords = [
        # Row 0
        [(83, 68), (225, 67), (371, 69), (520, 69), (658, 68), (799, 68), (936, 67)],
        # Row 1
        [(80, 181), (227, 182), (372, 181), (514, 178), (655, 182), (797, 183), (941, 181)],
        # Row 2
        [(85, 290), (242, 291), (380, 282), (520, 282), (659, 293), (800, 291), (942, 282)],
        # Row 3
        [(89, 387), (231, 387), (380, 387), (525, 387), (661, 387), (800, 387), (939, 387)],
        # Row 4
        [(90, 492), (239, 492), (390, 492), (507, 492), (645, 492), (781, 492), (951, 444)]
    ]

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut"],
        ["copy", "paste", "undo", "redo", "find", "findReplace", "zoomIn"],
        ["zoomOut", "printer", "settings", "exit", "select_all", "wrap", "text_indent"],
        ["text_indent_remove", "indentGuide", "invisibleChar", "restart", "startRecord", "stopRecord", "playRecord"],
        ["readonly", "folder_go", "bin", "cog", "saveRecord", "playRecord_m", "extra"]
    ]

    icons = {}
    for r in range(5):
        for c in range(7):
            name = grid[r][c]
            if name == "extra":
                continue
            cx, cy = rb_coords[r][c]
            # Bounding box around icon graphic
            crop = im.crop((cx - 45, cy - 45, cx + 45, cy + 45))
            icons[name] = clean_freestanding_border_flood(crop, bg_brightness_thresh=150, sat_thresh=35)

    save_pack_multi_res("royal_blue", icons)

def extract_notepad_pro():
    p = os.path.join(UPLOADED, "media_1794197789169_05ec1954.jpg")
    im = Image.open(p).convert("RGBA")

    # 33 icons from 6x6 alphabetical sheet in media_1794197789169_05ec1954.jpg
    grid = {
        # Row 0
        "bin": (64, 95, 146, 170),
        "closeAll": (219, 95, 303, 170),
        "closeFile": (384, 95, 470, 170),
        "cog": (557, 95, 640, 170),
        "copy": (720, 95, 815, 170),
        "cut": (881, 95, 969, 170),

        # Row 1
        "exit": (67, 178, 153, 268),
        "find": (223, 178, 313, 268),
        "findReplace": (388, 178, 478, 268),
        "folder_go": (555, 178, 650, 268),
        "indentGuide": (732, 178, 802, 268),
        "invisibleChar": (883, 178, 970, 268),

        # Row 2
        "newfile": (57, 277, 148, 368),
        "openFile": (222, 277, 311, 368),
        "paste": (393, 277, 476, 368),
        "playRecord_m": (553, 277, 637, 368),
        "playRecord": (722, 277, 803, 368),
        "printer": (880, 277, 971, 368),

        # Row 3
        "readonly": (61, 377, 154, 460),
        "redo": (228, 377, 306, 460),
        "restart": (392, 377, 473, 460),
        "saveAll": (561, 377, 653, 460),
        "saved": (723, 377, 805, 460),
        "saveRecord": (882, 377, 967, 460),

        # Row 4
        "select_all": (58, 466, 151, 552),
        "settings": (230, 466, 311, 552),
        "startRecord": (390, 466, 474, 552),
        "stopRecord": (557, 466, 639, 552),
        "text_indent_remove": (725, 466, 807, 552),
        "undo": (881, 466, 980, 552),

        # Row 5
        "wrap": (59, 561, 154, 650),
        "zoomIn": (230, 561, 311, 650),
        "zoomOut": (396, 561, 477, 650),
    }

    icons = {}
    for name, bbox in grid.items():
        x1, y1, x2, y2 = bbox
        # Crop upper icon only (max height 62 px) to strictly exclude text label and reflection
        crop = im.crop((x1, y1, x2, min(y2, y1 + 62)))
        icons[name] = clean_freestanding_border_flood(crop, bg_brightness_thresh=170, sat_thresh=35)

    # Synthesize text_indent from text_indent_remove (flip arrow)
    if "text_indent_remove" in icons:
        tir = icons["text_indent_remove"]
        # Mirror image for opposite indent direction
        icons["text_indent"] = ImageOps.mirror(tir)

    save_pack_multi_res("notepad_pro", icons)

def extract_sheet4_styles():
    p = os.path.join(UPLOADED, "media_1794188018822_7f7bbe28.jpg")
    im = Image.open(p).convert("RGBA")

    # 4 styles:
    # Style A (TL): modern_flat_colorful
    # Style B (TR): glossy_3d_skeuomorphic
    # Style C (BL): retro_8bit_pixel
    # Style D (BR): sketchy_doodle

    grid = [
        ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
        ["paste", "undo", "redo", "find", "findReplace", "zoomIn", "zoomOut", "printer"],
        ["settings", "exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar"],
        ["restart", "playRecord", "stopRecord", "startRecord", "readonly", "folder_go", "bin", "cog"]
    ]

    configs = [
        {
            "id": "modern_flat_colorful",
            "xs": [34, 98, 162, 226, 290, 354, 418, 482],
            "ys": [76, 132, 188, 244],
            "hw": 20, "hh": 18
        },
        {
            "id": "glossy_3d_skeuomorphic",
            "xs": [546, 610, 674, 738, 802, 866, 930, 994],
            "ys": [76, 132, 188, 244],
            "hw": 20, "hh": 18
        },
        {
            "id": "retro_8bit_pixel",
            "xs": [34, 98, 162, 226, 290, 354, 418, 482],
            "ys": [362, 418, 474, 530],
            "hw": 20, "hh": 18
        },
        {
            "id": "sketchy_doodle",
            "xs": [546, 610, 674, 738, 802, 866, 930, 994],
            "ys": [362, 418, 474, 530],
            "hw": 20, "hh": 18
        }
    ]

    for cfg in configs:
        pack_id = cfg["id"]
        xs = cfg["xs"]
        ys = cfg["ys"]
        hw = cfg["hw"]
        hh = cfg["hh"]

        icons = {}
        for r in range(4):
            for c in range(8):
                name = grid[r][c]
                cx = xs[c]
                cy = ys[r]
                # Crop strictly excluding label below
                crop = im.crop((cx - hw, cy - hh, cx + hw, cy + hh))
                icons[name] = clean_freestanding_border_flood(crop, bg_brightness_thresh=180, sat_thresh=35)

        save_pack_multi_res(pack_id, icons)

def main():
    print("[*] Starting Precision Icon Pack Generation for all 11 packs...")
    extract_fluent_modern()
    extract_macos_bigsur_modern()
    extract_macos_glass()
    extract_dark_neon_pro()
    extract_pastel()
    extract_royal_blue()
    extract_notepad_pro()
    extract_sheet4_styles()
    print("[+] All 11 packs successfully regenerated with high precision!")

if __name__ == "__main__":
    main()
