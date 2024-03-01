#include "Cpu.h"
#include <cassert>
#include <iostream>

void TestMemoryReadNextInstruction()
{
    emulator::Memory mem;
    mem.mBuffer[0] = 0xAB;
    mem.mBuffer[1] = 0xFF;
    mem.mBuffer[2] = 0xAB;
    mem.mBuffer[3] = 0xCD;
    uint16_t start_offset = 0x0000;

    emulator::Instruction i(mem.ReadNextInstruction(start_offset)); 
    std::cout << "going here" << std::endl;
    assert(i.ins == 0xABFF);
    std::cout << "ins=" << i.ins << std::endl;
    std::cout << "i.X=" << i.X << std::endl;
    assert(i.X == 0x000B);
    assert(i.Y == 0xF);
    assert(i.N == 0xF);
    assert(i.NN == 0xFF);
    assert(i.NNN == 0xBFF);
    assert(i.I == 0xA);

    i = emulator::Instruction(mem.ReadNextInstruction(start_offset)); 
    std::cout << "going here" << std::endl;
    assert(i.ins == 0xABCD);
    std::cout << "ins=" << i.ins << std::endl;
    std::cout << "i.X=" << i.X << std::endl;
    assert(i.X == 0xB);
    assert(i.Y == 0xC);
    assert(i.N == 0xD);
    assert(i.NN == 0xCD);
    assert(i.NNN == 0xBCD);
    assert(i.I == 0xA);
}

void TestCpuClearScreen00E0()
{
    emulator::Cpu cpu;

    // set all pixels on
    for (int i = 0; i < emulator::DISPLAY_HEIGHT; i++)
        for (int j = 0; j < emulator::DISPLAY_WIDTH; j++)
            cpu.mPixelBuffer[i][j] = true;
    
    emulator::Instruction ins(0x00E0);
    cpu.Decode(ins);
    for (int i = 0; i < emulator::DISPLAY_HEIGHT; i++)
        for (int j = 0; j < emulator::DISPLAY_WIDTH; j++)
            assert(cpu.mPixelBuffer[i][j] == false);
}

void TestCpuJump1NNN()
{
    emulator::Cpu cpu;
    emulator::Instruction ins(0x1BCD);

    cpu.Decode(ins);
    assert(cpu.mProgramCounter == 0xBCD);
}

void TestCpuSubroutines00EE()
{

}

void TestCpuSubroutines2NNN()
{

}

void TestCpuSkipConditionally3XNN()
{

}

void TestCpuSkipConditionally4XNN()
{
    
}

void TestCpuSkipConditionally5XY0()
{
    
}

void TestCpuSkipConditionally9XY0()
{
    
}
// do these first
void TestCpuSet6XNN()
{
    emulator::Cpu cpu;
    emulator::Instruction ins(0x6BFD);

    cpu.Decode(ins);
    assert(cpu.mRegisters[0xB] == 0xFD);
}

void TestCpuAdd7XNN()
{
    emulator::Cpu cpu;
    emulator::Instruction ins(0x6BFD);

    // setup the register with a value
    cpu.Decode(ins);
    assert(cpu.mRegisters[0xB] == 0xFD);

    // add to the register
    ins = emulator::Instruction(0x7BFF);
    cpu.Decode(ins);
    
    assert(cpu.mRegisters[0xB] == (emulator::byte) ((emulator::byte) 0xFF + (emulator::byte) 0xFD));
}

void TestCpuSetIndexANNN()
{
    emulator::Cpu cpu;
    emulator::Instruction ins(0xADCB);

    cpu.Decode(ins);
    assert(cpu.mIndexRegister == 0xDCB);
}

void TestCpuDrawDXYN()
{

}


int main(int argc, char *argv[])
{
    TestMemoryReadNextInstruction();
    TestCpuClearScreen00E0();
    TestCpuJump1NNN();
    TestCpuSet6XNN();
    TestCpuAdd7XNN();
    TestCpuSetIndexANNN();
}
