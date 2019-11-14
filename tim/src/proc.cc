#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include "tim/ctx.h"
#include "tim/proc.h"

namespace Tim {


uint32_t MmProtectToValue[32] = 
{
    PAGE_NOACCESS,
    PAGE_READONLY,
    PAGE_EXECUTE,
    PAGE_EXECUTE_READ,
    PAGE_READWRITE,
    PAGE_WRITECOPY,
    PAGE_EXECUTE_READWRITE,
    PAGE_EXECUTE_WRITECOPY,
    PAGE_NOACCESS,
    PAGE_NOCACHE | PAGE_READONLY,
    PAGE_NOCACHE | PAGE_EXECUTE,
    PAGE_NOCACHE | PAGE_EXECUTE_READ,
    PAGE_NOCACHE | PAGE_READWRITE,
    PAGE_NOCACHE | PAGE_WRITECOPY,
    PAGE_NOCACHE | PAGE_EXECUTE_READWRITE,
    PAGE_NOCACHE | PAGE_EXECUTE_WRITECOPY,
    PAGE_NOACCESS,
    PAGE_GUARD | PAGE_READONLY,
    PAGE_GUARD | PAGE_EXECUTE,
    PAGE_GUARD | PAGE_EXECUTE_READ,
    PAGE_GUARD | PAGE_READWRITE,
    PAGE_GUARD | PAGE_WRITECOPY,
    PAGE_GUARD | PAGE_EXECUTE_READWRITE,
    PAGE_GUARD | PAGE_EXECUTE_WRITECOPY,
    PAGE_NOACCESS,
    PAGE_WRITECOMBINE | PAGE_READONLY,
    PAGE_WRITECOMBINE | PAGE_EXECUTE,
    PAGE_WRITECOMBINE | PAGE_EXECUTE_READ,
    PAGE_WRITECOMBINE | PAGE_READWRITE,
    PAGE_WRITECOMBINE | PAGE_WRITECOPY,
    PAGE_WRITECOMBINE | PAGE_EXECUTE_READWRITE,
    PAGE_WRITECOMBINE | PAGE_EXECUTE_WRITECOPY
};

std::string MmProtectToValueStr[32] = {
	"----", // PAGE_NOACCESS
	"-r--", // PAGE_READONLY
	"---x", // PAGE_EXECUTE
	"-r-x", // PAGE_EXECUTE_READ
	"-rw-", // PAGE_READWRITE
	"-rW-", // PAGE_WRITECOPY // I assume we can read this too
	"-rwx", // PAGE_EXECUTE_READWRITE
	"----", // PAGE_NOACCESS
	"Nr--", // PAGE_NOCACHE | PAGE_READONLY
	"N--x", // PAGE_NOCACHE | PAGE_EXECUTE
	"Nr-x", // PAGE_NOCACHE | PAGE_EXECUTE_READ
	"Nrwx", // PAGE_NOCACHE | PAGE_EXECUTE_READWRITE
	"NrWx", // PAGE_NOCACHE | PAGE_EXECUTE_WRITECOPY
	"----", // PAGE_NOACCESS
	"Gr--", // PAGE_GUARD | PAGE_READONLY
	"G--x", // PAGE_GUARD | PAGE_EXECUTE
	"Gr-x", // PAGE_GUARD | PAGE_EXECUTE_READ
	"Grwx", // PAGE_GUARD | PAGE_EXECUTE_READWRITE
	"GrWx", // PAGE_GUARD | PAGE_EXECUTE_WRITECOPY
	"----", // PAGE_NOACCESS
	"Cr--", // PAGE_WRITECOMBINE | PAGE_READONLY
	"C--x", // PAGE_WRITECOMBINE | PAGE_EXECUTE
	"Cr-x", // PAGE_WRITECOMBINE | PAGE_EXECUTE_READ
	"Crwx", // PAGE_WRITECOMBINE | PAGE_EXECUTE_READWRITE
	"CrWx", // PAGE_WRITECOMBINE | PAGE_EXECUTE_WRITECOPY
};


Proc::Proc(Context& ctx, EPROC_INFO const& proc)
    : _ctx(ctx)
    , _proc(proc) {

}

std::vector<VAD_INFO> Proc::vad() {
    RTL_AVL_TREE tree_head;
    _vad_cache.clear();

    std::vector<VAD_INFO> list;

    if (_proc.vadroot == 0) {
        return list; // no vadroot no vad, job done
    }

    if ( VMI_FAILURE == vmi_read_addr_va(_ctx._vmi, _proc.vadroot, 0, (addr_t*)&tree_head.Root) ) {
        std::cerr << "Failed to read RTL_AVL_TREE (start of vad)" << std::endl;
        return list;
    }

    RTL_BALANCED_NODE node;
    size_t read;
    if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, (addr_t)_proc.vadroot, 0, sizeof(node), (void*)&node, &read) ) {
        std::cerr << "Failed to read RTL_BALANCED_NODE" << std::endl;
        return list;
    }

    // ITS VAD ENTRIES ALL THE WAY DOWN!
    recurse_vad_nodes(VAD_INFO(node, (addr_t)_proc.vadroot), list);

    return list;
}

