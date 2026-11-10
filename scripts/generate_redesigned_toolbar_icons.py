#!/usr/bin/env python3
import os
import struct
import zlib


def make_png(width, height, rgba_data):
    def chunk(tag, data):
        return (
            struct.pack(">I", len(data))
            + tag
            + data
            + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
        )

    header = b"\x89PNG\r\n\x1a\n"
    ihdr = chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
    raw = b""
    for y in range(height):
        raw += b"\x00" + bytes(rgba_data[y * width * 4 : (y + 1) * width * 4])
    idat = chunk(b"IDAT", zlib.compress(raw, 9))
    iend = chunk(b"IEND", b"")
    return header + ihdr + idat + iend


class PixelCanvas:
    def __init__(self, w=16, h=16):
        self.w = w
        self.h = h
        self.pixels = [[(0, 0, 0, 0) for _ in range(w)] for _ in range(h)]

    def put(self, x, y, r, g, b, a=255):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.pixels[y][x] = (
                min(255, max(0, int(r))),
                min(255, max(0, int(g))),
                min(255, max(0, int(b))),
                min(255, max(0, int(a))),
            )

    def blend(self, x, y, r, g, b, a=255):
        if 0 <= x < self.w and 0 <= y < self.h:
            sa = a / 255.0
            dr, dg, db, da = (
                self.pixels[y][x][0],
                self.pixels[y][x][1],
                self.pixels[y][x][2],
                self.pixels[y][x][3] / 255.0,
            )
            out_a = sa + da * (1.0 - sa)
            if out_a > 0:
                out_r = (r * sa + dr * da * (1.0 - sa)) / out_a
                out_g = (g * sa + dg * da * (1.0 - sa)) / out_a
                out_b = (b * sa + db * da * (1.0 - sa)) / out_a
                self.pixels[y][x] = (
                    min(255, max(0, round(out_r))),
                    min(255, max(0, round(out_g))),
                    min(255, max(0, round(out_b))),
                    min(255, max(0, round(out_a * 255.0))),
                )

    def to_rgba(self):
        buf = bytearray()
        for y in range(self.h):
            for x in range(self.w):
                buf.extend(self.pixels[y][x])
        return buf

    def get_stats(self):
        vis = [
            self.pixels[y][x]
            for y in range(self.h)
            for x in range(self.w)
            if self.pixels[y][x][3] > 30
        ]
        if not vis:
            return 0, 0.0
        lumas = [(0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2]) for p in vis]
        return len(vis), sum(lumas) / len(lumas)

    def print_preview(self, name):
        print(f"=== {name} ===")
        count, luma = self.get_stats()
        print(f"Visible pixels: {count}/256 | Avg Luma: {luma:.1f}")
        for y in range(self.h):
            row = ""
            for x in range(self.w):
                p = self.pixels[y][x]
                if p[3] < 30:
                    row += "  "
                else:
                    lum = 0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2]
                    if lum > 210:
                        row += "##"
                    elif lum > 140:
                        row += "++"
                    elif lum > 80:
                        row += "**"
                    else:
                        row += ".."
            print(f"{y:2d}: {row}")


