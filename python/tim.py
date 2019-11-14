#/usr/bin/env python

Censored_pid = 3836



lib_path = '/home/pancho/code/personal/tim2/tim/bin/tim.so'
print("running from the following so: [%s]" % lib_path)
# print("missing corcect pid,using %d" % Censored_pid)


import ctypes
tim = ctypes.cdll.LoadLibrary(lib_path)
# uint64_t pr64(uint64_t address);
tim.pr64.argtypes = [ctypes.c_ulong]
tim.pr64.restype = ctypes.c_ulong
r64 = tim.pr64
# uint64_t pr32(uint64_t address);
tim.pr32.argtypes = [ctypes.c_ulong]
tim.pr32.restype = ctypes.c_uint
r32 = tim.pr32
# uint64_t pr16(uint64_t address);
tim.pr16.argtypes = [ctypes.c_ulong]
tim.pr16.restype = ctypes.c_ulong
r16 = tim.pr16
# uint64_t pr8(uint64_t address);
tim.pr8.argtypes = [ctypes.c_ulong]
tim.pr8.restype = ctypes.c_byte
r8 = tim.pr8

# void pw64(uint64_t address, uint64_t val);
tim.pw64.argtypes = [ctypes.c_ulong, ctypes.c_ulong]
w64 = tim.pw64
# void pw32(uint64_t address, uint64_t val);
tim.pw32.argtypes = [ctypes.c_ulong, ctypes.c_uint]
w32 = tim.pw32
# void pw16(uint64_t address, uint64_t val);
tim.pw16.argtypes = [ctypes.c_ulong, ctypes.c_uint]
w16 = tim.pw16
# void pw8(uint64_t address, uint64_t val);
tim.pw8.argtypes = [ctypes.c_ulong, ctypes.c_uint]
w8 = tim.pw8

# char *pread(uint64_t address, uint64_t size);

def pread(addr, amount):
    tim.pread.argtypes = [ctypes.c_ulong, ctypes.c_ulong]
    tim.pread.restype = ctypes.c_ubyte * amount
    tmp = tim.pread(addr, amount)
    ret = ''
    for i in tmp:
        ret += chr(i)
    return ret

# void pwrite(uint64_t address, char* data, uint64_t size);
tim.pwrite.argtypes = [ctypes.c_ulong, ctypes.c_char_p, ctypes.c_ulong]
def pwrite(addr, s, l):
    tim.pwrite(addr, s.encode(), l)

# uint64_t PFindCensoredUWorldBase();
tim.PFindCensoredUWorldBase.restype = ctypes.c_ulong
PFindCensoredUWorldBase = tim.PFindCensoredUWorldBase

# uint64_t PSearch(char* query, uint64_t query_size)
tim.PSearch. argtypes = [ctypes.c_char_p, ctypes.c_ulong]
tim.PSearch.restype = ctypes.c_ulong
def PSearch(s, addr):
    try:
        return tim.PSearch(s.encode(), addr)
    except:
        return tim.PSearch(s, addr)

# char* PSearchAndReplace(char* query, uint64_t query_size, char* data, uint64_t datasize)
tim.PSearchAndReplace. argtypes = [ctypes.c_char_p, ctypes.c_ulong, ctypes.c_char_p, ctypes.c_ulong]
tim.PSearchAndReplace.restype = ctypes.c_char_p
def PSearchAndReplace(s, data):
    query_size = len(data)
    datasize = len(data)
    return tim.PSearchAndReplace(s.encode(), query_size, data.encode(), datasize)

tim.Pinit(Censored_pid)
