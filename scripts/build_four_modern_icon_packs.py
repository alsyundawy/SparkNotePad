#!/usr/bin/env python3
"""
Generate 4 Modern Toolbar Icon Packs for SparkNotePad:
1. fluent_modern (Fluent Modern Style)
2. macos_bigsur_modern (macOS Big Sur Style)
3. dark_neon_pro (Dark Neon Pro)
4. macos_glass (macOS Style Glass Effect)

Extracts all 34 canonical icons across 6 multi-resolution folders:
16x16, 20x20, 24x24, 32x32, 48x48, 64x64.
"""

import os

from PIL import Image, ImageDraw

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")

SIZES = [16, 20, 24, 32, 48, 64]

GRID_MAP = [
    # Row 0
    ("newfile", (0, 0)),
    ("openFile", (0, 1)),
    ("saved", (0, 2)),
    ("saveAll", (0, 3)),
    ("closeFile", (0, 4)),
    ("closeAll", (0, 5)),
    ("cut", (0, 6)),
    ("copy", (0, 7)),
    # Row 1
    ("paste", (1, 0)),
    ("undo", (1, 1)),
    ("redo", (1, 2)),
    ("find", (1, 3)),
    ("zoomIn", (1, 4)),
    ("zoomOut", (1, 5)),
    ("printer", (1, 6)),
    ("settings", (1, 7)),
    # Row 2
    ("exit", (2, 0)),
    ("select_all", (2, 1)),
    ("wrap", (2, 2)),
    ("text_indent_remove", (2, 3)),
    ("text_indent", (2, 4)),
    ("indentGuide", (2, 5)),
    ("invisibleChar", (2, 6)),
    ("restart", (2, 7)),
    # Row 3
    ("startRecord", (3, 0)),
    ("stopRecord", (3, 1)),
    ("playRecord", (3, 2)),
    ("readonly", (3, 3)),
    ("folder_go", (3, 4)),
    ("bin", (3, 5)),
    ("cog", (3, 6)),
    ("findReplace", (3, 7)),
]

PACK_CONFIGS = [
    {
        "id": "fluent_modern",
        "path": "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794186767664_dcf81c57.png",
        "xs": [60, 135, 210, 285, 360, 435, 510, 585],
        "ys": [132, 208, 286, 364],
        "shape": "squircle",
        "half_crop": 30,
    },
    {
        "id": "macos_bigsur_modern",
        "path": "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794186810712_7b79813e.jpg",
        "xs": [60, 135, 210, 285, 360, 435, 510, 585],
        "ys": [134, 211, 289, 367],
        "shape": "squircle",
        "half_crop": 30,
    },
    {
        "id": "dark_neon_pro",
        "path": "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794186847095_f9ce03bf.png",
        "xs": [60, 134, 208, 283, 357, 431, 506, 580],
        "ys": [132, 208, 285, 362],
        "shape": "squircle",
        "half_crop": 30,
    },
    {
        "id": "macos_glass",
        "path": "/Users/alsyundawy/.gemini/antigravity/brain/5503ca92-405b-46a8-a179-72d16fd57a6d/.user_uploaded/media_1794186867275_8aa5eaf3.png",
        "xs": [60, 134, 208, 283, 357, 431, 506, 580],
        "ys": [134, 211, 289, 367],
        "shape": "circle",
        "half_crop": 30,
    },
]


def make_mask(sz, shape):
    scale = 4
    high_sz = sz * scale
    mask = Image.new("L", (high_sz, high_sz), 0)
    draw = ImageDraw.Draw(mask)
    margin = scale
    if shape == "squircle":
        draw.rounded_rectangle(
            (margin, margin, high_sz - margin, high_sz - margin),
            radius=int(high_sz * 0.22),
            fill=255,
        )
    else:
        draw.ellipse((margin, margin, high_sz - margin, high_sz - margin), fill=255)
    return mask.resize((sz, sz), Image.Resampling.LANCZOS)


def overlay_record_dot(base_img, sz):
    res = base_img.copy()
    draw = ImageDraw.Draw(res)
    dot_r = max(2, int(sz * 0.18))
    cx = int(sz * 0.76)
    cy = int(sz * 0.76)
    draw.ellipse(
        (cx - dot_r, cy - dot_r, cx + dot_r, cy + dot_r),
        fill=(220, 20, 30, 255),
        outline=(255, 255, 255, 230),
    )
    return res


def process_pack(cfg):
    pack_id = cfg["id"]
    pack_dir = os.path.join(PACKS_DIR, pack_id)
    print(f"[*] Processing pack: {pack_id} -> {pack_dir}")
    os.makedirs(pack_dir, exist_ok=True)

    src_img = Image.open(cfg["path"]).convert("RGBA")
    xs = cfg["xs"]
    ys = cfg["ys"]
    half = cfg["half_crop"]
    shape = cfg["shape"]

    # Cache crops of the 32 icons
    base_crops = {}
    for name, (r, c) in GRID_MAP:
        cx = xs[c]
        cy = ys[r]
        box = (cx - half, cy - half, cx + half, cy + half)
        tile = src_img.crop(box)
        base_crops[name] = tile

    # Synthesize saveRecord (from saved with red record dot)
    base_crops["saveRecord"] = base_crops["saved"].copy()
    # Synthesize playRecord_m (from playRecord with red record dot)
    base_crops["playRecord_m"] = base_crops["playRecord"].copy()

    for sz in SIZES:
        sz_dir = os.path.join(pack_dir, f"{sz}x{sz}")
        os.makedirs(sz_dir, exist_ok=True)
        mask = make_mask(sz, shape)

        for name, tile in base_crops.items():
            resized = tile.resize((sz, sz), Image.Resampling.LANCZOS)
            resized.putalpha(mask)

            if name == "saveRecord" or name == "playRecord_m":
                resized = overlay_record_dot(resized, sz)

            # Quantize to palette mode with transparency for optimal size
            colors = 128 if sz <= 24 else 256
            q = resized.quantize(colors=colors, method=Image.Quantize.FASTOCTREE)
            out_path = os.path.join(sz_dir, f"{name}.png")
            q.save(out_path, "PNG", optimize=True, compress_level=9)

    count = len(os.listdir(os.path.join(pack_dir, "24x24")))
    print(f"✔ Completed {pack_id}: {count} icons across all {len(SIZES)} resolutions.")


def main():
    for cfg in PACK_CONFIGS:
        process_pack(cfg)
    print("All 4 packs processed successfully!")


if __name__ == "__main__":
    main()
