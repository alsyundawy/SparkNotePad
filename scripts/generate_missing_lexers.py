#!/usr/bin/env python3
import os
from typing import Dict, List, TypedDict


class LanguageDef(TypedDict):
    lexer: str
    comment: str
    extensions: List[str]
    keywords: Dict[int, str]


languages: Dict[str, LanguageDef] = {
    "asciidoc": {
        "lexer": "asciidoc",
        "comment": "// ",
        "extensions": ["adoc", "asciidoc"],
        "keywords": {
            0: "include ifdef ifndef endif tag end link image footnote callout audio video",
            1: "note tip warning caution important caption toc doctype author title",
        },
    },
    "bibtex": {
        "lexer": "bibtex",
        "comment": "% ",
        "extensions": ["bib"],
        "keywords": {
            0: "article book booklet inbook incollection inproceedings manual mastersthesis misc phdthesis proceedings techreport unpublished string comment preamble",
            1: "author title journal year volume number pages month note key publisher editor series address edition booktitle organization howpublished school institution",
        },
    },
    "fsharp": {
        "lexer": "fsharp",
        "comment": "// ",
        "extensions": ["fs", "fsi", "fsx"],
        "keywords": {
            0: "abstract and as assert base begin class default delegate do done downcast downto elif else end exception extern false finally for fun function global if in inherit inline interface internal lazy let match member module mutable namespace new not null of open or override private public rec return static struct then to true try type upcast use val void when while with yield",
            1: "bool byte sbyte int16 uint16 int uint int64 uint64 nativeint unativeint decimal float32 float char string unit obj",
        },
    },
    "eiffel": {
        "lexer": "eiffel",
        "comment": "-- ",
        "extensions": ["e"],
        "keywords": {
            0: "across agent alias all and as assign attribute check class convert create creation current debug deferred do else elseif end ensure expanded export external false feature from frozen if implies indexing inherit inspect invariant is like local loop not note obsolete old once or precondition quotient real_constant redefine rename require rescue result retry select separate then true undefine until variant void when xor",
            1: "BOOLEAN CHARACTER INTEGER REAL STRING ARRAY LIST HASH_TABLE POINTER",
        },
    },
    "modula": {
        "lexer": "modula",
        "comment": "(* *)",
        "extensions": ["m2", "m3", "mod"],
        "keywords": {
            0: "AND ARRAY BEGIN BY CASE CONST DEFINITION DIV DO ELSE ELSIF END EXIT EXPORT FOR FROM IF IMPLEMENTATION IMPORT IN LOOP MOD MODULE NOT OF OR POINTER PROCEDURE QUALIFIED RECORD REPEAT RETURN SET THEN TO TYPE UNTIL VAR WHILE WITH",
            1: "INTEGER CARDINAL REAL BOOLEAN CHAR BITSET WORD ADDRESS PROC NIL TRUE FALSE",
        },
    },
    "po": {
        "lexer": "po",
        "comment": "# ",
        "extensions": ["po", "pot"],
        "keywords": {
            0: "msgid msgid_plural msgstr msgctxt fuzzy",
            1: "c-format python-format qt-format php-format",
        },
    },
    "crontab": {
        "lexer": "crontab",
        "comment": "# ",
        "extensions": ["cron", "crontab"],
        "keywords": {
            0: "@reboot @yearly @annually @monthly @weekly @daily @midnight @hourly SHELL PATH MAILTO HOME",
            1: "jan feb mar apr may jun jul aug sep oct nov dec sun mon tue wed thu fri sat",
        },
    },
    "conf": {
        "lexer": "conf",
        "comment": "# ",
        "extensions": ["conf", "cfg"],
        "keywords": {
            0: "server listen server_name location root index proxy_pass alias error_page access_log include set",
            1: "on off true false yes no enable disable all none",
        },
    },
    "hex": {
        "lexer": "hex",
        "comment": "; ",
        "extensions": ["hex", "bin"],
        "keywords": {
            0: "OFFSET BYTES ASCII DATA CHECKSUM RECORD TYPE",
            1: "START END EXTENDED LINEAR SEGMENT",
        },
    },
    "raku": {
        "lexer": "raku",
        "comment": "# ",
        "extensions": ["raku", "rakumod", "p6", "pl6", "pm6"],
        "keywords": {
            0: "my our has sub method regex token rule role class grammar package module use need unit constant state subset return if elsif else unless given when default while until for loop gather take try catch quiet leave keep undo",
            1: "Int Str Num Rat Bool Array Hash List Set Bag Any Mu",
        },
    },
    "stata": {
        "lexer": "stata",
        "comment": "// ",
        "extensions": ["do", "ado", "dta"],
        "keywords": {
            0: "clear use save merge append drop keep gen egen replace sort by quietly noisily if else foreach forvalues while display quietly regress correlate summarize tabulate list",
            1: "byte int long float double str",
        },
    },
    "gdscript": {
        "lexer": "gdscript",
        "comment": "# ",
        "extensions": ["gd"],
        "keywords": {
            0: "if elif else for while match break continue pass return class class_name extends is as self tool signal func static const enum var onready export setget breakpoint",
            1: "void bool int float String Vector2 Vector3 Rect2 Transform Transform2D Color Plane Quat AABB Basis Node Node2D Spatial Viewport CanvasItem",
        },
    },
    "clojure": {
        "lexer": "lisp",
        "comment": "; ",
        "extensions": ["clj", "cljs", "cljc", "edn"],
        "keywords": {
            0: "def defn defmacro defmulti defmethod defprotocol defrecord deftype fn let loop recur if if-not when when-not do case cond try catch finally ns require use import quote",
            1: "true false nil vector list hash-map hash-set atom ref agent deref conj assoc dissoc get count map filter reduce",
        },
    },
    "elixir": {
        "lexer": "ruby",
        "comment": "# ",
        "extensions": ["ex", "exs"],
        "keywords": {
            0: "def defp defmodule defprotocol defimpl defmacro defstruct quote unquote do end if else unless cond case with try catch after rescue receive fn",
            1: "true false nil and or not when in use alias import require",
        },
    },
    "groovy": {
        "lexer": "java",
        "comment": "// ",
        "extensions": ["groovy", "gvy", "gy", "gsh"],
        "keywords": {
            0: "as assert def trait it in class interface enum extends implements package import public protected private static final abstract transient volatile native strictfp synchronized return if else switch case default for while do break continue throw throws try catch finally new this super",
            1: "byte short char int long float double boolean void String Object List Map Set BigInteger BigDecimal",
        },
    },
    "haxe": {
        "lexer": "cpp",
        "comment": "// ",
        "extensions": ["hx"],
        "keywords": {
            0: "class interface enum typedef abstract function var final public private static inline override dynamic untyped new this super cast return if else while do for break continue switch case default try catch throw import using package",
            1: "Void Int Float Bool String Dynamic Null Array Map",
        },
    },
    "ocaml": {
        "lexer": "caml",
        "comment": "(* *)",
        "extensions": ["ml", "mli"],
        "keywords": {
            0: "and as assert begin class constraint do done downto else end exception external false for fun function functor if in include inherit initializer land lazy let lor lsl lsr lxor match method mod module mutable new nonrec object of open or private rec sig struct then to true try type val virtual when while with",
            1: "int float bool char string unit list array option",
        },
    },
    "racket": {
        "lexer": "scheme",
        "comment": "; ",
        "extensions": ["rkt"],
        "keywords": {
            0: "define define-syntax lambda let let* letrec if cond case begin and or set! delay quote quasiquote unquote unquote-splicing syntax-rules provide require struct class interface",
            1: "#t #f null cons car cdr list vector string boolean? number? pair? symbol?",
        },
    },
    "glsl": {
        "lexer": "cpp",
        "comment": "// ",
        "extensions": ["glsl", "vert", "frag", "geom", "comp"],
        "keywords": {
            0: "attribute const uniform varying layout centroid flat smooth noperspective in out inout break continue do for while switch case default if else discard return struct precision highp mediump lowp",
            1: "void bool int uint float vec2 vec3 vec4 bvec2 bvec3 bvec4 ivec2 ivec3 ivec4 uvec2 uvec3 uvec4 mat2 mat3 mat4 sampler2D sampler3D samplerCube",
        },
    },
    "hlsl": {
        "lexer": "cpp",
        "comment": "// ",
        "extensions": ["hlsl", "fx"],
        "keywords": {
            0: "cbuffer tbuffer register packoffset texture sampler SamplerState Texture2D TextureCube struct typedef return if else for while do switch case default break continue discard inline uniform volatile",
            1: "void bool int uint float double half min16float min10float min16int min12int min16uint vector matrix float2 float3 float4 int2 int3 int4 uint2 uint3 uint4",
        },
    },
    "systemverilog": {
        "lexer": "verilog",
        "comment": "// ",
        "extensions": ["sv", "svh"],
        "keywords": {
            0: "always always_comb always_ff always_latch and assign begin break case casex casez class clocking config const constraint context continue covergroup default defparam design disable do edge else end endcase endclass endclocking endconfig endfunction endgenerate endinterface endmodule endpackage endprimitive endprogram endproperty endspecify endtable endtask enum event export extern final for force foreach forever fork forkjoin function generate genvar if iff ifnone import initial inout input inside instance integer interface intersect join join_any join_none local localparam logic macromodule matches modport module nand negedge new nmos nor noshowcancelled not notif0 notif1 or output package packed parameter pmos posedge primitive priority program property protected pull0 pull1 pulldown pullup rcmos real realtime ref reg release repeat return rnmos rpmos rtran rtranif0 rtranif1 scalared sequence shortint shortreal showcancelled signed small solve specify specparam static string strong0 strong1 struct super supply0 supply1 table tagged task this time timeprecision timeunit tran tranif0 tranif1 tri tri0 tri1 triand trior trireg type typedef union unique unique0 unsigned use uwire var vectored virtual void wait wait_order wand weak0 weak1 while wildcard wire with wor xnor xor",
            1: "logic bit byte int shortint longint integer time real realtime string",
        },
    },
    "troff": {
        "lexer": "troff",
        "comment": '.\\" ',
        "extensions": ["tr", "roff", "man"],
        "keywords": {
            0: "TH SH SS PP LP IP TP HP RS RE B I BI BR IB RB IR RI SM SB",
            1: "so ps ft vs in ll ls ti nf fi ce br sp",
        },
    },
    "pov": {
        "lexer": "pov",
        "comment": "// ",
        "extensions": ["pov", "inc"],
        "keywords": {
            0: "camera light_source object background sphere box cylinder cone plane torus blob mesh texture pigment finish normal interior color rgb rgbf rgbt rotate translate scale union intersection difference merge declare default version",
            1: "ambient diffuse specular roughness reflection refraction phong metallic",
        },
    },
}

base_dir = "src/languages"
for name, data in languages.items():
    path = os.path.join(base_dir, f"{name}.lua")
    kw0 = data["keywords"].get(0, "")
    kw1 = data["keywords"].get(1, "")
    exts = ",\n\t".join(f'"{e}"' for e in data["extensions"])
    content = f"""local L = {{}}

L.lexer = "{data['lexer']}"

L.singleLineComment = "{data['comment']}"

L.extensions = {{
\t{exts},
}}

L.keywords = {{
\t[0] = "{kw0}",
\t[1] = "{kw1}",
}}

return L
"""
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"Generated {path}")