def generate_find():
    c = PixelCanvas(16, 16)
    # Lens rim dark contour
    dark_rim = [
        (4, 1),
        (5, 1),
        (6, 1),
        (7, 1),
        (3, 2),
        (8, 2),
        (2, 3),
        (9, 3),
        (1, 4),
        (1, 5),
        (1, 6),
        (1, 7),
        (10, 4),
        (10, 5),
        (10, 6),
        (10, 7),
        (2, 8),
        (9, 8),
        (3, 9),
        (8, 9),
        (4, 10),
        (5, 10),
        (6, 10),
        (7, 10),
    ]
    for x, y in dark_rim:
        c.put(x, y, 15, 23, 42, 220)

    # Lens primary rim (Azure/Cyan gradient)
    rim_azure = [
        (4, 2),
        (5, 2),
        (6, 2),
        (7, 2),
        (3, 3),
        (8, 3),
        (2, 4),
        (9, 4),
        (2, 5),
        (9, 5),
        (2, 6),
        (9, 6),
        (2, 7),
        (9, 7),
        (3, 8),
        (8, 8),
        (4, 9),
        (5, 9),
        (6, 9),
        (7, 9),
    ]
    for x, y in rim_azure:
        c.put(x, y, 2, 132, 199, 255)

    # Highlight rim (upper-left arc)
    c.put(4, 2, 56, 189, 248, 255)
    c.put(5, 2, 56, 189, 248, 255)
    c.put(3, 3, 56, 189, 248, 255)
    c.put(2, 4, 56, 189, 248, 255)
    c.put(2, 5, 56, 189, 248, 255)

    # Specular Gleam on Glass (Crisp White reflection)
    c.put(4, 3, 255, 255, 255, 255)
    c.put(5, 3, 255, 255, 255, 240)
    c.put(3, 4, 255, 255, 255, 255)
    c.put(4, 4, 224, 242, 254, 230)
    c.put(3, 5, 255, 255, 255, 230)

    # Translucent glass fill
    glass = [
        (5, 4),
        (6, 4),
        (7, 4),
        (8, 4),
        (4, 5),
        (5, 5),
        (6, 5),
        (7, 5),
        (8, 5),
        (3, 6),
        (4, 6),
        (5, 6),
        (6, 6),
        (7, 6),
        (8, 6),
        (3, 7),
        (4, 7),
        (5, 7),
        (6, 7),
        (7, 7),
        (8, 7),
        (4, 8),
        (5, 8),
        (6, 8),
        (7, 8),
    ]
    for x, y in glass:
        c.blend(x, y, 186, 230, 253, 140)

    # Metallic Ferrule Connector (Angle at 45 deg)
    c.put(8, 8, 148, 163, 184, 255)
    c.put(9, 8, 203, 213, 225, 255)
    c.put(8, 9, 203, 213, 225, 255)
    c.put(9, 9, 241, 245, 249, 255)
    c.put(10, 9, 100, 116, 139, 255)
    c.put(9, 10, 100, 116, 139, 255)

    # Handle Grip (Rich Amber / Gold with dark contrast borders)
    handle_pixels = [
        (10, 10, 251, 191, 36, 255),
        (11, 10, 245, 158, 11, 255),
        (10, 11, 245, 158, 11, 255),
        (11, 11, 254, 240, 138, 255),
        (12, 11, 217, 119, 6, 255),
        (11, 12, 217, 119, 6, 255),
        (12, 12, 251, 191, 36, 255),
        (13, 12, 180, 83, 9, 255),
        (12, 13, 180, 83, 9, 255),
        (13, 13, 245, 158, 11, 255),
        (14, 13, 120, 53, 15, 255),
        (13, 14, 120, 53, 15, 255),
        (14, 14, 217, 119, 6, 255),
        (15, 14, 69, 26, 3, 220),
        (14, 15, 69, 26, 3, 220),
        (15, 15, 180, 83, 9, 255),
    ]
    for x, y, r, g, b, a in handle_pixels:
        c.put(x, y, r, g, b, a)

    # Handle dark safety contour
    c.blend(11, 9, 15, 23, 42, 180)
    c.blend(9, 11, 15, 23, 42, 180)
    c.blend(12, 10, 15, 23, 42, 180)
    c.blend(10, 12, 15, 23, 42, 180)
    c.blend(13, 11, 15, 23, 42, 180)
    c.blend(11, 13, 15, 23, 42, 180)
    c.blend(14, 12, 15, 23, 42, 180)
    c.blend(12, 14, 15, 23, 42, 180)

    return c


