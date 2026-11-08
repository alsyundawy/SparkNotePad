local L = {}

L.name = "Text"

L.lexer = "null"

L.disableFoldMargin = true

L.extensions = {
    "",
    "txt",
    "text",
    "log",
    "1st",
    "ans",
    "asc",
    "ascii",
    "diz",
    "err",
    "me",
    "msg",
    "readme",
    "sub",
    "srt",
    "vtt",
    "uris",
    "uri",
    "spec",
    "rst",
    "csv",
    "tsv",
    "in",
    "def",
}

L.styles = {
    ["DEFAULT"] = {
        id = 0,
        fgColor = rgb(0x000000),
        bgColor = rgb(0xFFFFFF),
        fontStyle = 0,
    },
}

return L
