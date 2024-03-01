#include "Cpu.h"
#include <iostream>
#include <iomanip>
namespace emulator
{
    
    void PrintInstruction(Instruction ins)
    {
        std::cout << "X: " << (int) ins.X << std::endl;
        std::cout << "Y: " << (int) ins.Y << std::endl;
        std::cout << "N: " << (int) ins.N << std::endl;
        std::cout << "NN: " << (int) ins.NN << std::endl;
        std::cout << "NNN: " << (int) ins.NNN << std::endl;
    }
    byte Memory::ReadByte(uint16_t& address)
    {
        return mBuffer[address++];
    }

    uint16_t Memory::ReadNextInstruction(uint16_t& address)
    {
        uint16_t ret = mBuffer[address++];
        ret = ret << 8;
        uint16_t next = mBuffer[address++];
        return ret | next;

    }

    void Memory::WriteByte(uint16_t address, byte val)
    {
        mBuffer[address] = val;
    }

    Cpu::Cpu()
    {
        mProgramCounter = ROM_OFFSET_START;
    }

    Instruction Cpu::Fetch()
    {
        return Instruction(mMemoryBuffer.ReadNextInstruction(mProgramCounter));
    }

    void Cpu::Decode(Instruction instruct)
    {
        std::cout << "ProgramCounter=" << mProgramCounter << std::endl;
        switch (instruct.I)
        {
            case 0x0:
                
                if (instruct.ins == 0x00EE)
                {
                    std::cout << "Executing instruction 0x00EE" << std::endl;
                    mProgramCounter = mStack.top();
                    mStack.pop();
                }
                else if (instruct.ins = 0x00E0)
                {
                    std::cout << "Executing instruction 0x00E0 CLEAR SCREEN" << std::endl;
                    for (int i = 0; i < DISPLAY_HEIGHT; i++)
                        for (int j = 0; j < DISPLAY_WIDTH; j++)
                            mPixelBuffer[i][j] = false;
                }
                break;
            case 0x1:
                std::cout << "Executing instruction 0x1" << std::endl;
                PrintInstruction(instruct);
                mProgramCounter = instruct.NNN;
                break;
            case 0x2:
                std::cout << "Executing instruction 0x2" << std::endl;
                mStack.push(mProgramCounter);
                mProgramCounter = instruct.NNN;
                break;
            case 0x3:
                std::cout << "Executing instruction 0x3" << std::endl;
                if (mRegisters[instruct.X] == instruct.NN)
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x4:
                std::cout << "Executing instruction 0x4" << std::endl;
                if (mRegisters[instruct.X] != instruct.NN)
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x5:
                std::cout << "Executing instruction 0x5" << std::endl;
                if (mRegisters[instruct.X] == mRegisters[instruct.Y])
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x9:
                std::cout << "Executing instruction 0x9" << std::endl;
                if (mRegisters[instruct.X] != mRegisters[instruct.Y])
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x6:
                std::cout << "Executing instruction 0x6" << std::endl;
                PrintInstruction(instruct);
                mRegisters[instruct.X] = instruct.NN;
                break;
            case 0x7:
                std::cout << "Executing instruction 0x7" << std::endl;
                PrintInstruction(instruct);
                mRegisters[instruct.X] = mRegisters[instruct.X] + instruct.NN;
                break;
            case 0xA:
                std::cout << "Executing instruction 0xA" << std::endl;
                PrintInstruction(instruct);
                mIndexRegister = instruct.NNN;
                break;
            case 0xD:
                std::cout << "Executing instruction 0xD" << std::endl;
                PrintInstruction(instruct);
                Draw(instruct);
                break;
            case 0x8:
                switch (instruct.N)
                {
                    case 0x0:
                        mRegisters[instruct.X] = mRegisters[instruct.Y];
                        break;
                    case 0x1:
                        mRegisters[instruct.X] = mRegisters[instruct.X] | mRegisters[instruct.Y];
                        break;
                    case 0x2:
                        mRegisters[instruct.X] = mRegisters[instruct.X] & mRegisters[instruct.Y];
                        break;
                    case 0x3:
                        mRegisters[instruct.X] = mRegisters[instruct.X] ^ mRegisters[instruct.Y];
                        break;
                    case 0x4:
                    {
                        int val = mRegisters[instruct.X] + mRegisters[instruct.Y];
                        if (val > 255)
                            mRegisters[0xF] = 0x1;
                        else
                            mRegisters[0xF] = 0x0;
                        mRegisters[instruct.X] = mRegisters[instruct.X] + mRegisters[instruct.Y];
                        break;
                    }
                    case 0x5:
                        std::cout << "Executing instruction 0x8XY5" << std::endl;
                        mRegisters[instruct.X] = mRegisters[instruct.X] - mRegisters[instruct.Y];
                        if (mRegisters[instruct.X] > mRegisters[instruct.Y])
                            mRegisters[0xF] = 0x1;
                        else
                            mRegisters[0xF] = 0x0;
                        break;
                    case 0x7:
                        if (mRegisters[instruct.Y] > mRegisters[instruct.X])
                            mRegisters[0xF] = 1;
                        else
                            mRegisters[0xF] = 0;
                        mRegisters[instruct.X] = mRegisters[instruct.Y] - mRegisters[instruct.X];
                        break;

                }
                break;
            case 0xF:
                switch (instruct.NN)
                {
                    case 0x55:
                        uint16_t start = mIndexRegister;
                        for (int i = 0; i <= instruct.X; i++)
                        {
                            mMemoryBuffer.WriteByte(start + i, mRegisters[i]);
                        } 
                        break;

                }
                break;
        }
    }

