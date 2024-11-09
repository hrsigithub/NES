// cpu.cpp
#include <cstdint>

class CPU6502 {
public:
    // 主要なレジスタ
    uint8_t A;    // Accumulator (A)
    uint8_t X;    // Index Register (X)
    uint8_t Y;    // Index Register (Y)
    uint16_t PC;  // Program Counter (PC)
    uint8_t SP;   // Stack Pointer (SP)
    uint8_t P;    // Processor Status Register (P)

    // ステータスフラグ (Processor Status Register) のビット位置
    enum Flags {
        C = 1 << 0,  // Carry Flag
        Z = 1 << 1,  // Zero Flag
        I = 1 << 2,  // Interrupt Disable
        D = 1 << 3,  // Decimal Mode (NESでは使われません)
        B = 1 << 4,  // Break Command
        U = 1 << 5,  // Unused (常に1)
        V = 1 << 6,  // Overflow Flag
        N = 1 << 7   // Negative Flag
    };

    // コンストラクタでレジスタを初期化
    CPU6502() : A(0), X(0), Y(0), PC(0), SP(0xFD), P(0x24) {} // 初期値設定

    // ステータスフラグの操作メソッド
    void setFlag(Flags flag, bool value) {
        if (value)
            P |= flag;  // フラグをセット
        else
            P &= ~flag; // フラグをクリア
    }

    bool getFlag(Flags flag) const {
        return (P & flag) > 0;
    }
};
