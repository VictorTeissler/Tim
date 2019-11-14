#ifndef TIM_PROC_H
#define TIM_PROC_H

#include "winstructs.h"

#include <vector>

namespace Tim {

struct EPROC_INFO;
class Context;

enum VadType {
    None,
    DevicePhysicalMemory,
    ImageMap,
    Awe,
    WriteWatch,
    LargePages,
    RotatePhysical,
    LargePageSection
};

struct VAD_INFO {
    VAD_INFO(RTL_BALANCED_NODE _node_, addr_t _addr_)
        : node(_node_)
        , addr(_addr_)
        , longvad(false) {
    }

    friend bool operator == (const VAD_INFO& a, const VAD_INFO& b) {
        return (a.range_start == b.range_start);
    }

    friend bool operator != (const VAD_INFO& a, const VAD_INFO& b) {
        return !(a == b);
    }

    friend bool operator > (const VAD_INFO& a, const VAD_INFO& b) {
        return (a.range_start > b.range_start);
    }

    friend bool operator < (const VAD_INFO& a, const VAD_INFO& b) {
        return !(a > b);
    }

    addr_t addr; // phys memory address where this lived when we copied it.
    RTL_BALANCED_NODE node; // for now just store a copy
  
    bool    longvad;
    bool    commited;
    bool    priv_mem;
    VadType  type;
    uint32_t protection;
	std::string protection_str;
    uint64_t range_start;
    uint64_t range_stop;
};


class Proc {
  public:
    Proc(Context& ctx, EPROC_INFO const& proc);
    std::vector<VAD_INFO> vad();

    addr_t sig_scan(addr_t const addr_start, addr_t const addr_end, uint8_t const* pattern, char const* mask);
    std::vector<addr_t> sig_scan_multi(addr_t const addr_start, addr_t const addr_end, uint8_t const* pattern, char const* mask);

    bool is_valid_addr(addr_t addr);
    
    //    template<typename T> T read(addr_t);
//    template<typename T> T write(addr_t);

    Context& _ctx;
    EPROC_INFO _proc;
  private:

    int process_vad_node(VAD_INFO& node);
    int recurse_vad_nodes(VAD_INFO node, std::vector<VAD_INFO>& vadlist);

    int _vad_cache_update();
    std::vector<VAD_INFO> _vad_cache;
};

}

#endif // TIM_PROC_H
