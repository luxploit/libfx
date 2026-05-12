#!/usr/bin/env python3
import os
import sys

EXTENSIONS = ['.cpp', '.c', '.cc', '.cxx']

# Usage: meson_glob.py [--no-walk] [directory] [src_dir]
no_walk = '--no-walk' in sys.argv
if no_walk:
    sys.argv.remove('--no-walk')

directory = sys.argv[1] if len(sys.argv) > 1 else '.'
src_dir   = sys.argv[2] if len(sys.argv) > 2 else 'src'

os.chdir(directory)

file_list = []
if no_walk:
    for f in os.listdir(src_dir):
        if os.path.isfile(os.path.join(src_dir, f)) and os.path.splitext(f)[1].lower() in EXTENSIONS:
            rel_path = (src_dir + '/' + f)
            file_list.append(rel_path)
else:
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            if os.path.splitext(f)[1].lower() in EXTENSIONS:
                rel_path = os.path.join(root, f).replace('\\', '/')
                file_list.append(rel_path)

file_list.sort()

for f in file_list:
    print(f)