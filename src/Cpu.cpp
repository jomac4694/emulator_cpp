#include "Cpu.h"

namespace emulator
{
    
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

    }

    void Cpu::Decode(Instruction instruct)
    {
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
                    for (int i = 0; i < DISPLAY_WIDTH; i++)
                        for (int j = 0; j < DISPLAY_HEIGHT; j++)
                            mPixelBuffer[i][j] = false;
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
            case 0xA:
                mIndexRegister = instruct.NNN;
                break;
            
        }
    }

    void Cpu::Draw(Instruction ins)
    {
        byte draw_x = mRegisters[ins.X] % DISPLAY_WIDTH;
        byte draw_y = mRegisters[ins.Y] % DISPLAY_HEIGHT;
        byte pixels_to_draw = ins.N;
        uint16_t start_offset = mIndexRegister;

        std::vector<byte> sprite_buffer;
        sprite_buffer.reserve(pixels_to_draw);

        for (int i = 0; i < pixels_to_draw; i++)
        {
            sprite_buffer[i] = mMemoryBuffer.ReadByte(start_offset);
        }

        for (auto& sprite : sprite_buffer)
        {
            for (int i = 7; i >= 0; i--)
            {
                bool bit = sprite >> i & 0x1;
                if (draw_x > DISPLAY_WIDTH)
                    break;
                if (bit && mPixelBuffer[draw_x][draw_y])
                {
                    mPixelBuffer[draw_x][draw_y] = false;
                    mRegisters[0xF] = 0x1;
                }
                else if (bit && !mPixelBuffer[draw_x][draw_y])
                {
                    mPixelBuffer[draw_x][draw_y] = true;
                }
                draw_x++;
            }
            draw_y++;
        }
    }

}