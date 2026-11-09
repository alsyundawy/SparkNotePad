#!/usr/bin/env python3
"""
Generate 4 Stylized & Retro Toolbar Icon Packs for SparkNotePad (from Sheet 4):
1. modern_flat_colorful (Modern Flat & Colorful)
2. glossy_3d_skeuomorphic (Glossy 3D Skeuomorphic)
3. retro_8bit_pixel (Retro 8-Bit Pixel Art)
4. sketchy_doodle (Sketchy Hand-Drawn Doodle)

Extracts all 34 canonical icons across 6 multi-resolution folders:
16x16, 20x20, 24x24, 32x32, 48x48, 64x64.
"""

import os
from collections import deque
from PIL import Image, ImageDraw

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")
SOURCE_IMAGE = "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794188018822_7f7bbe28.jpg"

SIZES = [16, 20, 24, 32, 48, 64]

GRID = [
    ["newfile", "openFile", "saved", "saveAll", "closeFile", "closeAll", "cut", "copy"],
    ["paste", "undo", "redo", "find", "findReplace", "zoomIn", "zoomOut", "printer"],
    ["settings", "exit", "select_all", "wrap", "text_indent", "text_indent_remove", "indentGuide", "invisibleChar"],
    ["restart", "playRecord", "stopRecord", "startRecord", "readonly", "folder_go", "bin", "cog"],
]

PACK_CONFIGS = [
    {
        "id": "modern_flat_colorful",
        "name": "Modern Flat & Colorful",
        "xs": [32, 96, 160, 224, 288, 352, 416, 480],
        "ys": [76, 132, 188, 244],
        "half_w": 18,
        "half_h": 18,
    },
    {
        "id": "glossy_3d_skeuomorphic",
        "name": "Glossy 3D Skeuomorphic",
        "xs": [544, 608, 672, 736, 800, 864, 928, 992],
        "ys": [76, 132, 188, 244],
        "half_w": 18,
        "half_h": 18,
    },
    {
        "id": "retro_8bit_pixel",
        "name": "Retro 8-Bit Pixel Art",
        "xs": [32, 96, 160, 224, 288, 352, 416, 480],
        "ys": [360, 418, 476, 535],
        "half_w": 18,
        "half_h": 18,
    },
    {
        "id": "sketchy_doodle",
        "name": "Sketchy Hand-Drawn Doodle",
        "xs": [544, 608, 672, 736, 800, 864, 928, 992],
        "ys": [360, 418, 476, 535],
        "half_w": 18,
        "half_h": 18,
    },
]

def remove_background(im, bg_color=(220, 225, 225), tol=22):
    """
    Flood-fill background removal starting from the borders.
    Preserves white/light fills inside the icon graphics.
    Applies soft-edge anti-aliasing near transparent boundaries.
    """
    img = im.convert("RGBA")
    w, h = img.size
    pixels = img.load()

    visited = [[False] * h for _ in range(w)]
    queue = deque()

    def is_bg(x, y):
        r, g, b, _ = pixels[x, y]
        dist = max(abs(r - bg_color[0]), abs(g - bg_color[1]), abs(b - bg_color[2]))
        return dist <= tol

    # Seed with borders
    for x in range(w):
        for y in (0, h - 1):
            if is_bg(x, y):
                visited[x][y] = True
                queue.append((x, y))
    for y in range(h):
        for x in (0, w - 1):
            if not visited[x][y] and is_bg(x, y):
                visited[x][y] = True
                queue.append((x, y))

    # Flood fill outer background to transparent
    while queue:
        cx, cy = queue.popleft()
        pixels[cx, cy] = (0, 0, 0, 0)
        for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            nx, ny = cx + dx, cy + dy
            if 0 <= nx < w and 0 <= ny < h and not visited[nx][ny]:
                visited[nx][ny] = True
                if is_bg(nx, ny):
                    queue.append((nx, ny))

    # Soft edge feathering
    for x in range(w):
        for y in range(h):
            r, g, b, a = pixels[x, y]
            if a > 0:
                dist = max(abs(r - bg_color[0]), abs(g - bg_color[1]), abs(b - bg_color[2]))
                if dist < tol + 18:
                    has_trans_neighbor = any(
                        0 <= x + dx < w and 0 <= y + dy < h and pixels[x + dx, y + dy][3] == 0
                        for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1))
                    )
                    if has_trans_neighbor:
                        alpha_factor = min(1.0, max(0.0, (dist - 6) / (tol + 12)))
                        pixels[x, y] = (r, g, b, int(255 * alpha_factor))

    return img

def overlay_record_dot(base_img, sz):
    """Overlay standard red record indicator dot in the bottom-right quadrant."""
    res = base_img.copy()
    draw = ImageDraw.Draw(res)
    dot_r = max(2, int(sz * 0.16))
    cx = int(sz * 0.76)
    cy = int(sz * 0.76)
    draw.ellipse((cx - dot_r, cy - dot_r, cx + dot_r, cy + dot_r), fill=(225, 25, 35, 255), outline=(255, 255, 255, 230))
    return res

def process_pack(cfg, src_img):
    pack_id = cfg["id"]
    pack_dir = os.path.join(PACKS_DIR, pack_id)
    print(f"[*] Processing pack: {pack_id} ({cfg['name']}) -> {pack_dir}")
    os.makedirs(pack_dir, exist_ok=True)

    xs = cfg["xs"]
    ys = cfg["ys"]
    hw = cfg["half_w"]
    hh = cfg["half_h"]

    # Cache cropped and cleaned 32 base icons
    base_crops = {}
    for r in range(4):
        for c in range(8):
            name = GRID[r][c]
            cx = xs[c]
            cy = ys[r]
            box = (cx - hw, cy - hh, cx + hw, cy + hh)
            raw_crop = src_img.crop(box)
            cleaned = remove_background(raw_crop)
            base_crops[name] = cleaned

    # Synthesize saveRecord (from saved with red record dot)
    base_crops["saveRecord"] = base_crops["saved"].copy()
    # Synthesize playRecord_m (from playRecord with red record dot)
    base_crops["playRecord_m"] = base_crops["playRecord"].copy()

    for sz in SIZES:
        sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
        os.makedirs(sz_dir, exist_ok=True)

        for name, tile in base_crops.items():
            # For retro pixel art, use nearest neighbor resampling if scaling up/down, or Lanczos
            if pack_id == "retro_8bit_pixel":
                resized = tile.resize((sz, sz), Image.Resampling.NEAREST if sz <= 32 else Image.Resampling.LANCZOS)
            else:
                resized = tile.resize((sz, sz), Image.Resampling.LANCZOS)

            if name in ("saveRecord", "playRecord_m"):
                resized = overlay_record_dot(resized, sz)

            # High-efficiency color quantization to keep DMG well below 35MB
            colors = 64 if sz <= 24 else 128
            q = resized.quantize(colors=colors, method=Image.Quantize.FASTOCTREE)
            out_path = os.path.join(sz_dir, f"{name}.png")
            q.save(out_path, "PNG", optimize=True, compress_level=9)

    count = len(os.listdir(os.path.join(pack_dir, "24x24")))
    print(f"✔ Completed {pack_id}: {count} icons across all {len(SIZES)} resolutions.")

def main():
    if not os.path.exists(SOURCE_IMAGE):
        raise FileNotFoundError(f"Source image not found: {SOURCE_IMAGE}")
    src_img = Image.open(SOURCE_IMAGE).convert("RGBA")

    for cfg in PACK_CONFIGS:
        process_pack(cfg, src_img)

    print("All 4 packs (816 PNGs total) generated successfully!")

if __name__ == "__main__":
    main()
