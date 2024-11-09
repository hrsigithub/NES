#ifndef CPU_H
#define CPU_H

#include <cstdint>

class CPU {
private:
public:
    // ステータスフラグ (Processor Status Register) のビット位置
    enum class Flags : uint8_t {  // enum classを使用
        C = 1 << 0,  // Carry Flag
        Z = 1 << 1,  // Zero Flag
        I = 1 << 2,  // Interrupt Disable
        D = 1 << 3,  // Decimal Mode (NESでは使われません)
        B = 1 << 4,  // Break Command
        U = 1 << 5,  // Unused (常に1)
        V = 1 << 6,  // Overflow Flag
        N = 1 << 7   // Negative Flag
    };

    enum class AddressingMode {
        Immediate,
        ZeroPage,
        Absolute,
        IndexedX,
        IndexedY
    };


    CPU();  // コンストラクタの宣言

    // ステータスフラグの操作メソッド
    void setFlag(Flags flag, bool value);
    bool getFlag(Flags flag) const;

    // 主要なレジスタ
    uint8_t A;    // Accumulator (A)
    uint8_t X;    // Index Register (X)
    uint8_t Y;    // Index Register (Y)
    uint16_t PC;  // Program Counter (PC)
    uint8_t SP;   // Stack Pointer (SP)
    uint8_t P;    // Processor Status Register (P)


    uint8_t readMemory(uint8_t address);  // メモリを読むメソッドの宣言

// LDA命令の実装
    void LDA(uint16_t address, AddressingMode mode);


};

#endif // CPU_H