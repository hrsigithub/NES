#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "cpu.h"

// コンストラクタでレジスタを初期化
// CPU::CPU() : A(0), X(0), Y(0), PC(0), SP(0xFD), P(0x24)
// {
//     std::fill(std::begin(memory), std::end(memory), 0); // メモリを0で初期化
// } // 初期値設定

CPU::CPU()
{
    reset();
}

void CPU::reset()
{
    A = 0;
    X = 0;
    Y = 0;
    PC = 0x8000;                                        // プログラム開始アドレス
    SP = 0xFD;                                          // スタックポインタ初期値
    P = 0x24;                                           // ステータスレジスタ初期値
    std::fill(std::begin(memory), std::end(memory), 0); // メモリをクリア
}

// プログラムを指定されたアドレスにロードする関数
void CPU::loadProgram(const std::vector<uint8_t> &program)
{
    std::copy(program.begin(), program.end(), memory + 0x8000); // メモリの0x8000番地からプログラムを配置
    PC = 0x8000;                                                // プログラム開始位置をPCに設定
}

// 命令の実行を開始する関数
void CPU::run()
{
    while (true)
    {
        uint8_t opcode = fetch();
        switch (opcode)
        {
        case 0xA9: // LDA Immediate
            LDA(fetch(), AddressingMode::Immediate);
            break;
        case 0xE8: // INX
            INX();
            break;
        case 0x00: // BRK
            BRK();
            return; // BRK命令で停止
        default:
            throw std::runtime_error("Unknown opcode");
        }
    }
}

// 命令をフェッチしてPCを更新
uint8_t CPU::fetch()
{
    return memory[PC++];
}

// ステータスフラグの操作メソッド
void CPU::setFlag(Flags flag, bool value)
{
    P = value ? (P | static_cast<uint8_t>(flag)) : (P & ~static_cast<uint8_t>(flag));
}

bool CPU::getFlag(Flags flag) const
{
    return (P & static_cast<uint8_t>(flag)) > 0;
}

void CPU::writeMemory(uint16_t address, uint8_t value)
{
    memory[address] = value; // メモリに値を書き込む
}

uint8_t CPU::readMemory(uint16_t address)
{
    return memory[address]; // メモリから値を読み込む
}

void CPU::pushStack(uint8_t value)
{
    memory[0x0100 + SP] = value; // スタックに値を書き込み
    SP--;                        // スタックポインタをデクリメント
}

void CPU::step()
{
    uint8_t opcode = readMemory(PC); // PC（プログラムカウンタ）から命令をフェッチ
    PC++;                            // 次の命令に進む

    // opcode に基づいて適切な命令を実行
    switch (opcode)
    {
    case 0xA9: // LDA Immediate
        LDA(readMemory(PC), AddressingMode::Immediate);
        PC++;
        break;
    case 0xAA: // TAX
        TAX();
        break;
    case 0xE8: // INX
        INX();
        break;
    case 0x00: // BRK
        BRK();
        break;
    // その他の命令もここに追加
    default:
        throw std::runtime_error("Unknown opcode");
    }
}
