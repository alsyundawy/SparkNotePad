#!/usr/bin/env python3
import glob
import os
import re

with open("src/scripts/init.lua", "r", encoding="utf-8") as f:
    content = f.read()

required = set(re.findall(r'require\(["\'](.*?)["\']\)', content))
all_lua = sorted([os.path.basename(p)[:-4] for p in glob.glob("src/languages/*.lua")])

missing = [name for name in all_lua if name not in required]
print(f"Total lua files: {len(all_lua)}")
print(f"Already in init.lua: {len(required)}")
print(f"Missing in init.lua ({len(missing)}): {missing}")

display_names = {
    "asciidoc": "AsciiDoc",
    "bibtex": "BibTeX",
    "bicep": "Bicep",
    "clojure": "Clojure",
    "conf": "Configuration File",
    "crontab": "Crontab",
    "docker": "Dockerfile",
    "eiffel": "Eiffel",
    "elixir": "Elixir",
    "fsharp": "F#",
    "gdscript": "GDScript",
    "glsl": "GLSL Shader",
    "graphql": "GraphQL",
    "groovy": "Groovy",
    "haxe": "Haxe",
    "hex": "Hex Dump",
    "hlsl": "HLSL Shader",
    "julia": "Julia",
    "kotlin": "Kotlin",
    "mermaid": "Mermaid",
    "modula": "Modula",
    "mojo": "Mojo",
    "ocaml": "OCaml",
    "po": "Gettext PO",
    "pov": "POV-Ray",
    "protobuf": "Protocol Buffers",
    "racket": "Racket",
    "raku": "Raku",
    "solidity": "Solidity",
    "stata": "Stata",
    "systemverilog": "SystemVerilog",
    "terraform": "Terraform",
    "troff": "Troff / Roff",
    "wat": "WebAssembly Text",
}

lines = []
for name in missing:
    dname = display_names.get(name, name.capitalize())
    lines.append(f'languages["{dname}"] = require("{name}")')

addition = "\n" + "\n".join(lines) + "\n"
content = content.rstrip() + addition

with open("src/scripts/init.lua", "w", encoding="utf-8") as f:
    f.write(content)

print(f"Successfully appended {len(lines)} languages to src/scripts/init.lua!")
