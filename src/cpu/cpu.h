#ifndef CPU_CLASS_H
#define CPU_CLASS_H

#include <cstddef>
#include <cstdint>
#include <vector>

class CPU
{
  private:
    uint8_t fetch();

  public:
    uint8_t memory[0x10000]; // メモリのサイズ（64KB）

    // ステータスフラグ (Processor Status Register) のビット位置
    enum class Flags : uint8_t
    {
        C = 1 << 0, // Carry Flag
        Z = 1 << 1, // Zero Flag
        I = 1 << 2, // Interrupt Disable
        D = 1 << 3, // Decimal Mode (NESでは使われません)
        B = 1 << 4, // Break Command
        U = 1 << 5, // Unused (常に1)
        V = 1 << 6, // Overflow Flag
        N = 1 << 7  // Negative Flag
    };

    enum class AddressingMode
    {
        Immediate,
        ZeroPage,
        ZeroPageX,
        ZeroPageY,
        Absolute,
        AbsoluteX,
        AbsoluteY,
        IndexedX,
        IndexedY
    };

    CPU();
    void loadProgram(const std::vector<uint8_t> &program);
    void reset();
    void run();


    // ステータスフラグの操作メソッド
    void setFlag(Flags flag, bool value);
    bool getFlag(Flags flag) const;

    // 主要なレジスタ
    uint8_t A;   // Accumulator (A)
    uint8_t X;   // Index Register (X)
    uint8_t Y;   // Index Register (Y)
    uint16_t PC; // Program Counter (PC)
    uint8_t SP;  // Stack Pointer (SP)
    uint8_t P;   // Processor Status Register (P)

    uint8_t readMemory(uint16_t address); // メモリを読むメソッドの宣言
    void writeMemory(uint16_t address, uint8_t value);
    void pushStack(uint8_t value);
    void step();

    
    void printMemory(const uint8_t memory[], size_t size);
    void printMemory(uint16_t address, size_t size);

    // 命令の実装
    void LDA(uint16_t address, AddressingMode mode);
    void STA(uint16_t address, AddressingMode mode);

    void TAX();
    void BRK();
    void INX();
};

#endif
