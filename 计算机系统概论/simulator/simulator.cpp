/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 19:41:16
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-24 05:33:38
 * @Description  : file content
 */
#include "simulator.h"

namespace virtual_machine_nsp {
    template <typename T, unsigned B>
    inline T SignExtend(const T x) {
        // Extend the number
        if (((1 << (B - 1)) & x) == 0) return x;
        T ret = x;
        for (int i = B; i <= 15; i++) ret |= (1 << i);
        return ret;
    }

    void virtual_machine_tp::UpdateCondRegister(int regname) {
        // Update the condition register
        if (reg[regname] < 0) reg[R_COND] = 4;
        else if (reg[regname] == 0) reg[R_COND] = 2;
        else reg[R_COND] = 1;
    }

    void virtual_machine_tp::VM_ADD(int16_t inst) {
        int flag = inst & 0b100000;
        int dr = (inst >> 9) & 0x7;
        int sr1 = (inst >> 6) & 0x7;
        if (flag) {
            // add inst number
            int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
            reg[dr] = reg[sr1] + imm;
        }
        else {
            // add register
            int sr2 = inst & 0x7;
            reg[dr] = reg[sr1] + reg[sr2];
        }
        // Update condition register
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_AND(int16_t inst) {
        int flag = inst & 0b100000;
        int dr = (inst >> 9) & 0x7;
        int sr1 = (inst >> 6) & 0x7;
        if (flag) {
            // add inst number
            int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
            reg[dr] = reg[sr1] & imm;
        }
        else {
            // add register
            int sr2 = inst & 0x7;
            reg[dr] = reg[sr1] & reg[sr2];
        }
        // Update condition register
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_BR(int16_t inst) {
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        int16_t cond_flag = (inst >> 9) & 0x7;
        if (gIsDetailedMode) {
            std::cout << reg[R_PC] << std::endl;
            std::cout << pc_offset << std::endl;
        }
        if (cond_flag & reg[R_COND]) {
            reg[R_PC] += pc_offset;
        }
    }

    void virtual_machine_tp::VM_JMP(int16_t inst) {
        int base = (inst >> 6) & 0x7;
        reg[R_PC] = reg[base];
    }

    void virtual_machine_tp::VM_JSR(int16_t inst) {
        int mode = inst >> 11 & 0x1;
        if (mode == 1) //JSR
        {
            int16_t pc_offset = SignExtend<int16_t, 11>(inst & 0x7FF);
            reg[R_R7] = reg[R_PC];
            reg[R_PC] += pc_offset;
        }
        else
        {
            int base = (inst >> 6) & 0x7;
            reg[R_R7] = reg[R_PC];
            reg[R_PC] = reg[base];
        }
    }

    void virtual_machine_tp::VM_LD(int16_t inst) {
        int16_t dr = (inst >> 9) & 0x7;
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        reg[dr] = mem[reg[R_PC] + pc_offset];
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_LDI(int16_t inst) {
        int16_t dr = (inst >> 9) & 0x7;
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        reg[dr] = mem[mem[reg[R_PC] + pc_offset]];
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_LDR(int16_t inst) {
        int16_t dr = (inst >> 9) & 0x7;
        int base = (inst >> 6) & 0x7;
        int16_t offset = SignExtend<int16_t, 6>(inst & 0x3F);
        reg[dr] = mem[reg[base] + offset];
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_LEA(int16_t inst) {
        int16_t dr = (inst >> 9) & 0x7;
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        reg[dr] = reg[R_PC] + pc_offset;
    }

    void virtual_machine_tp::VM_NOT(int16_t inst) {
        int dr = (inst >> 9) & 0x7;
        int sr = (inst >> 6) & 0x7;
        // add register
        int sr2 = inst & 0x7;
        reg[dr] = ~reg[sr];
        // Update condition register
        UpdateCondRegister(dr);
    }

    void virtual_machine_tp::VM_RTI(int16_t inst) {
        reg[R_PC] = reg[R_R7];
    }

    void virtual_machine_tp::VM_ST(int16_t inst) {
        int16_t sr = (inst >> 9) & 0x7;
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        mem[reg[R_PC] + pc_offset] = reg[sr];
    }

    void virtual_machine_tp::VM_STI(int16_t inst) {
        int16_t sr = (inst >> 9) & 0x7;
        int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
        mem[mem[reg[R_PC] + pc_offset]] = reg[sr];
    }

    void virtual_machine_tp::VM_STR(int16_t inst) {
        int16_t sr = (inst >> 9) & 0x7;
        int base = (inst >> 6) & 0x7;
        int16_t offset = SignExtend<int16_t, 6>(inst & 0x3F);
        mem[reg[base] + offset] = reg[sr];
    }

    void virtual_machine_tp::VM_TRAP(int16_t inst) {
        ;//pass
        //int trapnum = inst & 0xFF;
        //if (trapnum == 0x25)
        //    exit(0);
        // TODO: build trap program
    }

    virtual_machine_tp::virtual_machine_tp(const int16_t address, const std::string& memfile, const std::string& regfile) {
        // Read memory
        if (memfile != "") {
            mem.ReadMemoryFromFile(memfile,address);
        }
        // Read registers
        std::ifstream input_file;
        input_file.open(regfile);
        if (input_file.is_open()) {
            int line_count = std::count(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>(), '\n');
            input_file.close();
            input_file.open(regfile);
            if (line_count >= 8) {
                for (int index = R_R0; index <= R_R7; ++index) {
                    input_file >> reg[index];
                }
            }
            else {
                for (int index = R_R0; index <= R_R7; ++index) {
                    reg[index] = 0;
                }
            }
            input_file.close();
        }
        else {
            for (int index = R_R0; index <= R_R7; ++index) {
                reg[index] = 0;
            }
        }

        // Set address
        reg[R_PC] = address;
        reg[R_COND] = 0;
    }

    void virtual_machine_tp::SetReg(const register_tp& new_reg) {
        reg = new_reg;
    }

    int16_t virtual_machine_tp::NextStep() {
        int16_t current_pc = reg[R_PC];
        reg[R_PC]++;
        int16_t current_instruct = mem[current_pc];
        int opcode = (current_instruct >> 12) & 15;

        switch (opcode) {
        case O_ADD:
            if (gIsDetailedMode) {
                std::cout << "ADD" << std::endl;
            }
            VM_ADD(current_instruct);
            break;
        case O_AND:
            if (gIsDetailedMode) {
                std::cout << "AND" << std::endl;
            }
            VM_AND(current_instruct);
            break;
        case O_BR:
            if (gIsDetailedMode) {
                std::cout << "BR" << std::endl;
            }
            VM_BR(current_instruct);
            break;
        case O_JMP:
            if (gIsDetailedMode) {
                std::cout << "JMP" << std::endl;
            }
            VM_JMP(current_instruct);
            break;
        case O_JSR:
            if (gIsDetailedMode) {
                std::cout << "JSR" << std::endl;
            }
            VM_JSR(current_instruct);
            break;
        case O_LD:
            if (gIsDetailedMode) {
                std::cout << "LD" << std::endl;
            }
            VM_LD(current_instruct);
            break;
        case O_LDI:
            if (gIsDetailedMode) {
                std::cout << "LDI" << std::endl;
            }
            VM_LDI(current_instruct);
            break;
        case O_LDR:
            if (gIsDetailedMode) {
                std::cout << "LDR" << std::endl;
            }
            VM_LDR(current_instruct);
            break;
        case O_LEA:
            if (gIsDetailedMode) {
                std::cout << "LEA" << std::endl;
            }
            VM_LEA(current_instruct);
            break;
        case O_NOT:
            if (gIsDetailedMode) {
                std::cout << "NOT" << std::endl;
            }
            VM_NOT(current_instruct);
            break;
        case O_RTI:
            if (gIsDetailedMode) {
                std::cout << "RTI" << std::endl;
            }
            VM_RTI(current_instruct);
            break;
        case O_ST:
            if (gIsDetailedMode) {
                std::cout << "ST" << std::endl;
            }
            VM_ST(current_instruct);
            break;
        case O_STI:
            if (gIsDetailedMode) {
                std::cout << "STI" << std::endl;
            }
            VM_STI(current_instruct);
            break;
        case O_STR:
            if (gIsDetailedMode) {
                std::cout << "STR" << std::endl;
            }
            VM_STR(current_instruct);
            break;
        case O_TRAP:
            if (gIsDetailedMode) {
                std::cout << "TRAP" << std::endl;
            }
            if ((current_instruct & 0xFF) == 0x25) {
                reg[R_PC] = 0;
            }
            VM_TRAP(current_instruct);
            break;
        default:
            VM_RTI(current_instruct);
            break;
        }

        if (current_instruct == 0) {
            // END
            // TODO: add more detailed judge information
            return 0;
        }
        return reg[R_PC];
    }

} // namespace virtual_machine_nsp