/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:26:26
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-20 18:00:34
 * @Description  : file content
 */

#include "common.h"

namespace virtual_machine_nsp {
    const int kRegisterNumber = 10;
    enum RegisterName {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC, // 8
        R_COND // 9
    };
    
    typedef std::array<int16_t, kRegisterNumber> register_tp;
    std::ostream& operator<<(std::ostream& os, const register_tp& reg);
} // virtual machine namespace