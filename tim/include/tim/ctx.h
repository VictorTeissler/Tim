#ifndef TIM_H
#define TIM_H

#define PAGE_SHIFT 12L
#include <string>
#include <vector>
#include <libvmi/libvmi.h>
#include <cassert>
#include "winstructs.h"

namespace Tim {

#define TIM_ASSERT(...) assert(__VA_ARGS__)

struct EPROC_INFO {

    EPROC_INFO(std::string const& _name_, vmi_pid_t _pid_, addr_t _vadroot_)
        : name(_name_)
        , pid(_pid_)
        , vadroot(_vadroot_) {

    }

    std::string name;
    vmi_pid_t pid;
    addr_t vadroot;
};

class Context {
  public:
    std::vector<EPROC_INFO> procs();
//    template<typename T> T read_va(EPROC_INFO const& proc, addr_t);
//    template<typename T> T write_va(EPROC_INFO cosnt& proc, addr_t);

//    template<typename T> T read_pa(addr_t);
//    template<typename T, std::size_t N> std::array<T, N> read_pa(addr_t);
    void read_va_unchecked(addr_t addr, void* dest, std::size_t len, int pid = 0);
//    template<typename T> T write_pa(addr_t);

    std::wstring read_wstring(vmi_pid_t pid, addr_t addr);

    Context(std::string const& vmname, std::string const& profile_path);
    bool init();

    unsigned long _offset_tasks;
    unsigned long _offset_pid;
    unsigned long _offset_name;
    unsigned long _offset_vad_root; // not abstracted by libvmi, so we pull it from rekall profile

    vmi_instance_t _vmi; // make private
  private:
    std::string vm_name;
    std::string profile_path;

};

}

#endif // ARVID_H
