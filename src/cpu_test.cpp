#include <cassert>
#include <iostream>

#include "cpu.h"

void test_initial_state()
{
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

void test_set_flags()
{
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

void LDA()
{
    CPU cpu;

    // 即値モードでのLDAテスト
    std::cout << "Testing LDA Immediate" << std::endl;
    cpu.LDA(0x42, CPU::AddressingMode::Immediate); // 即値0x42をAレジスタにロード
    assert(cpu.A == 0x42);                         // Aレジスタの値を確認
    assert(cpu.getFlag(CPU::Flags::Z) == 0);       // Zeroフラグが0であることを確認
    assert(cpu.getFlag(CPU::Flags::N) == 0);       // Negativeフラグが0であることを確認

    // ゼロページモードでのLDAテスト
    std::cout << "Testing LDA Zero Page" << std::endl;
    cpu.writeMemory(0x10, 0x00); // メモリ0x10に0を書き込む（Zeroフラグが立つように）
    cpu.LDA(0x10, CPU::AddressingMode::ZeroPage);
    assert(cpu.A == 0x00);                   // Aレジスタの値を確認
    assert(cpu.getFlag(CPU::Flags::Z) == 1); // Zeroフラグが1であることを確認
    assert(cpu.getFlag(CPU::Flags::N) == 0); // Negativeフラグが0であることを確認

    // 絶対アドレスモードでのLDAテスト
    std::cout << "Testing LDA Absolute" << std::endl;
    cpu.writeMemory(0x2000, 0x80); // メモリ0x2000に0x80を書き込む（Negativeフラグが立つように）
    cpu.LDA(0x2000, CPU::AddressingMode::Absolute);
    assert(cpu.A == 0x80);                   // Aレジスタの値を確認
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // Zeroフラグが0であることを確認
    assert(cpu.getFlag(CPU::Flags::N) == 1); // Negativeフラグが1であることを確認

    // インデックスモード (Xインデックス) でのLDAテスト
    std::cout << "Testing LDA X Indexed" << std::endl;
    uint8_t address = 0x10;      // 仮のゼロページアドレス
    cpu.writeMemory(0x15, 0x01); // メモリ0x15に0x01を書き込む
    cpu.X = 0x05;                // Xレジスタにインデックス値をセット
    cpu.LDA(address, CPU::AddressingMode::IndexedX);
    assert(cpu.A == 0x01);                   // Aレジスタの値を確認
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // Zeroフラグが0であることを確認
    assert(cpu.getFlag(CPU::Flags::N) == 0); // Negativeフラグが0であることを確認

    // インデックスモード (Yインデックス) でのLDAテスト
    std::cout << "Testing LDA Y Indexed" << std::endl;
    cpu.writeMemory(0x15, 0x02); // メモリ0x15に0x02を書き込む
    cpu.Y = 0x05;                // Yレジスタにインデックス値をセット
    cpu.LDA(address, CPU::AddressingMode::IndexedY);
    assert(cpu.A == 0x02);                   // Aレジスタの値を確認
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // Zeroフラグが0であることを確認
    assert(cpu.getFlag(CPU::Flags::N) == 0); // Negativeフラグが0であることを確認

    std::cout << "LDA tests passed." << std::endl;
}

void STA()
{
    CPU cpu;

    // 即値モードでのSTAテスト（STAでは即値モードは使えません）
    std::cout << "Testing STA Immediate" << std::endl;
    try
    {
        cpu.STA(0x42, CPU::AddressingMode::Immediate); // 即値モードはSTAでは使えません
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl; // 例外メッセージを表示
    }

    // ゼロページモードでのSTAテスト
    std::cout << "Testing STA Zero Page" << std::endl;
    cpu.A = 0x77;                                 // Aレジスタに適当な値を設定
    cpu.STA(0x10, CPU::AddressingMode::ZeroPage); // ゼロページアドレス0x10にAレジスタの値を格納
    std::cout << "Memory at 0x10: " << +cpu.readMemory(0x10) << std::endl;

    // 絶対モードでのSTAテスト
    std::cout << "Testing STA Absolute" << std::endl;
    cpu.A = 0x55;                                   // Aレジスタに適当な値をセット
    cpu.STA(0x2000, CPU::AddressingMode::Absolute); // AbsoluteアドレスタイプでSTAを実行
    std::cout << "Memory at 0x2000: " << +cpu.readMemory(0x2000) << std::endl;

    // ゼロページXインデックスモードでのSTAテスト
    std::cout << "Testing STA Zero Page X Indexed" << std::endl;
    cpu.X = 0x02;                                  // Xレジスタに2を設定
    cpu.STA(0x10, CPU::AddressingMode::ZeroPageX); // ゼロページXインデックス0x12（0x10 + X）にAレジスタの値を格納
    std::cout << "Memory at 0x12: " << +cpu.readMemory(0x12) << std::endl;

    // 絶対XインデックスモードでのSTAテスト
    std::cout << "Testing STA Absolute X Indexed" << std::endl;
    cpu.X = 0x02; // Xレジスタに2を設定
    cpu.STA(0x2000,
            CPU::AddressingMode::AbsoluteX); // 絶対アドレスXインデックス0x2002（0x2000 + X）にAレジスタの値を格納
    std::cout << "Memory at 0x2002: " << +cpu.readMemory(0x2002) << std::endl;

    // ゼロページYインデックスモードでのSTAテスト
    std::cout << "Testing STA Zero Page Y Indexed" << std::endl;
    cpu.Y = 0x03;                                  // Yレジスタに3を設定
    cpu.STA(0x10, CPU::AddressingMode::ZeroPageY); // ゼロページYインデックス0x13（0x10 + Y）にAレジスタの値を格納
    std::cout << "Memory at 0x13: " << +cpu.readMemory(0x13) << std::endl;

    // 絶対YインデックスモードでのSTAテスト
    std::cout << "Testing STA Absolute Y Indexed" << std::endl;
    cpu.Y = 0x03; // Yレジスタに3を設定
    cpu.STA(0x2000,
            CPU::AddressingMode::AbsoluteY); // 絶対アドレスYインデックス0x2003（0x2000 + Y）にAレジスタの値を格納
    std::cout << "Memory at 0x2003: " << +cpu.readMemory(0x2003) << std::endl;
}

void testTAX()
{
    CPU cpu;

    // 初期状態の設定
    cpu.A = 0x42; // 任意の値をAレジスタに設定
    cpu.X = 0;    // Xレジスタは0から始める

    cpu.TAX(); // TAX命令を実行

    // 結果の確認
    assert(cpu.X == cpu.A);                               // Aの値がXに転送されているか
    assert(cpu.getFlag(CPU::Flags::Z) == (cpu.X == 0));   // ゼロフラグの設定が正しいか
    assert(cpu.getFlag(CPU::Flags::N) == (cpu.X & 0x80)); // ネガティブフラグの設定が正しいか
    std::cout << "TAX test passed.\n";
}

void testBRK()
{
    CPU cpu;

    // 状態設定
    cpu.PC = 0x1234;                   // 現在のPC
    cpu.SP = 0xFD;                     // スタックポインタの初期位置
    cpu.setFlag(CPU::Flags::I, false); // 割り込みフラグをクリア

    cpu.BRK(); // BRK命令の実行

    // 結果の確認
    uint16_t expectedPC = (cpu.memory[0xFFFF] << 8) | cpu.memory[0xFFFE];
    assert(cpu.PC == expectedPC);               // PCが割り込みベクタに変更されたか
    assert(cpu.SP == 0xFA);                     // スタックポインタが3つ分減少しているか
    assert(cpu.getFlag(CPU::Flags::I) == true); // 割り込みフラグがセットされているか
    std::cout << "BRK test passed.\n";
}

void testINX()
{
    CPU cpu;

    cpu.X = 0x00; // 初期値を設定
    cpu.INX();
    assert(cpu.X == 0x01);                   // Xがインクリメントされているか
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // ゼロフラグが正しく設定されているか
    assert(cpu.getFlag(CPU::Flags::N) == 0); // 負フラグが正しく設定されているか

    cpu.X = 0xFF;
    cpu.INX();
    assert(cpu.X == 0x00);                   // Xがオーバーフローで0になる
    assert(cpu.getFlag(CPU::Flags::Z) == 1); // ゼロフラグがセットされているか
    assert(cpu.getFlag(CPU::Flags::N) == 0); // 負フラグがクリアされているか

    std::cout << "INX test passed.\n";
}

void testMachineCodeExecution()
{
    CPU cpu;

    // メモリにマシンコードをロード
    cpu.writeMemory(0x8000, 0xA9); // LDA #$C0
    cpu.writeMemory(0x8001, 0xC0);
    cpu.writeMemory(0x8002, 0xAA); // TAX
    cpu.writeMemory(0x8003, 0xE8); // INX
    cpu.writeMemory(0x8004, 0x00); // BRK

    // プログラムカウンタをセット
    cpu.PC = 0x8000;

    cpu.printMemory(0x8000, 16); // 0x8000 から16バイトを表示

    // 命令を順に実行
    cpu.step(); // LDA #$C0
    assert(cpu.A == 0xC0);
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // A != 0 なので Zフラグはクリア
    assert(cpu.getFlag(CPU::Flags::N) == 1); // Aの最上位ビットが1なので Nフラグはセット

    cpu.step(); // TAX
    assert(cpu.X == 0xC0);

    cpu.step();                              // INX
    assert(cpu.X == 0xC1);                   // Xがインクリメントされる
    assert(cpu.getFlag(CPU::Flags::Z) == 0); // X != 0なので Zフラグはクリア
    assert(cpu.getFlag(CPU::Flags::N) == 1); // Xの最上位ビットが1なので Nフラグはセット

    cpu.step();                              // BRK
    assert(cpu.getFlag(CPU::Flags::B) == 1); // BRK実行で Bフラグがセット

    std::cout << "testMachineCodeExecution passed." << std::endl;
}

void CPU_test()
{

    CPU cpu;

    // LDA();  // LDA命令のテスト
    // STA();  // STA命令のテスト

    // testTAX();
    // testBRK();
    // testINX();

    // testMachineCodeExecution();

    // テスト1：プログラムのロードと実行
    std::vector<uint8_t> program = {0xA9, 0x10, 0xE8, 0x00}; // LDA #$10; INX; BRK
    cpu.loadProgram(program);
    cpu.run();

    cpu.printMemory(0x8000, 16); // 0x8000 から16バイトを表示


    // Aレジスタが10、Xレジスタが1に設定されていることを確認
    assert(cpu.A == 0x10);
    assert(cpu.X == 1);

    std::cout << "All tests passed!" << std::endl;
}
