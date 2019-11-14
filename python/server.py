#!/usr/bin/env python
from tim import *
from flask import Flask,request
app = Flask(__name__)
from struct import pack
import re


print("Looking for world base")
#world_pointer = PFindCensoredUWorldBase()
world_pointer = 0x7ff6b9269e30
print("Found world pointer 0x%x" % world_pointer)

@app.route("/")
def main():
    addr = 0
    data_request = 'empty data set requested'
    if request.args.get('addr'):
        addr = int(request.args.get('addr'),0x10)
        data_request = ''
        for i in range(0,0x1000, 8):
            setup = '<a href=/?addr=0x%08x>0x%0*x</a>: ' + '<a href=/?addr=0x%08x>0x%0*x</a> '* 8
            base_addr = addr+8*(i+0)
            r0 = r64(addr+8*(i+0))
            r1 = r64(addr+8*(i+1))
            r2 = r64(addr+8*(i+2))
            r3 = r64(addr+8*(i+3))
            r4 = r64(addr+8*(i+4))
            r5 = r64(addr+8*(i+5))
            r6 = r64(addr+8*(i+6))
            r7 = r64(addr+8*(i+7))
            hexdump_raw  = pack("<Q", r0) + pack("<Q", r1) + pack("<Q", r2) + pack("<Q", r3)
            hexdump_raw += pack("<Q", r4) + pack("<Q", r5) + pack("<Q", r6) + pack("<Q", r7)
            hexdump = ''
            for c in hexdump_raw:
                if (c >= 0x20) and (c <= 0x7e):
                    hexdump += chr(c)
                else:
                    hexdump += '.'
            hexdump = str(hexdump)
            hexdump = hexdump.replace('<', '&lt;').replace('>', '&gt;')

            data_request += setup % (base_addr,16,base_addr,
                                     r0,16,r0,
                                     r1,16,r1,
                                     r2,16,r2,
                                     r3,16,r3,
                                     r4,16,r4,
                                     r5,16,r5,
                                     r6,16,r6,
                                     r7,16,r7)
            data_request += '| %s<br>' % hexdump
    ret = '<html><body style="font-family:monospace;">'
    ret += 'world_pointer = <a href=/?addr=0x%x>0x%x</a> ' % (world_pointer,world_pointer)
    ret += 'ptr_requested = <a href=/?addr=0x%x>0x%x</a><br><br>' % (addr,addr)
    ret += '<table border="1"><center>'
    ret += '<tr><td><form action="/utsearch" method="get"><input type="text" name="addr" value="search unicode text"><input type="submit"></form></td>'
    ret += '<td><form action="/tsearch" method="get"><input type="text" name="addr" value="search text"><input type="submit"></form></td></tr>'
    ret += '<tr><td><form action="/search" method="get"><input type="text" name="addr" value="search for pointer reference"><input type="submit"></form></td>'
    ret += '<td><form action="/uwrite_data" method="get"><input type="text" name="addr" value="addr_to_write"><input type="text" name="data" value="data"><input type="submit"></form></td></tr>'
    ret += '<tr><td><form action="/search_and_replace" method="get"><input type="text" name="src" value="target_text"><input type="text" name="dst" value="dest_text"><input type="submit"></form></td></tr>'
    ret += '</center></table>'
    ret += '<hr>'
    ret += '<pre>%s</pre>' % data_request
    ret += ''
    ret += '</body></html>'
    return ret


@app.route("/search")
def search():
    search_str = int(request.args.get('addr'),0x10)
    search_bytes =  pack("<Q", search_str)
    search_bytes_len = len(search_bytes)
    ret = '<html><body><pre>0x%0*x</pre></body></html>' % (16, PSearch(search_bytes, search_bytes_len))
    return ret


@app.route("/utsearch")
def utsearch():
    search_str = request.args.get('addr')
    search_bytes = ''
    for i in search_str:
        search_bytes += i + '\0'
    search_bytes_len = len(search_bytes)
    addr = PSearch(search_bytes, search_bytes_len)
    ret = '<html><body><pre>0x%x</pre></body></html>' % addr
    return ret


@app.route("/tsearch")
def tsearch():
    search_str = request.args.get('addr')
    search_bytes = ''
    for i in search_str:
        search_bytes += i
    search_bytes_len = len(search_bytes)
    addr = PSearch(search_bytes, search_bytes_len)
    ret = '<html><body><pre>0x%x</pre></body></html>' % addr
    return ret

@app.route("/uwrite_data")
def write_data():
    addr = int(request.args.get('addr'), 0x10)
    data = request.args.get('data')
    data2 = ''
    for i in data:
        data2 += i + '\0'
    print(data2)
    pwrite(addr, data2, len(data2))
    return '<html><body><pre>Data written</pre></body></html>'

@app.route("/search_and_replace")
def search_and_replace():
    src = request.args.get('src')
    src2 = ''
    for i in src:
        src2 += i + '\0'
    dst = request.args.get('dst')
    dst2 = ''
    for i in dst:
        dst2 += i + '\0'
    print("[%s] -> [%s]\n", src2, dst2)
    return str(PSearchAndReplace(src2,dst2).decode()).replace('\n', '<br>')
