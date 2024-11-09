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

void STA() {
    CPU cpu;

// 即値モードでのSTAテスト（STAでは即値モードは使えません）
    std::cout << "Testing STA Immediate" << std::endl;
    try {
        cpu.STA(0x42, CPU::AddressingMode::Immediate);  // 即値モードはSTAでは使えません
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;  // 例外をキャッチして表示
    }

    // ゼロページモードでのSTAテスト
    std::cout << "Testing STA Zero Page" << std::endl;
    cpu.STA(0x10, CPU::AddressingMode::ZeroPage);  // ゼロページアドレス0x10にAレジスタの値を格納
    std::cout << "Memory at 0x10: " << +cpu.readMemory(0x10) << std::endl;

    // 絶対モードでのSTAテスト
    std::cout << "Testing STA Absolute" << std::endl;
    cpu.A = 0x55;  // Aレジスタに適当な値をセット
    cpu.STA(0x2000, CPU::AddressingMode::Absolute);  // AbsoluteアドレスタイプでSTAを実行
    std::cout << "Memory at 0x2000: " << +cpu.readMemory(0x2000) << std::endl;

// ゼロページXインデックスモードでのSTAテスト
    std::cout << "Testing STA Zero Page X Indexed" << std::endl;
    cpu.X = 0x02;  // Xレジスタに2を設定
    cpu.STA(0x10, CPU::AddressingMode::ZeroPageX);  // ゼロページXインデックス0x12（0x10 + X）にAレジスタの値を格納
    std::cout << "Memory at 0x12: " << +cpu.readMemory(0x12) << std::endl;

    // 絶対XインデックスモードでのSTAテスト
    std::cout << "Testing STA Absolute X Indexed" << std::endl;
    cpu.X = 0x02;  // Xレジスタに2を設定
    cpu.STA(0x2000, CPU::AddressingMode::AbsoluteX);  // 絶対アドレスXインデックス0x2002（0x2000 + X）にAレジスタの値を格納
    std::cout << "Memory at 0x2002: " << +cpu.readMemory(0x2002) << std::endl;

    // ゼロページYインデックスモードでのSTAテスト
    std::cout << "Testing STA Zero Page Y Indexed" << std::endl;
    cpu.Y = 0x03;  // Yレジスタに3を設定
    cpu.STA(0x10, CPU::AddressingMode::ZeroPageY);  // ゼロページYインデックス0x13（0x10 + Y）にAレジスタの値を格納
    std::cout << "Memory at 0x13: " << +cpu.readMemory(0x13) << std::endl;

    // 絶対YインデックスモードでのSTAテスト
    std::cout << "Testing STA Absolute Y Indexed" << std::endl;
    cpu.Y = 0x03;  // Yレジスタに3を設定
    cpu.STA(0x2000, CPU::AddressingMode::AbsoluteY);  // 絶対アドレスYインデックス0x2003（0x2000 + Y）にAレジスタの値を格納
    std::cout << "Memory at 0x2003: " << +cpu.readMemory(0x2003) << std::endl;

    // std::cout << "Testing STA Absolute" << std::endl;
    // cpu.A = 0x55;  // Aレジスタに適当な値をセット
    // cpu.STA(0x2000, CPU::AddressingMode::Absolute);
    // std::cout << "Memory at 0x55: " << +cpu.readMemory(0x55) << std::endl;

}

void CPU_test() {
    CPU cpu;

    std::cout << "Starting LDA Test:" << std::endl;
    LDA();  // LDA命令のテスト

    std::cout << "Starting STA Test:" << std::endl;
    STA();  // STA命令のテスト
}
