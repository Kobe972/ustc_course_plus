/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:24
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 16:08:51
 * @Description  : file content
 */
#include "common.h"
#include "memory.h"

namespace virtual_machine_nsp {
    void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address) {
        // Read from the file
        // TO BE DONE
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        // TO BE DONE
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        // TO BE DONE
    }    
}; // virtual machine namespace