def generate_find_replace():
    c = PixelCanvas(16, 16)
    # Upper-left compact search lens (Center at 4, 4)
    lens_contour = [
        (3, 0),
        (4, 0),
        (5, 0),
        (2, 1),
        (6, 1),
        (1, 2),
        (7, 2),
        (0, 3),
        (0, 4),
        (0, 5),
        (8, 3),
        (8, 4),
        (8, 5),
        (1, 6),
        (7, 6),
        (2, 7),
        (6, 7),
        (3, 8),
        (4, 8),
        (5, 8),
    ]
    for x, y in lens_contour:
        c.put(x, y, 15, 23, 42, 200)

    lens_rim = [
        (3, 1),
        (4, 1),
        (5, 1),
        (2, 2),
        (6, 2),
        (1, 3),
        (7, 3),
        (1, 4),
        (7, 4),
        (1, 5),
        (7, 5),
        (2, 6),
        (6, 6),
        (3, 7),
        (4, 7),
        (5, 7),
    ]
    for x, y in lens_rim:
        c.put(x, y, 2, 132, 199, 255)

    c.put(3, 1, 56, 189, 248, 255)
    c.put(4, 1, 56, 189, 248, 255)
    c.put(2, 2, 56, 189, 248, 255)
    c.put(1, 3, 56, 189, 248, 255)

    c.put(3, 2, 255, 255, 255, 255)
    c.put(2, 3, 255, 255, 255, 255)
    c.put(3, 3, 224, 242, 254, 230)

    for x in range(2, 7):
        for y in range(2, 7):
            if (x, y) not in [(2, 2), (6, 2), (2, 6), (6, 6), (3, 2), (2, 3), (3, 3)]:
                c.blend(x, y, 186, 230, 253, 130)

    c.put(6, 6, 203, 213, 225, 255)
    c.put(7, 7, 251, 191, 36, 255)
    c.put(8, 8, 217, 119, 6, 255)
    c.blend(8, 7, 15, 23, 42, 180)
    c.blend(7, 8, 15, 23, 42, 180)

    # Top Arrow (Replace A->B) in Neon Emerald (#10b981 / #34d399)
    for x in range(6, 12):
        c.put(x, 9, 6, 78, 59, 220)
        c.put(x, 10, 16, 185, 129, 255)
        c.put(x, 11, 6, 78, 59, 220)

    for x in range(7, 11):
        c.blend(x, 10, 209, 250, 229, 140)

    c.put(11, 8, 5, 150, 105, 255)
    c.put(12, 8, 6, 78, 59, 220)
    c.put(12, 9, 52, 211, 153, 255)
    c.put(13, 9, 5, 150, 105, 255)
    c.put(12, 10, 110, 231, 183, 255)
    c.put(13, 10, 255, 255, 255, 255)
    c.put(14, 10, 16, 185, 129, 255)
    c.put(15, 10, 6, 78, 59, 220)
    c.put(12, 11, 52, 211, 153, 255)
    c.put(13, 11, 5, 150, 105, 255)
    c.put(11, 12, 5, 150, 105, 255)
    c.put(12, 12, 6, 78, 59, 220)

    # Bottom Arrow (Leftward Return Arrow) in Vivid Amber/Gold (#f59e0b / #fbbf24)
    for x in range(4, 11):
        c.put(x, 13, 120, 53, 15, 220)
        c.put(x, 14, 245, 158, 11, 255)
        c.put(x, 15, 120, 53, 15, 220)

    for x in range(5, 10):
        c.blend(x, 14, 254, 240, 138, 140)

    c.put(3, 12, 217, 119, 6, 255)
    c.put(2, 12, 120, 53, 15, 220)
    c.put(2, 13, 251, 191, 36, 255)
    c.put(1, 13, 217, 119, 6, 255)
    c.put(2, 14, 254, 240, 138, 255)
    c.put(1, 14, 255, 255, 255, 255)
    c.put(0, 14, 120, 53, 15, 220)
    c.put(2, 15, 251, 191, 36, 255)
    c.put(1, 15, 217, 119, 6, 255)

    return c


