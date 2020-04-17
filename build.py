#!/usr/bin/env python3
import os
import subprocess
import sys
import glob

# TODO: Support other compilers
# TODO: Build 'stdafx.h' as a precompiled header
cc = 'clang'
options = ['-Wall', '-Wextra', '-g']


def run(args):
    completed = subprocess.run(args)
    if completed.returncode != 0:
        print(f"command: {args} exited with code ({completed.returncode})")
        exit(completed.returncode)


def exe_name(name):
    base_name = os.path.splitext(name)[0]
    if sys.platform == "linux":
        return base_name
    elif sys.platform == "win32":
        return base_name + ".exe"
    else:
        print("Unsupported platform: " + sys.platform)
        exit(1)


if __name__ == '__main__':
    base_path = os.path.normpath(__file__ + '/..')
    src_path = os.path.normpath(base_path + '/src')
    bin_path = os.path.normpath(base_path + '/bin')

    # Create './bin'
    if not os.path.exists(bin_path):
        os.makedirs(bin_path)

    out_path = os.path.normpath(bin_path + "/" + exe_name("qk"))
    sources = glob.glob(src_path + '/*.c')
    # TODO: Precompiled headers
    run(['clang', *options, *sources, '-o', out_path])

    if '--run' in sys.argv:
        run([out_path])
