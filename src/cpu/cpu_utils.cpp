
#include <iomanip>
#include <iostream>

#include "cpu.h"

void CPU::printMemory(const uint8_t memory[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        // 16進数で表示
        std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << i << ": ";
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)memory[i];
        std::cout << std::endl;
    }
}

void CPU::printMemory(uint16_t startAddress, size_t size)
{
    for (size_t i = 0; i < size; i += 16)
    {
        // 各行の先頭のアドレスを表示
        std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << startAddress + i << ": ";

        // 16バイト分のメモリを表示
        for (size_t j = 0; j < 16; j++)
        {
            // メモリのバイトを表示
            uint8_t value = readMemory(startAddress + i + j);
            std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)value << " ";
        }
        std::cout << std::endl;
    }
}
