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
        char line[17];
        int lines = beginning_address;
        FILE* infile;
        infile = fopen(filename.c_str(), "r");
        while (fscanf(infile, "%s", line) != EOF)
        {
            std::string tmp(line);
            memory[lines] = TranslateInstruction(tmp);
            lines++;
        }
        fclose(infile);
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        return memory[address];
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        return memory[address];
    }
}; // virtual machine namespace