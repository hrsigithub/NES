#include <stdexcept>
#include "cpu.h"

// コンストラクタでレジスタを初期化
CPU::CPU() : A(0), X(0), Y(0), PC(0), SP(0xFD), P(0x24) {} // 初期値設定

// ステータスフラグの操作メソッド
void CPU::setFlag(Flags flag, bool value) {
    if (value)
        P |= static_cast<uint8_t>(flag);  // フラグをセット
    else
        P &= ~static_cast<uint8_t>(flag); // フラグをクリア
}

bool CPU::getFlag(Flags flag) const {
    return (P & static_cast<uint8_t>(flag)) > 0;
}


// メモリを読むメソッド (仮にこうした関数がある場合)
uint8_t CPU::readMemory(uint8_t address) {
    // 実際には、メモリから値を読み込む処理を実装する必要があります
    // ここでは仮にメモリがゼロページのみと仮定して動作します
    return 0xFF;  // 仮に0xFFを返す
}

void CPU::LDA(uint16_t address, AddressingMode mode) {
    uint8_t value;

    switch (mode) {
        case AddressingMode::Immediate:
            value = address & 0xFF;  // 即値モード：addressは即値として使用
            break;
        case AddressingMode::ZeroPage:
            value = readMemory(address);  // ゼロページモード：低位8ビットがゼロページアドレス
            break;
        case AddressingMode::Absolute:
            value = readMemory(address);  // 絶対アドレスモード
            break;
        case AddressingMode::IndexedX:
            value = readMemory(address + X);  // 絶対アドレス + X (インデックスモード X)
            break;
        case AddressingMode::IndexedY:
            value = readMemory(address + Y);  // 絶対アドレス + Y (インデックスモード Y)
            break;
        default:
            throw std::invalid_argument("Unsupported addressing mode");
    }

    A = value;  // Aレジスタに値をロード

    setFlag(Flags::Z, A == 0);  // ゼロフラグの設定
    setFlag(Flags::N, A & 0x80);  // 負フラグの設定
}