int Proc::_vad_cache_update() {
    _vad_cache = vad();
}

addr_t Proc::sig_scan(addr_t const addr_start, addr_t const addr_end, uint8_t const* pattern, char const* mask) {
	size_t pos = 0;
	auto maskLength = std::strlen(mask) - 1;

	for (size_t it = 0; addr_start + it <= addr_end; it+=0x1000) {
		uint8_t page[0x1000] = {0};
		size_t read = 0;

		if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, addr_start + it, _proc.pid, 0x1000, (void*)&page, &read) ) {
			continue;
		}

        pos = 0; // all scans *hopefully* fit in a single page

        for (int i = 0; i < 0x1000; i++) {
		    if (page[i] == pattern[pos] || mask[pos] == '?') {
		    	if (mask[pos + 1] == '\0') {
                    std::cout << "FOUND at " << std::hex << addr_start + it + i - maskLength << std::endl;
		    	    int linelen = 0;
                    for (int x = 0; x < 0x1000; x++) {
                        std::cout << std::hex << (int)page[x];
                        if (linelen >= 8)   {
                            linelen = 0;
                            std::cout << std::endl;
                        }
                    }
                    
                    pos = 0;
                    return addr_start + it + i - maskLength;
		    	}

		    	pos++;
		    }
		    else
		    {
		    	pos = 0;
		    }

        }
	}
return -1;
}

// duplicate code for short-term web ui dev
std::vector<addr_t> Proc::sig_scan_multi(addr_t const addr_start, addr_t const addr_end, uint8_t const* pattern, char const* mask) {
    std::vector<addr_t> results;
    size_t pos = 0;
	auto maskLength = std::strlen(mask) - 1;

	for (size_t it = 0; addr_start + it <= addr_end; it+=0x1000) {
		uint8_t page[0x1000] = {0};
		size_t read = 0;

		if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, addr_start + it, _proc.pid, 0x1000, (void*)&page, &read) )
			continue;
		
        pos = 0; // all scans *hopefully* fit in a single page

        for (int i = 0; i < 0x1000; i++) {
		    if (page[i] == pattern[pos] || mask[pos] == '?') 		    {
		    	if (mask[pos + 1] == '\0') 		    	{
                    results.push_back(addr_start + it + i - maskLength);
                    std::cout << "FOUND multi at " << std::hex << addr_start + it + i - maskLength << std::endl;
                    pos = 0;
                    continue;
		    	}
		    	pos++;
		    }
		    else
		    {
		    	pos = 0;
		    }

        }
	}
return results;
}

bool Proc::is_valid_addr(addr_t addr) {
    _vad_cache_update(); // add a timer

    for (auto& v : _vad_cache) { 
        if (addr >= v.range_start && addr <= v.range_stop)
            return true;
    }
    return false;
}

