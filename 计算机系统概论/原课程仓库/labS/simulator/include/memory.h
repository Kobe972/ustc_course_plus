/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:30
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-22 20:02:31
 * @Description  : file content
 */
#include "common.h"

namespace virtual_machine_nsp {
const int kInstructionLength = 16;

inline int16_t TranslateInstruction(std::string &line) {
    // TODO: translate hex mode
    int16_t result = 0;
    if (line.size() == kInstructionLength) {
        for (int index = 0; index < kInstructionLength; ++index) {
            result = (result << 1) | (line[index] & 1);
        }
    }
    return result;
}

const int kVirtualMachineMemorySize = 0xFFFF;

class memory_tp {
    private:
    int16_t memory[kVirtualMachineMemorySize];

    public:
    memory_tp() {
        memset(memory, 0, sizeof(int16_t) * kVirtualMachineMemorySize);
    }
    // Managements
    void ReadMemoryFromFile(std::string filename, int beginning_address=0x3000);
    int16_t GetContent(int address) const;
    int16_t& operator[](int address);
};

}; // virtual machine nsp