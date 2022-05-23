/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 09:27:50
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-22 19:36:46
 * @Description  : file content
 */
#pragma once

#include "common.h"
#include "register.h"
#include "memory.h"

namespace virtual_machine_nsp {

enum kOpcodeList {
    O_ADD = 0b0001,
    O_AND = 0b0101,
    O_BR  = 0b0000,
    O_JMP = 0b1100,
    O_JSR = 0b0100,
    O_LD  = 0b0010,
    O_LDI = 0b1010,
    O_LDR = 0b0110,
    O_LEA = 0b1110,
    O_NOT = 0b1001,
    O_RTI = 0b1000,
    O_ST  = 0b0011,
    O_STI = 0b1011,
    O_STR = 0b0111,
    O_TRAP = 0b1111
};

enum kTrapRoutineList {
};

class virtual_machine_tp {
    public:
    register_tp reg;
    memory_tp mem;
    
    // Instructions
    void VM_ADD(int16_t inst);
    void VM_AND(int16_t inst);
    void VM_BR(int16_t inst);
    void VM_JMP(int16_t inst);
    void VM_JSR(int16_t inst);
    void VM_LD(int16_t inst);
    void VM_LDI(int16_t inst);
    void VM_LDR(int16_t inst);
    void VM_LEA(int16_t inst);
    void VM_NOT(int16_t inst);
    void VM_RTI(int16_t inst);
    void VM_ST(int16_t inst);
    void VM_STI(int16_t inst);
    void VM_STR(int16_t inst);
    void VM_TRAP(int16_t inst);

    // Managements
    virtual_machine_tp() {}
    virtual_machine_tp(const int16_t address, const std::string &memfile, const std::string &regfile);
    void UpdateCondRegister(int reg);
    void SetReg(const register_tp &new_reg);
    int16_t NextStep();
};

}; // virtual machine namespace
