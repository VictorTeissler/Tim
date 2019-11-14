#include <Python.h>

namespace Tim {
    extern "C" {
        int Pinit (uint64_t pid);
        int Pterminate ();
        uint64_t pr64(uint64_t address);
        uint64_t pr32(uint64_t address);
        uint64_t pr16(uint64_t address);
        uint64_t pr8(uint64_t address);
        void pw64(uint64_t address, uint64_t val);
        void pw32(uint64_t address, uint64_t val);
        void pw16(uint64_t address, uint64_t val);
        void pw8(uint64_t address, uint64_t val);
        char *pread(uint64_t address, uint64_t size);
        void pwrite(uint64_t address, char* data, uint64_t size);
        uint64_t PSearch(char* query, uint64_t query_size);
        char* PSearchAndReplace(char* query, uint64_t query_size, char* data, uint64_t datasize);

        uint64_t PFindCensoredUWorldBase();

        auto counter = -1;
        int TargetPid = 0;
        vmi_instance* GlobalVMI;
        Context* gctx = new Context("win10", "/home/pancho/code/personal/tim2/profiles/ntkrnlmp.json");
        Proc* proc;
        std::vector<EPROC_INFO> GlobalProcList;
        uint64_t data = 0;
        uint64_t uworld_base;
    }

    int Pterminate () {
    }
    
    int Pinit (uint64_t pid) {
        TargetPid = pid;

        
        gctx->init();
        GlobalProcList = gctx->procs();
        for (auto& x : GlobalProcList) {

            if (x.pid == TargetPid) {
                proc = new Proc(*gctx, x);
                GlobalVMI = proc->_ctx._vmi;
                return 0;
            }
        }
        return 1;
    }

    uint64_t PFindCensoredUWorldBase() {
        auto vadinfo = proc->vad();
        std::sort(vadinfo.begin(), vadinfo.end());

        for (auto const& v : vadinfo) {
            std::string message;

            auto comittype = v.commited ? "c" : "r";
            auto vadtype = v.longvad ? "L" : "S";
            auto results = proc->sig_scan(v.range_start, v.range_stop, (uint8_t*)"\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41", "xxx????xxxxxx");
            if (results == -1)
                continue;
            uint64_t offset = pr32(results+3);
            uworld_base = results + offset + 0x7;
            return uworld_base;
        }
    }



    uint64_t pr64(uint64_t address) {
        data = 0;
        uint64_t bytes_read;
        vmi_read_va(GlobalVMI, address, TargetPid, 8, &data, &bytes_read);

        return data;
    }

    uint64_t pr32(uint64_t address) {
        data = 0;
        uint64_t bytes_read;
        vmi_read_va(GlobalVMI, address, TargetPid, 8, &data, &bytes_read);
        data &= 0xffffffff;
        return data;
    }

    uint64_t pr16(uint64_t address) {
        data = 0;
        uint64_t bytes_read;
        vmi_read_va(GlobalVMI, address, TargetPid, 8, &data, &bytes_read);
        data &= 0xffff;
        return data;
    }

    uint64_t pr8(uint64_t address) {
        data = 0;
        uint64_t bytes_read;
        vmi_read_va(GlobalVMI, address, TargetPid, 8, &data, &bytes_read);
        data &= 0xff;
        return data;
    }

    void pw64(uint64_t address, uint64_t val) {
        uint64_t bytes_written;
        vmi_write_va(GlobalVMI, address, TargetPid, 8, &val, &bytes_written);
    }

    void pw32(uint64_t address, uint64_t val) {
        uint64_t bytes_written;
        vmi_write_va(GlobalVMI, address, TargetPid, 4, &val, &bytes_written);
    }

    void pw16(uint64_t address, uint64_t val) {
        uint64_t bytes_written;
        vmi_write_va(GlobalVMI, address, TargetPid, 2, &val, &bytes_written);
    }

    void pw8(uint64_t address, uint64_t val) {
        uint64_t bytes_written;
        vmi_write_va(GlobalVMI, address, TargetPid, 1, &val, &bytes_written);
    }

    char *pread(uint64_t address, uint64_t size) {
        char* ret = (char*)malloc(size);
        uint64_t bytes_read;
        vmi_read_va(GlobalVMI, address, TargetPid, size, ret, &bytes_read);
        return ret;
    }

    void pwrite(uint64_t address, char* data, uint64_t size) {
        printf("python_bindings->pwrite(uint64_t address=0x%lx, char* data=[%s], uint64_t size=0x%lx)\n",address,data,size);
        uint64_t bytes_written;
        //size_t vmi_write_va (vmi_instance_t vmi, addr_t vaddr, vmi_pid_t pid, void *buf, size_t count)
        vmi_write_va(GlobalVMI, address, TargetPid, size, data, &bytes_written);
    }

    char* pwrite2(uint64_t address, char* data, uint64_t size) {
        char* r = (char*)malloc(1024);
        sprintf(r, "python_bindings->pwrite(uint64_t address=0x%lx, char* data=[%s], uint64_t size=0x%lx)\n",address,data,size);
        uint64_t bytes_written;
        //size_t vmi_write_va (vmi_instance_t vmi, addr_t vaddr, vmi_pid_t pid, void *buf, size_t count)
        vmi_write_va(GlobalVMI, address, TargetPid, size, data, &bytes_written);
        return r;
    }

    uint64_t PSearch(char* query, uint64_t query_size) {
        auto vadinfo = proc->vad();
        std::sort(vadinfo.begin(), vadinfo.end());

        for (auto const& v : vadinfo) {
            std::string message;

            auto comittype = v.commited ? "c" : "r";
            auto vadtype = v.longvad ? "L" : "S";
            char * mask = (char *)malloc(query_size);
            memset(mask, 'x', query_size);
            auto results = proc->sig_scan(v.range_start, v.range_stop, (uint8_t*)query, mask);
            if (results == -1)
                continue;
            return results;
        }
    }

    char* PSearchAndReplace(char* query, uint64_t query_size, char* data, uint64_t datasize) {
        uint64_t rsize = 0x1000;
        char* r = (char*)malloc(rsize);
        sprintf(r, "PSearchAndReplace(char* query=[%s], uint64_t query_size=0x%lx, char* data=[%s], uint64_t datasize=0x%lx)\n",
                query, query_size, data, datasize);
        char msg[0x400];
        memset(r, 0, rsize);
        auto vadinfo = proc->vad();
        std::sort(vadinfo.begin(), vadinfo.end());

        for (auto const& v : vadinfo) {
            std::string message;

            auto comittype = v.commited ? "c" : "r";
            auto vadtype = v.longvad ? "L" : "S";
            char * mask = (char *)malloc(query_size);
            memset(mask, 'x', query_size);
            auto results = proc->sig_scan_multi(v.range_start, v.range_stop, (uint8_t*)query, mask);
            
            for (auto addr : results) {
                sprintf(msg, "Write and replace on addr 0x%lx\n", addr);
                if (strlen(r) + strlen(msg)>rsize){
                    rsize*=2;
                    r = (char*)realloc(r, rsize);
                }
                strcat(r, msg);
                char* p2 = pwrite2(addr, data, datasize);
                if (strlen(r) + strlen(p2)>rsize){
                    rsize*=2;
                    r = (char*)realloc(r, rsize);
                }
                strcat(r, p2);
            }
        }
        printf("%s\n", r);
        return r;
    }
}













