#!/bin/bash
gcc -c -fPIC vga_bindings.c -o vga_bindings.o
gcc -shared vga_bindings.o  -o vga_bindings.so
echo "now run python -i test_vga.py"

