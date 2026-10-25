#!/usr/bin/env python3
"""
Adds compilation database entries for all C++ headers in src/.
This guarantees 100% native resolution in clangd and IDE linters.
"""
import glob
import json
import os
import sys

def main():
    repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    build_dir = os.path.join(repo_root, 'build')
    cc_path = os.path.join(build_dir, 'compile_commands.json')

    if not os.path.exists(cc_path):
        print(f"Error: {cc_path} does not exist", file=sys.stderr)
        return 1

    with open(cc_path, 'r', encoding='utf-8') as f:
        cmds = json.load(f)

    existing_files = {c['file'] for c in cmds}

    template_cmd = None
    for c in cmds:
        if 'src/ApplicationSettings.cpp' in c['file']:
            template_cmd = c
            break

    if not template_cmd and cmds:
        template_cmd = cmds[0]

    src_dir = os.path.join(repo_root, 'src')
    h_files = glob.glob(os.path.join(src_dir, '**', '*.h'), recursive=True)

    added = 0
    for h in sorted(h_files):
        if h not in existing_files:
            base = os.path.splitext(h)[0]
            cpp = base + '.cpp'
            matched_cmd = next((c for c in cmds if c['file'] == cpp), template_cmd)

            cmd_str = matched_cmd['command']
            cmd_str = cmd_str.replace(matched_cmd['file'], h)
            if '-x c++-header' not in cmd_str:
                cmd_str = cmd_str.replace('-std=c++20', '-std=c++20 -x c++-header')

            entry = {
                'directory': matched_cmd['directory'],
                'command': cmd_str,
                'file': h
            }
            cmds.append(entry)
            existing_files.add(h)
            added += 1

    with open(cc_path, 'w', encoding='utf-8') as f:
        json.dump(cmds, f, indent=2)

    root_cc = os.path.join(repo_root, 'compile_commands.json')
    if not os.path.islink(root_cc):
        with open(root_cc, 'w', encoding='utf-8') as f:
            json.dump(cmds, f, indent=2)

    print(f"Successfully synchronized {added} headers in compilation database ({len(cmds)} total entries).")
    return 0

if __name__ == '__main__':
    sys.exit(main())
