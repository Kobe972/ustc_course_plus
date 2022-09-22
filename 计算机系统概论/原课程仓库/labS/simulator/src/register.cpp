/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:26:21
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-20 18:00:46
 * @Description  : file content
 */

#include "register.h"

namespace virtual_machine_nsp {
    std::ostream& operator<<(std::ostream& os, const register_tp& reg) {
        os << "\e[1mR0\e[0m = " << std::hex << reg[R_R0] << ", ";
        os << "\e[1mR1\e[0m = " << std::hex << reg[R_R1] << ", ";
        os << "\e[1mR2\e[0m = " << std::hex << reg[R_R2] << ", ";
        os << "\e[1mR3\e[0m = " << std::hex << reg[R_R3] << std::endl;
        os << "\e[1mR4\e[0m = " << std::hex << reg[R_R4] << ", ";
        os << "\e[1mR5\e[0m = " << std::hex << reg[R_R5] << ", ";
        os << "\e[1mR6\e[0m = " << std::hex << reg[R_R6] << ", ";
        os << "\e[1mR7\e[0m = " << std::hex << reg[R_R7] << std::endl;
        os << "\e[1mCOND[NZP]\e[0m = " << std::bitset<3>(reg[R_COND]) << std::endl;
        os << "\e[1mPC\e[0m = " << std::hex << reg[R_PC] << std::endl;
        return os;
    }
} // virtual machine namespace