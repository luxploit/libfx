import os
import sys

filename = sys.argv[1] if len(sys.argv) > 1 else 'meson.build'
os.utime(filename)