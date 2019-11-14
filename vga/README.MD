# VGA
All based off of SystemV shared memory regions
display attempts to sync 1/60 of every second, but is efficient if no change is flagged

# Usage
Create the QT binary by opening vga.pro with QT creator and running
Run the binary (it will just hang out)


Generically
```
initialize();
write_byte(x,y,color);
shutdown();
```

# C Bindings
```
#include "vga_bindings.h"
// make sure to compile with vga_bindings.c
```

# Python Bindings
```
./recompile
# this will make vga_bindings.so
```
```
import ctypes
vga = ctypes.cdll.LoadLibrary('./vga_bindings.so')
vga.initialize.restype=ctypes.c_int
vga.write_byte.restype=ctypes.c_int
vga.shutdown.restype=ctypes.c_int


vga.initialize()
for i in xrange(600):
    vga.write_byte(i,i,0xc0c0c0c0)

vga.shutdown()
```
