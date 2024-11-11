#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "cpu.h"

// コンストラクタでレジスタを初期化
CPU::CPU() : A(0), X(0), Y(0), PC(0), SP(0xFD), P(0x24)
{
    std::fill(std::begin(memory), std::end(memory), 0); // メモリを0で初期化
} // 初期値設定

// ステータスフラグの操作メソッド
void CPU::setFlag(Flags flag, bool value)
{
    if (value)
        P |= static_cast<uint8_t>(flag); // フラグをセット
    else
        P &= ~static_cast<uint8_t>(flag); // フラグをクリア
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

void CPU::LDA(uint16_t address, AddressingMode mode)
{
    uint8_t value;

    switch (mode)
    {
    case AddressingMode::Immediate:
        value = address & 0xFF; // 即値モード：addressは即値として使用　0xFFがないと、address
                                // が16ビットであれば意図しない動作を引き起こす可能性がある
        break;
    case AddressingMode::ZeroPage:
        value = readMemory(address); // ゼロページモード：低位8ビットがゼロページアドレス
        break;
    case AddressingMode::Absolute:
        value = readMemory(address); // 絶対アドレスモード
        break;
    case AddressingMode::IndexedX:
        value = readMemory(address + X); // 絶対アドレス + X (インデックスモード X)
        break;
    case AddressingMode::IndexedY:
        value = readMemory(address + Y); // 絶対アドレス + Y (インデックスモード Y)
        break;
    default:
        throw std::invalid_argument("Unsupported addressing mode");
    }

    A = value; // Aレジスタに値をロード

    setFlag(Flags::Z, A == 0);   // ゼロフラグの設定
    setFlag(Flags::N, A & 0x80); // 負フラグの設定
}

// STA命令の実装
void CPU::STA(uint16_t address, AddressingMode mode)
{
    uint16_t effectiveAddress = address;

    switch (mode)
    {
    case AddressingMode::ZeroPage:
        effectiveAddress = address & 0xFF; // ゼロページアドレス
        break;
    case AddressingMode::Absolute:
        // 絶対アドレスモードはそのまま使用
        break;
    case AddressingMode::IndexedX:
        effectiveAddress = (address + X) & 0xFFFF; // Xインデックス付き
        break;
    case AddressingMode::IndexedY:
        effectiveAddress = (address + Y) & 0xFFFF; // Yインデックス付き
        break;
    default:
        throw std::invalid_argument("Unsupported addressing mode in STA");
    }

    // メモリに書き込む前に、アドレスが有効であるか確認
    if (effectiveAddress <= 0xFFFF)
    {
        writeMemory(effectiveAddress, A); // Aレジスタの内容をメモリに書き込む
    }
    else
    {
        // アドレスが範囲外の場合のエラーハンドリング（必要に応じて追加）
        std::cerr << "Error: Invalid address 0x" << std::hex << effectiveAddress << std::dec << std::endl;
    }
}

void CPU::TAX()
{
    X = A;                       // アキュムレータの値をXレジスタにコピー
    setFlag(Flags::Z, X == 0);   // ゼロフラグを設定
    setFlag(Flags::N, X & 0x80); // 負フラグを設定
}

void CPU::BRK()
{
    PC += 2; // BRK命令は2バイトなので、次の命令に進める

    // ステータスレジスタの内容とPCをスタックに保存
    pushStack((PC >> 8) & 0xFF); // 上位バイト
    pushStack(PC & 0xFF);        // 下位バイト

    // フラグの設定。割り込みフラグ（I）をセットし、ブレークフラグ（B）もセット
    setFlag(Flags::B, true); // Bフラグをセット
    setFlag(Flags::I, true); // Iフラグをセット

    // ステータスレジスタに予約ビットを含めてスタックにプッシュ
    uint8_t statusWithReservedBits = P | 0x30; // 0b0011xxxx（予約ビットを含む）
    pushStack(statusWithReservedBits);

    // 割り込みベクタから新しいPCを取得
    uint8_t low = readMemory(0xFFFE);
    uint8_t high = readMemory(0xFFFF);
    PC = (high << 8) | low;
}

void CPU::INX()
{
    X += 1; // Xレジスタの値をインクリメント

    // ゼロフラグの設定：Xレジスタが0であればZフラグをセット
    setFlag(Flags::Z, X == 0);

    // 負フラグの設定：Xレジスタの最上位ビットが1ならばNフラグをセット
    setFlag(Flags::N, X & 0x80);
}