def generate_zoom_in():
    c = PixelCanvas(16, 16)
    dark_rim = [
        (4, 1),
        (5, 1),
        (6, 1),
        (7, 1),
        (3, 2),
        (8, 2),
        (2, 3),
        (9, 3),
        (1, 4),
        (1, 5),
        (1, 6),
        (1, 7),
        (10, 4),
        (10, 5),
        (10, 6),
        (10, 7),
        (2, 8),
        (9, 8),
        (3, 9),
        (8, 9),
        (4, 10),
        (5, 10),
        (6, 10),
        (7, 10),
    ]
    for x, y in dark_rim:
        c.put(x, y, 15, 23, 42, 220)

    rim_azure = [
        (4, 2),
        (5, 2),
        (6, 2),
        (7, 2),
        (3, 3),
        (8, 3),
        (2, 4),
        (9, 4),
        (2, 5),
        (9, 5),
        (2, 6),
        (9, 6),
        (2, 7),
        (9, 7),
        (3, 8),
        (8, 8),
        (4, 9),
        (5, 9),
        (6, 9),
        (7, 9),
    ]
    for x, y in rim_azure:
        c.put(x, y, 2, 132, 199, 255)

    c.put(4, 2, 56, 189, 248, 255)
    c.put(5, 2, 56, 189, 248, 255)
    c.put(3, 3, 56, 189, 248, 255)
    c.put(2, 4, 56, 189, 248, 255)
    c.put(2, 5, 56, 189, 248, 255)

    c.put(4, 3, 255, 255, 255, 230)
    c.put(3, 4, 255, 255, 255, 230)

    glass = [
        (5, 3),
        (6, 3),
        (7, 3),
        (4, 4),
        (5, 4),
        (7, 4),
        (8, 4),
        (3, 5),
        (4, 5),
        (5, 5),
        (7, 5),
        (8, 5),
        (3, 6),
        (9, 6),
        (3, 7),
        (4, 7),
        (5, 7),
        (7, 7),
        (8, 7),
        (4, 8),
        (5, 8),
        (6, 8),
        (7, 8),
    ]
    for x, y in glass:
        c.blend(x, y, 186, 230, 253, 120)

    plus_border = [(6, 3), (5, 5), (7, 5), (3, 6), (9, 6), (5, 7), (7, 7), (6, 9)]
    for x, y in plus_border:
        c.blend(x, y, 3, 105, 161, 240)

    for x in range(4, 9):
        c.put(x, 6, 255, 255, 255, 255)
    for y in range(4, 9):
        c.put(6, y, 255, 255, 255, 255)

    c.put(8, 8, 148, 163, 184, 255)
    c.put(9, 8, 203, 213, 225, 255)
    c.put(8, 9, 203, 213, 225, 255)
    c.put(9, 9, 241, 245, 249, 255)
    c.put(10, 9, 100, 116, 139, 255)
    c.put(9, 10, 100, 116, 139, 255)

    handle_pixels = [
        (10, 10, 251, 191, 36, 255),
        (11, 10, 245, 158, 11, 255),
        (10, 11, 245, 158, 11, 255),
        (11, 11, 254, 240, 138, 255),
        (12, 11, 217, 119, 6, 255),
        (11, 12, 217, 119, 6, 255),
        (12, 12, 251, 191, 36, 255),
        (13, 12, 180, 83, 9, 255),
        (12, 13, 180, 83, 9, 255),
        (13, 13, 245, 158, 11, 255),
        (14, 13, 120, 53, 15, 255),
        (13, 14, 120, 53, 15, 255),
        (14, 14, 217, 119, 6, 255),
        (15, 14, 69, 26, 3, 220),
        (14, 15, 69, 26, 3, 220),
        (15, 15, 180, 83, 9, 255),
    ]
    for x, y, r, g, b, a in handle_pixels:
        c.put(x, y, r, g, b, a)

    c.blend(11, 9, 15, 23, 42, 180)
    c.blend(9, 11, 15, 23, 42, 180)
    c.blend(12, 10, 15, 23, 42, 180)
    c.blend(10, 12, 15, 23, 42, 180)
    c.blend(13, 11, 15, 23, 42, 180)
    c.blend(11, 13, 15, 23, 42, 180)

    return c