    void Cpu::Draw(Instruction ins)
    {
        int draw_x = mRegisters[ins.X] % DISPLAY_WIDTH;
        int draw_y = mRegisters[ins.Y] % DISPLAY_HEIGHT;
        int pixels_to_draw = ins.N;
        uint16_t start_offset = mIndexRegister;
        std::cout << "DRAW_X_INITIAL=" << draw_x << std::endl;
        std::cout << "DRAW_Y_INITIAL=" << draw_y << std::endl;
        std::cout << "START_OFFSET=" << start_offset << std::endl;

        std::vector<byte> sprite_buffer;
        std::cout << "to_draw=" << pixels_to_draw << std::endl;
        for (int i = 0; i < pixels_to_draw; i++)
        {   
            sprite_buffer.push_back(mMemoryBuffer.ReadByte(start_offset));
        }

        for (byte sprite : sprite_buffer)
        {
            //std::cout << "sprite=" << (int) sprite << std::endl; 
            int tmp_x = draw_x;
            for (int i = 7; i >= 0; i--)
            {
                bool bit = sprite >> i & 0x1;
                std::cout << bit << " ";
                if (bit)
                    //std::cout << "GOT IT" << std::endl;
                if (tmp_x >= DISPLAY_WIDTH)
                    break;
                if (bit && mPixelBuffer[draw_y][tmp_x])
                {
                 //   std::cout << "Disabling pixel at " << draw_x << ", " << draw_y << std::endl;
                    mPixelBuffer[draw_y][tmp_x] = false;
                    mRegisters[0xF] = 0x1;
                }
                else if (bit && !mPixelBuffer[draw_y][tmp_x])
                {
                    std::cout << "Enabling pixel at " << tmp_x << ", " << draw_y << std::endl;
                    mPixelBuffer[draw_y][tmp_x] = true;
                }
                tmp_x = (tmp_x + 1) % DISPLAY_WIDTH;
                std::cout << "draw_x=" << draw_x << std::endl;
            }
            draw_y++;
            std::cout << std::endl;
            std::cout << "draw_y=" << draw_y << std::endl;
        }
    }

}