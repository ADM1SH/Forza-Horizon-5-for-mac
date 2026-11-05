#!/bin/sh
# Build the d3d12.dll proxy with the installed mingw compiler. No Docker, no install.
# Run from anywhere: sh proxy/build.sh
# Output: build/d3d12.dll (ignored by git, never commit a DLL).
# Command used: x86_64-w64-mingw32-gcc -std=c11 -shared -O2 -Wall -Wextra -Werror -nostdlib -Wl,--entry=DllMain ... -lkernel32
# License: MIT (see LICENSE).
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
x86_64-w64-mingw32-gcc -std=c11 -shared -O2 -Wall -Wextra -Werror -nostdlib -Wl,--entry=DllMain \
    -o build/d3d12.dll proxy/d3d12_proxy.c proxy/d3d12.def -lkernel32
shasum -a 256 build/d3d12.dll