def generate_zoom_out():
    c = PixelCanvas(16, 16)
    dark_rim = [
        (4, 1),
        (5, 1),
        (6, 1),
        (7, 1),
        (3, 2),
        (8, 2),
        (2, 3),
        (9, 3),
        (1, 4),
        (1, 5),
        (1, 6),
        (1, 7),
        (10, 4),
        (10, 5),
        (10, 6),
        (10, 7),
        (2, 8),
        (9, 8),
        (3, 9),
        (8, 9),
        (4, 10),
        (5, 10),
        (6, 10),
        (7, 10),
    ]
    for x, y in dark_rim:
        c.put(x, y, 15, 23, 42, 220)

    rim_azure = [
        (4, 2),
        (5, 2),
        (6, 2),
        (7, 2),
        (3, 3),
        (8, 3),
        (2, 4),
        (9, 4),
        (2, 5),
        (9, 5),
        (2, 6),
        (9, 6),
        (2, 7),
        (9, 7),
        (3, 8),
        (8, 8),
        (4, 9),
        (5, 9),
        (6, 9),
        (7, 9),
    ]
    for x, y in rim_azure:
        c.put(x, y, 2, 132, 199, 255)

    c.put(4, 2, 56, 189, 248, 255)
    c.put(5, 2, 56, 189, 248, 255)
    c.put(3, 3, 56, 189, 248, 255)
    c.put(2, 4, 56, 189, 248, 255)
    c.put(2, 5, 56, 189, 248, 255)

    c.put(4, 3, 255, 255, 255, 230)
    c.put(3, 4, 255, 255, 255, 230)

    glass = [
        (5, 3),
        (6, 3),
        (7, 3),
        (4, 4),
        (5, 4),
        (6, 4),
        (7, 4),
        (8, 4),
        (3, 5),
        (4, 5),
        (5, 5),
        (6, 5),
        (7, 5),
        (8, 5),
        (3, 6),
        (9, 6),
        (3, 7),
        (4, 7),
        (5, 7),
        (6, 7),
        (7, 7),
        (8, 7),
        (4, 8),
        (5, 8),
        (6, 8),
        (7, 8),
    ]
    for x, y in glass:
        c.blend(x, y, 186, 230, 253, 120)

    for x in range(4, 9):
        c.blend(x, 5, 3, 105, 161, 240)
        c.blend(x, 7, 3, 105, 161, 240)
    c.blend(3, 6, 3, 105, 161, 240)
    c.blend(9, 6, 3, 105, 161, 240)

    for x in range(4, 9):
        c.put(x, 6, 255, 255, 255, 255)

    c.put(8, 8, 148, 163, 184, 255)
    c.put(9, 8, 203, 213, 225, 255)
    c.put(8, 9, 203, 213, 225, 255)
    c.put(9, 9, 241, 245, 249, 255)
    c.put(10, 9, 100, 116, 139, 255)
    c.put(9, 10, 100, 116, 139, 255)

    handle_pixels = [
        (10, 10, 251, 191, 36, 255),
        (11, 10, 245, 158, 11, 255),
        (10, 11, 245, 158, 11, 255),
        (11, 11, 254, 240, 138, 255),
        (12, 11, 217, 119, 6, 255),
        (11, 12, 217, 119, 6, 255),
        (12, 12, 251, 191, 36, 255),
        (13, 12, 180, 83, 9, 255),
        (12, 13, 180, 83, 9, 255),
        (13, 13, 245, 158, 11, 255),
        (14, 13, 120, 53, 15, 255),
        (13, 14, 120, 53, 15, 255),
        (14, 14, 217, 119, 6, 255),
        (15, 14, 69, 26, 3, 220),
        (14, 15, 69, 26, 3, 220),
        (15, 15, 180, 83, 9, 255),
    ]
    for x, y, r, g, b, a in handle_pixels:
        c.put(x, y, r, g, b, a)

    c.blend(11, 9, 15, 23, 42, 180)
    c.blend(9, 11, 15, 23, 42, 180)
    c.blend(12, 10, 15, 23, 42, 180)
    c.blend(10, 12, 15, 23, 42, 180)
    c.blend(13, 11, 15, 23, 42, 180)
    c.blend(11, 13, 15, 23, 42, 180)

    return c


