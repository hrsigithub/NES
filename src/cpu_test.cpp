#include <iostream>
#include "cpu.h"

void test_initial_state() {
    CPU cpu;
    // 初期状態のテストコード

    // 初期状態のレジスタの確認
    std::cout << "Initial State:" << std::endl;
    std::cout << "A (Accumulator): " << (int)cpu.A << std::endl;
    std::cout << "X (Index Register): " << (int)cpu.X << std::endl;
    std::cout << "Y (Index Register): " << (int)cpu.Y << std::endl;
    std::cout << "PC (Program Counter): " << cpu.PC << std::endl;
    std::cout << "SP (Stack Pointer): " << (int)cpu.SP << std::endl;
    std::cout << "P (Processor Status Register): " << (int)cpu.P << std::endl;

// 初期状態のレジスタの確認
    std::cout << "Initial State:" << std::endl;
    std::cout << "A (Accumulator): " << (int)cpu.A << std::endl;
    std::cout << "X (Index Register): " << (int)cpu.X << std::endl;
    std::cout << "Y (Index Register): " << (int)cpu.Y << std::endl;
    std::cout << "PC (Program Counter): " << cpu.PC << std::endl;
    std::cout << "SP (Stack Pointer): " << (int)cpu.SP << std::endl;
    std::cout << "P (Processor Status Register): " << (int)cpu.P << std::endl;

}

void test_set_flags() {
    CPU cpu;
    // フラグ設定のテストコード
    // フラグの設定
    cpu.setFlag(CPU::Flags::C, true);  // Carryフラグをセット
    cpu.setFlag(CPU::Flags::Z, false); // Zeroフラグをクリア


// フラグが変更されたか確認
    std::cout << "\nFlags after setting and clearing:" << std::endl;
    std::cout << "Carry Flag: " << cpu.getFlag(CPU::Flags::C) << std::endl;
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
}

void LDA() {
    CPU cpu;

 // 即値モードでのLDAテスト
    std::cout << "Testing LDA Immediate" << std::endl;
    cpu.LDA(0x42, CPU::AddressingMode::Immediate);  // 即値0x42をAレジスタにロード
    std::cout << "A: " << +cpu.A << std::endl;  // Aレジスタの値を表示
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
    std::cout << "Negative Flag: " << cpu.getFlag(CPU::Flags::N) << std::endl;

    // ゼロページモードでのLDAテスト
    std::cout << "Testing LDA Zero Page" << std::endl;
    cpu.LDA(0x10, CPU::AddressingMode::ZeroPage);  // ゼロページアドレス0x10から読み込み
    std::cout << "A: " << +cpu.A << std::endl;
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
    std::cout << "Negative Flag: " << cpu.getFlag(CPU::Flags::N) << std::endl;

// 絶対アドレスモードでのLDAテスト
    std::cout << "Testing LDA Absolute" << std::endl;
    cpu.LDA(0x2000, CPU::AddressingMode::Absolute);  // 絶対アドレス0x2000から読み込み
    std::cout << "A: " << +cpu.A << std::endl;
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
    std::cout << "Negative Flag: " << cpu.getFlag(CPU::Flags::N) << std::endl;

// インデックスモード (Xインデックス) でのLDAテスト
    std::cout << "Testing LDA X Indexed" << std::endl;
    uint8_t address = 0x10; // 仮のゼロページアドレス
    cpu.X = 0x05;  // Xレジスタにインデックス値をセット
    cpu.LDA(address, CPU::AddressingMode::IndexedX);  // XインデックスモードでのLDA
    std::cout << "A: " << +cpu.A << std::endl;
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
    std::cout << "Negative Flag: " << cpu.getFlag(CPU::Flags::N) << std::endl;

    // インデックスモード (Yインデックス) でのLDAテスト
    std::cout << "Testing LDA Y Indexed" << std::endl;
    cpu.Y = 0x05;  // Yレジスタにインデックス値をセット
    cpu.LDA(address, CPU::AddressingMode::IndexedY);  // YインデックスモードでのLDA
    std::cout << "A: " << +cpu.A << std::endl;
    std::cout << "Zero Flag: " << cpu.getFlag(CPU::Flags::Z) << std::endl;
    std::cout << "Negative Flag: " << cpu.getFlag(CPU::Flags::N) << std::endl;

}
