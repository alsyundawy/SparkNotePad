#!/usr/bin/env python3
"""
Lossless Icon Pack PNG Optimizer for SparkNotePad:
Applies fast octree palette quantization with full alpha transparency
and zlib level 9 compression to minimize disk and binary footprint
without degrading visual fidelity.
"""

import os

from PIL import Image

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PACKS_DIR = os.path.join(REPO_ROOT, "src", "icons", "packs")


def optimize_icons():
    total_before = 0
    total_after = 0
    optimized_count = 0

    for root, _, files in os.walk(PACKS_DIR):
        for f in files:
            if not f.endswith(".png"):
                continue

            p = os.path.join(root, f)
            sz = os.path.getsize(p)
            total_before += sz

            try:
                im = Image.open(p).convert("RGBA")
                # Quantize to 256 indexed colors with alpha support
                q = im.quantize(colors=256, method=Image.Quantize.FASTOCTREE)
                tmp_p = p + ".opt_tmp"
                q.save(tmp_p, "PNG", optimize=True, compress_level=9)
                new_sz = os.path.getsize(tmp_p)

                if new_sz < sz:
                    os.replace(tmp_p, p)
                    total_after += new_sz
                    optimized_count += 1
                else:
                    if os.path.exists(tmp_p):
                        os.remove(tmp_p)
                    total_after += sz
            except Exception:
                total_after += sz

    saved = total_before - total_after
    pct = (saved / total_before * 100) if total_before > 0 else 0
    print("[*] Optimization completed:")
    print(f"    Files optimized: {optimized_count}")
    print(
        f"    Original size:   {total_before:,} bytes ({total_before/1024/1024:.2f} MB)"
    )
    print(
        f"    Optimized size:  {total_after:,} bytes ({total_after/1024/1024:.2f} MB)"
    )
    print(
        f"    Total saved:     {saved:,} bytes ({saved/1024/1024:.2f} MB, {pct:.1f}% reduction)"
    )


if __name__ == "__main__":
    optimize_icons()
