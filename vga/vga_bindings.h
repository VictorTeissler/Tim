#ifndef VGA_BINDINGS
#define VGA_BINDINGS

int initialize();
int write_byte(uint32_t x, uint32_t y, uint32_t color);
int shutdown();

#endif