def generate_indent_guide():
    c = PixelCanvas(16, 16)
    # Line 0 (Root Level, y=1..2): Slate Gray with bright highlight
    for x in range(1, 15):
        c.put(x, 1, 15, 23, 42, 200)
        c.put(x, 2, 148, 163, 184, 255)
        c.put(x, 3, 71, 85, 105, 200)
    c.put(1, 2, 226, 232, 240, 255)
    c.put(14, 2, 100, 116, 139, 255)

    # Line 1 (Indent Level 1, y=4..5): Neon Emerald (#10b981 / #34d399)
    for x in range(6, 14):
        c.put(x, 4, 6, 78, 59, 200)
        c.put(x, 5, 16, 185, 129, 255)
        c.put(x, 6, 6, 78, 59, 200)
    c.put(6, 5, 110, 231, 183, 255)

    # Line 2 (Indent Level 2, y=7..8): Electric Cyan (#06b6d4 / #67e8f9)
    for x in range(10, 15):
        c.put(x, 7, 8, 47, 73, 200)
        c.put(x, 8, 6, 182, 212, 255)
        c.put(x, 9, 8, 47, 73, 200)
    c.put(10, 8, 165, 243, 252, 255)

    # Line 3 (Indent Level 2, y=10..11): Electric Cyan (#06b6d4 / #67e8f9)
    for x in range(10, 14):
        c.put(x, 10, 8, 47, 73, 200)
        c.put(x, 11, 6, 182, 212, 255)
        c.put(x, 12, 8, 47, 73, 200)
    c.put(10, 11, 165, 243, 252, 255)

    # Line 4 (Indent Level 1, y=13..14): Neon Emerald (#10b981 / #34d399)
    for x in range(6, 12):
        c.put(x, 13, 6, 78, 59, 200)
        c.put(x, 14, 16, 185, 129, 255)
        c.put(x, 15, 6, 78, 59, 200)
    c.put(6, 14, 110, 231, 183, 255)

    # Vertical Indent Guides at x=3 and x=7
    for y in [3, 4, 6, 7, 9, 10, 12, 13, 14, 15]:
        c.put(3, y, 251, 191, 36, 255)
        c.blend(2, y, 69, 26, 3, 160)
        c.blend(4, y, 69, 26, 3, 160)

    for y in [6, 7, 9, 10, 12, 13]:
        c.put(7, y, 251, 191, 36, 255)
        c.blend(6, y, 69, 26, 3, 160)
        c.blend(8, y, 69, 26, 3, 160)

    return c


if __name__ == "__main__":
    targets = {
        "src/icons/find.png": generate_find(),
        "src/icons/findReplace.png": generate_find_replace(),
        "src/icons/zoomIn.png": generate_zoom_in(),
        "src/icons/zoomOut.png": generate_zoom_out(),
        "src/icons/indentGuide.png": generate_indent_guide(),
    }

    for path, canvas in targets.items():
        name = os.path.basename(path)
        canvas.print_preview(name)
        png_bytes = make_png(canvas.w, canvas.h, canvas.to_rgba())
        with open(path, "wb") as f:
            f.write(png_bytes)
        print(f"Wrote {len(png_bytes)} bytes to {path}\n")
