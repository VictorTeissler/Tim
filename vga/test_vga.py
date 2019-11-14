#!/usr/bin/env python
import ctypes
vga = ctypes.cdll.LoadLibrary('./vga_bindings.so')
vga.initialize.restype=ctypes.c_int
vga.write_byte.restype=ctypes.c_int
vga.shutdown.restype=ctypes.c_int


vga.initialize()
for i in xrange(600):
    vga.write_byte(i,i,0xc0c0c0c0)

print 'don\'t forget to vga.shutdown()'


