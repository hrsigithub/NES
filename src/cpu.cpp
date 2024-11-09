#include <stdexcept>
#include <algorithm> 
#include <iostream>


#include "cpu.h"

// コンストラクタでレジスタを初期化
CPU::CPU() : A(0), X(0), Y(0), PC(0), SP(0xFD), P(0x24) {
    std::fill(std::begin(memory), std::end(memory), 0);  // メモリを0で初期化
} // 初期値設定

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


// クラス内で定義するように変更
void CPU::writeMemory(uint16_t address, uint8_t value) {
    memory[address] = value;  // メモリに値を書き込む
}

uint8_t CPU::readMemory(uint16_t address) {
    return memory[address];  // メモリから値を読み込む
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

// STA命令の実装
void CPU::STA(uint16_t address, AddressingMode mode) {
    uint16_t effectiveAddress = address;

    switch (mode) {
        case AddressingMode::ZeroPage:
            effectiveAddress = address & 0xFF;  // ゼロページアドレス
            break;
        case AddressingMode::Absolute:
            // 絶対アドレスモードはそのまま使用
            break;
        case AddressingMode::IndexedX:
            effectiveAddress = (address + X) & 0xFFFF;  // Xインデックス付き
            break;
        case AddressingMode::IndexedY:
            effectiveAddress = (address + Y) & 0xFFFF;  // Yインデックス付き
            break;
        default:
            break;
    }

    // メモリに書き込む前に、アドレスが有効であるか確認
    if (effectiveAddress < 0x10000) {
        writeMemory(effectiveAddress, A);  // Aレジスタの内容をメモリに書き込む
    } else {
        // アドレスが範囲外の場合のエラーハンドリング（必要に応じて追加）
        std::cerr << "Error: Invalid address 0x" << std::hex << effectiveAddress << std::dec << std::endl;
    }
}