int Proc::process_vad_node(VAD_INFO& node) {
    MMVAD_SHORT vadshort;

    size_t read;

    if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, node.addr, 0, sizeof(MMVAD_SHORT), (void*)&vadshort, &read) ) {
        std::cerr << "Failed to read vad_info MMVAD_SHORT" << std::endl;
        return 1;
    }
    // so we promote some small types to larger types, shift them around and or them
    uint64_t start_vpn = (((uint64_t)vadshort.StartingVpn << PAGE_SHIFT) | ((uint64_t)vadshort.StartingVpnHigh << 44));
    uint64_t end_vpn = (((uint64_t)vadshort.EndingVpn << PAGE_SHIFT) | ((uint64_t)vadshort.EndingVpnHigh << 44));

    end_vpn |= 0xfff;

    node.range_start    = start_vpn;
    node.range_stop     = end_vpn;
    node.commited       = vadshort.u1.VadFlags1.MemCommit;
    node.priv_mem       = vadshort.u.VadFlags.PrivateMemory;    
    node.type           = (VadType)vadshort.u.VadFlags.VadType; // and I thought this was right way to get vad type (short or long)
    node.protection     = MmProtectToValue[vadshort.u.VadFlags.Protection];
    node.protection_str = MmProtectToValueStr[(uint8_t)vadshort.u.VadFlags.Protection];

    // hack learned from volatility - offset -0xc points to kernel pool tag
    // that in turn lets us know what kind of vad we have (short or long)
    char pool_tag[4] = {0};
    vmi_read_va(_ctx._vmi, node.addr - 0xc, 0, 4, (void*)pool_tag, &read);
    auto pool_tag_str = std::string((const char*)pool_tag, read);

    if ( (pool_tag_str == "Vadl" || pool_tag_str == "Vadm" || pool_tag_str == std::string("Vad ")) ) {
        node.longvad = true;

        MMVAD vadlong;
        FILE_OBJECT fileobj;
        if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, node.addr, 0, sizeof(MMVAD), (void*)&vadlong, &read) ) {
            std::cerr << "Failed to read vad_info MMVAD" << std::endl;
            return 1;
        }

/*        std::cout << std::hex << vadlong.FileObject << std::endl;

        // SHIT NOT MAKING SENSE
        //
        //
        if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, (addr_t)vadlong.FileObject, 0, sizeof(FILE_OBJECT), (void*)&fileobj, &read) ) {
            std::cerr << "Failed to read FILE_OBJECTS" << std::endl;
            return 1;
        }
        
        std::cout << "Yo.. the filename pointer is: " << std::hex << fileobj.FileName.Buffer << std::endl;
*/

    }
    else {
        node.longvad = false;
    }
        // "long vad"
        // so we can pull out some more info


//   if (0 == vadshort.u.VadFlags1.PrivateMemory)
//       node.private_mem = false;
//   else
//       node.private_mem = true;
//
    return 0;
}

// this is pulled into its own function, because the RTL_AVL_TREE bit is just at the start of the walk
int Proc::recurse_vad_nodes(VAD_INFO node, std::vector<VAD_INFO>& vadlist) {

    RTL_BALANCED_NODE node_l;
    RTL_BALANCED_NODE node_r;

    size_t read;

    process_vad_node(node);
    vadlist.push_back(node);

    auto nodecopy = node.node; // since we are accessing members for pointers - lets just use a RTL_BALANCED_NODE copy

    if (nodecopy.Left != 0) {
        if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, (addr_t)nodecopy.Left, 0, sizeof(nodecopy), (void*)&node_l, &read) ) {
            std::cerr << "Failed to read RTL_BALANCED_NODE (left)" << std::endl;
            return 1;
        }

        recurse_vad_nodes(VAD_INFO(node_l, (addr_t)nodecopy.Left), vadlist);
    }
    if (nodecopy.Right != 0) {
        if ( VMI_FAILURE == vmi_read_va(_ctx._vmi, (addr_t)nodecopy.Right, 0, sizeof(nodecopy), (void*)&node_r, &read) ) {
            std::cerr << "Failed to read RTL_BALANCED_NODE (right)" << std::endl;
            return 1;
        }
        recurse_vad_nodes(VAD_INFO(node_r, (addr_t)nodecopy.Right), vadlist);
    }
}


} // namespace Tim
