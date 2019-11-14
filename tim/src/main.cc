#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>


#include "tim/ctx.h"
#include "tim/proc.h"

void main_loop(Tim::Proc& proc) {
    auto vadinfo = proc.vad();
    std::cout << "Printing some vad contents..." << std::endl;
    std::sort(vadinfo.begin(), vadinfo.end());

    for (auto const& v : vadinfo) {
        std::string message;

        auto comittype = v.commited ? "c" : "r";
        auto vadtype = v.longvad ? "L" : "S";


//        auto results = proc.sig_scan(v.range_start, v.range_stop, (uint8_t*)"\xff\xff\xff\xff\xff\xFF\xff\xff\xff\xff\xff", "xxxxxxxxxxx");
//        auto results = proc.sig_scan(v.range_start, v.range_stop, (uint8_t*)"censored", "censored");
//
//        scan for sig, read uint at unknown bytes, add the address you found the sig at and add 0x7
        auto results = proc.sig_scan(v.range_start, v.range_stop, (uint8_t*)"censored", "xxx????xxxxxx");
        if (results == -1)
            continue;

        uint64_t unknown = results + 3;

        uint32_t blah = 0;
        size_t read = 0;

        vmi_read_va(proc._ctx._vmi, unknown, proc._proc.pid, 4, &blah, &read);

        std::cout << "blah: " << std::hex << blah << std::endl;

        addr_t uworld_base = results + blah + 0x7;
        std::cout << "uworld base: " << std::hex << uworld_base << std::endl;

       
        addr_t cuworld, view_loc_last_frame, view_loc;

        uint8_t data[0x100];

        vmi_read_va(proc._ctx._vmi, uworld_base, proc._proc.pid, 8, (void*)&cuworld, &read);
        
        for (;;) {
            // we don't know the right offset for loc_last_Frame - so we look around
            // 0x00d0 - is an offset taken from online, so we search +- 0x20 around it
            for (size_t x = 0; x < 0x40; x+=8) {
                if (VMI_FAILURE == vmi_read_va(proc._ctx._vmi, uworld_base + (0x00D0 - 0x20) + x, proc._proc.pid, 8, (void*)&view_loc_last_frame, &read))
                    continue;
                if (VMI_FAILURE == vmi_read_va(proc._ctx._vmi, view_loc_last_frame, proc._proc.pid, 0x10, (void*)&data, &read))
                    continue;
                vmi_print_hex(data, read);
                std::cout << std::endl;
             }
          }

            std::cout << "\033[H\033[J" << std::endl;
        }
       
//		if (protec & Tim::PAGE_GUARD)
//			perm << "G";
//		else
//			perm << "-";
//		if (protec & Tim::PAGE_NOACCESS)
//			perm << "---";
//		else if (protec & Tim::PAGE_READ
//		if (v.protection & 
//                perm = "---x"; break;
//                perm = "-r-x"; break;
//                perm = "-rwx"; break;
//                perm = "crwx"; break;
//                perm = "----"; break;
//                perm = "-r--"; break;
//                perm = "-rw-"; break;
//                perm = "crw-"; break;
//        }


        //std::cout << comittype << v.type << " " << vadtype << " " << v.protection_str << " [" << std::hex << v.range_start << " - " << v.range_stop << "] " << std::endl;
}


int main (int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s vmname conf_path pid\n", argv[1]);
    }

    auto vmname    = std::string(argv[1]);
    auto conf_path = std::string(argv[2]);
    auto pid       = std::stoi(argv[3]);

    Tim::Context ctx(vmname, conf_path);
    ctx.init();

    auto proclist = ctx.procs();

    for (auto& x : proclist) {

        if (x.pid == pid) {
            Tim::Proc proc(ctx, x);
            main_loop(proc);
            return 0;
        }
    }

    std::cerr << "Pid not found" << std::endl;

    return 0;
}


#include "python_bindings.c"
