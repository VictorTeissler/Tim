#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <libvmi/libvmi.h>

#include "tim/ctx.h"
#include "tim/rekall_parser.h"

// This code stands on the shoulders of giants
// xen
// qemu
// libvmi
// rekall
// volatility
// rekall json parser from vmi-unpack
// windbg
// interface inspiration from hadesmem (joshua boyce)
namespace Tim {

Context::Context(std::string const& vmname, std::string const& profile_path)
    : vm_name(vmname)
    , profile_path(profile_path) {
    _offset_tasks = _offset_pid = _offset_name = 0;
}

bool Context::init() {
    auto rv = vmi_init_complete(&_vmi, (void*)vm_name.c_str(), VMI_INIT_DOMAINNAME, NULL, VMI_CONFIG_GLOBAL_FILE_ENTRY, NULL, NULL);
    if (VMI_FAILURE == rv) {
        std::cout << "failed to init vmi on: " << vm_name.c_str() << std::endl;
        return false;
    }

    if ( VMI_FAILURE == vmi_get_offset(_vmi, "win_tasks", &_offset_tasks) ) {
        std::cerr << "failed to get win_tasks offset\n";
        return 0;
    }

    if ( VMI_FAILURE == vmi_get_offset(_vmi, "win_pname", &_offset_name) ) {
        std::cerr << "failed to get win_pname offset\n";
        return 0;
    }

    if ( VMI_FAILURE == vmi_get_offset(_vmi, "win_pid", &_offset_pid) ) {
        std::cerr << "failed to get win_pid offset\n";
        return 0;
    }

    // borrowed rekall json parsing code from a project called vmi_unpack
    // but I didn't want to refactor it, so I slightly patched it, and now we pull stuff out for this ctx
    windows_rekall_t win_offsets;

    if ( false == parse_rekall_windows(&win_offsets, (char*)profile_path.c_str()) ) { // casting away const WHAT COULD GO WRONG!?
        std::cerr << "failed to parse rekall config file\n";
        return 0;
    }
    _offset_vad_root = win_offsets.eprocess_vadroot;
}

// This sort of stuff belongs in a template function - trait idiom
// TODO: MAX SIZE IS 500 - WHAT ARE YOU TRYING TO READ? A NOVEL!?
std::wstring Context::read_wstring(vmi_pid_t pid, addr_t addr) {

    uint16_t buff[500] = {0};
    size_t read = 0;
    if ( VMI_FAILURE == vmi_read_va(_vmi, addr, pid, 500, buff, &read) ) {
        return std::wstring(L"");
    }

    std::wstringstream wstream;

    int i = 0;
    for (; i < 500; i++) {
        if (buff[i] == 0x0000)
            break;
        wstream << (wchar_t)buff[i];
    }
    return wstream.str();
}

std::vector<EPROC_INFO> Context::procs() {

    addr_t proc_current = 0;
    vmi_pid_t pid = 0;
    addr_t list_head;
    addr_t list_current;
    addr_t list_next;
    char* procname = NULL;
    addr_t vadroot;

    std::vector<EPROC_INFO> proclist;

    if (VMI_FAILURE == vmi_read_addr_ksym(_vmi, (char*)"PsActiveProcessHead", &list_head)) {
        std::cerr << "Failed to find PsActiveProcessHead\n";
        return proclist;
    }

    list_current = list_head;

    if (VMI_FAILURE == vmi_read_addr_va(_vmi, list_current, 0, &list_next)) {
        std::cerr << "Failed to read initial next proc pointer\n";
        return proclist;
    }

    // List of EPROCESS entries and pull out relevant info - then put that in a EPROC_INFO vector and return
    do {
        proc_current = list_current - _offset_tasks; // list_entry is at bot, so we subtract to get top

        vmi_read_32_va(_vmi, proc_current + _offset_pid, 0, (uint32_t*)&pid);
        auto procname_c = vmi_read_str_va(_vmi, proc_current + _offset_name, 0);

        if ( VMI_FAILURE == vmi_read_64_va(_vmi, proc_current + _offset_vad_root, 0, &vadroot) ) {
            std::cerr << "Failed to read vad root out of eprocess\n";
            return proclist;
        }

        std::string procname(procname_c);
        free(procname_c); // cringy c memory management

        list_current = list_next;
        if (VMI_FAILURE == vmi_read_addr_va(_vmi, list_current, 0, &list_next)) {
            std::cerr << "Failed to read next proc pointer in loop\n";
            return proclist;
        }

        proclist.push_back(EPROC_INFO(procname, pid, vadroot)); // I wonder if this compiles down to 'move-semantics'
    } while(list_current != list_head);

    return proclist;
}


void Context::read_va_unchecked(addr_t addr, void* dest, std::size_t len, int pid) {
    if ( 0 == len ) return;

    std::size_t read;
    if (VMI_FAILURE == vmi_read_va(_vmi, addr, pid, len, dest, &read)) {

    }
}



///tmp/dump/7fe1c8ce0000: SEASON 7
///tmp/dump/7fe1c8ce0000: SEASON 7
///tmp/dump/7fe1c8ce0000: SEASON 7
///tmp/dump/7fe248ce0000: SEASON 7


//void dump_vad(Context const& arvid, EPROC_INFO const& proc, std::vector<VAD_INFO> const& vad) {
//
//    for (auto const& v : vad) {
//        std::stringstream stream;
//        stream << "/tmp/dump/" << std::hex << v.range_start;
//
//
//        std::cout << "dumping at: " << std::hex << v.range_start << std::endl;
//
//        std::fstream file;
//        std::string path(stream.str());
//
//        std::cout << path << std::endl;
//
//        size_t read=0;
//
//
//        file.open(path, std::fstream::out | std::fstream::binary);
//
//        for (addr_t i = v.range_start, read=0; i < v.range_stop; i+=0x1000) {
//
//            char buffer[0x1000] = {0};
//
//
//            if (VMI_FAILURE == vmi_read_va(arvid.vmi, (addr_t)i, proc.pid, 0x1000, (void*)buffer, &read) ) {
////                read = 1000; // pretend its k
//               // std::cerr << "failed to read mem" << std::endl;
//                //std::cerr << "so i read: " << std::dec << read << std::endl;
//                memset(buffer, 0, 0x1000);
//                file.write(buffer, 0x1000);
//                continue;
//            }
//            else {
//
//                file.write((const char*)buffer, read);
//                // std::cerr << "good read: " << std::dec << read << std::endl;
//            }
//
//  //          if (read == 0)
//    //            i += 500;
//
//        }
//
//        file.close();
//
//    }
//}

//addr_t find_unicode(Context const& arvid, EPROC_INFO const& proc, std::vector<VAD_INFO> const& vad, char const* str) {
//
//    uint16_t result[strlen(str) + 2] = {0}; // 2 for null terminator
//
//    for (int i = 0; i < strlen(str); i++) {
//       result[i] = str[i];
//    }
//
//
//    size_t read;
//    for (auto const& v : vad) {
//        for (addr_t a = v.range_start; a < v.range_stop; a+=0x1000) {
//            uint16_t buffer[0x1000];
//            memset(buffer, 0, sizeof(uint16_t) * 0x1000);
//
//            if (VMI_FAILURE == vmi_read_va(arvid.vmi, (addr_t)a, proc.pid, 0x1000, (void*)buffer, &read) ) {
//                continue;
//            }
//
//           auto b = ((uint16_t*)buffer);
//            for (size_t i = 0, size_t x = 0; i < 0x1000; i++) {
//                if (buffer[i] == result[x])
//                    if (x >=
//                    x++;
//                else {
//                    i -= x;
//                    x = 0;
//                }
//            }
//    }
//    return 0;
//}




///////////////// SOEN ////////////////////////
//#define RETSIZE 0x1000
//
//extern "C" {
//    char* b_search();
//    char* b_dump_vad(uint64_t pid, uint64_t addr);
//}
//
//char* b_dump_vad(uint64_t pid, uint64_t addr) {
//    char *ret = (char*)malloc(RETSIZE);
//    memset(ret, 0, RETSIZE);
//    auto a1 = "win10";
//    auto a2 = "ntkrnlmp.json";
//    Context tim(a1, a2);
//    tim.init();
//    std::vector<EPROC_INFO> proclist;
//    tim.procs(proclist);
//    for (auto& x : proclist) {
//        sprintf(&ret[(unsigned int)strlen(ret)], "Process: 0x%x pid: %x\n");
//        std::vector<VAD_INFO> vadinfo;
//        tim.vad(x, vadinfo);
//        if (x.pid == pid) {
//       //     dump_vad(arvid, x, vadinfo);
//        }
//    }
//    return ret;
//}
//
//#define RETSIZE 0x1000
//
//char* b_search() {
//    char *ret = (char*)malloc(RETSIZE);
//    memset(ret, 0, RETSIZE);
//    auto a1 = "win10";
//    auto a2 = "ntkrnlmp.json";
//    Context tim(a1, a2);
//    tim.init();
//    std::vector<EPROC_INFO> proclist;
//    tim.procs(proclist);
//    for (auto& x : proclist) {
//        sprintf(&ret[strlen(ret)], "Process: 0x%x pid: %x\n");
//        std::vector<VAD_INFO> vadinfo;
//        //walk_vad(arvid, x, vadinfo);
//    }
//    return ret;
//}

///////////////// SOEN ////////////////////////
//
}
