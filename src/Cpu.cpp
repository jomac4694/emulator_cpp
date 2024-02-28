#include "Cpu.h"

namespace emulator
{
    
    struct Instruction
    {
        Instruction(uint16_t instruction)
        {
            ins = instruction;
            I = instruction >> 12 & 0x000F;
            X = instruction >> 8 & 0x0F00;
            Y = instruction >> 4 & 0x00F0;
            N = instruction & 0x000F;
            NN = instruction & 0x00FF;
            NNN = instruction & 0x0FFF;
        }
        
        uint16_t ins;
        uint16_t I;
        uint16_t X;
        uint16_t Y;
        uint16_t N;
        uint16_t NN;
        uint16_t NNN;

    };

    void Cpu::Decode(uint16_t instruction)
    {
        Instruction instruct(instruction);

        switch (instruct.I)
        {
            case 0x0:
                if (instruct.ins == 0x00EE)
                {
                    mProgramCounter = mStack.top();
                    mStack.pop();
                }
                else if (instruct.ins = 0x00E0)
                {

                }
                break;
            case 0x1:
                mProgramCounter = instruct.NNN;
                break;
            case 0x2:
                mStack.push(mProgramCounter);
                mProgramCounter = instruct.NNN;
                break;
            case 0x3:
                if (mRegisters[instruct.X] == instruct.NN)
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x4:
                if (mRegisters[instruct.X] != instruct.NN)
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x5:
                if (mRegisters[instruct.X] == mRegisters[instruct.Y])
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x9:
                if (mRegisters[instruct.X] != mRegisters[instruct.Y])
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x6:
                mRegisters[instruct.X] = instruct.NN;
                break;
            case 0x7:
                mRegisters[instruct.X] = mRegisters[instruct.X] + instruct.NN;
                break;
            
        }
    }

}