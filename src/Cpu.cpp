#include "Cpu.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
namespace emulator
{

    uint64_t Now() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
    
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
        uint16_t ret = ReadByte(address);
        ret = ret << 8;
        uint16_t next = ReadByte(address);
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
        if (Now() > mNextTick)
        {
            if (mTickCount % TIMER_SPEED == 0)
            {
                std::cout << "time=" << Now() << std::endl;
            }
            if (mDelayTimer > 0)
                mDelayTimer--;
            if (mSoundTimer > 0)
                mSoundTimer--;
            mNextTick = Now() + TICK_DELAY;
            mTickCount++;
        }
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
                PrintInstruction(instruct);
                mStack.push(mProgramCounter);
                mProgramCounter = instruct.NNN;
                break;
            case 0x3:
                std::cout << "Executing instruction 0x3" << std::endl;
                PrintInstruction(instruct);
                std::cout << "reg[instruct.X]=" << (int) mRegisters[instruct.X] << std::endl;
                if (mRegisters[instruct.X] == instruct.NN)
                {
                    std::cout << "Equals NN" << std::endl;
                    mProgramCounter+=2;
                }
                break;
            case 0x4:
                std::cout << "Executing instruction 0x4" << std::endl;
                PrintInstruction(instruct);
                if (mRegisters[instruct.X] != instruct.NN)
                {
                    mProgramCounter+=2;
                }
                break;
            case 0x5:
                std::cout << "Executing instruction 0x5" << std::endl;
                PrintInstruction(instruct);
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
                        std::cout << "Executing instruction 0x8XY0" << std::endl;
                        mRegisters[instruct.X] = mRegisters[instruct.Y];
                        break;
                    case 0x1:
                        std::cout << "Executing instruction 0x8XY1" << std::endl;
                        mRegisters[instruct.X] = mRegisters[instruct.X] | mRegisters[instruct.Y];
                        break;
                    case 0x2:
                        std::cout << "Executing instruction 0x8XY2" << std::endl;
                        mRegisters[instruct.X] = mRegisters[instruct.X] & mRegisters[instruct.Y];
                        break;
                    case 0x3:
                        std::cout << "Executing instruction 0x8XY3" << std::endl;
                        mRegisters[instruct.X] = mRegisters[instruct.X] ^ mRegisters[instruct.Y];
                        break;
                    case 0x4:
                    {
                        std::cout << "Executing instruction 0x8XY4" << std::endl;
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
                        if (mRegisters[instruct.X] > mRegisters[instruct.Y])
                            mRegisters[0xF] = 0x1;
                        else
                            mRegisters[0xF] = 0x0;
                        mRegisters[instruct.X] = mRegisters[instruct.X] - mRegisters[instruct.Y];

                        break;
                    case 0x6:
                    {
                        std::cout << "Executing instruction 0x8XY6" << std::endl;
                        byte vy = mRegisters[instruct.X];
                        byte shft = vy & 0x1;
                        vy = vy >> 1;
                        mRegisters[instruct.X] = vy;
                        mRegisters[0xF] = shft;
                        break;
                    }
                    case 0xE:
                    {
                        std::cout << "Executing instruction 0x8XYE" << std::endl;
                        byte vy = mRegisters[instruct.X];
                        byte shft = vy >> 7 & 0x1;
                        vy = vy << 1;
                        mRegisters[instruct.X] = vy;
                        mRegisters[0xF] = shft;
                        break;
                    }
                    case 0x7:
                        std::cout << "Executing instruction 0x8XY7" << std::endl;
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
                    {
                        std::cout << "Executing instruction 0xFX55" << std::endl;
                        PrintInstruction(instruct);
                        uint16_t start = mIndexRegister;
                        for (int i = 0; i <= instruct.X; i++)
                        {
                            mMemoryBuffer.WriteByte(start + i, mRegisters[i]);
                        } 
                        break;
                    
                    }
                    case 0x65:
                    {
                        std::cout << "Executing instruction 0xFX55" << std::endl;
                        PrintInstruction(instruct);
                        uint16_t start = mIndexRegister;
                        for (int i = 0; i <= instruct.X; i++)
                        {
                            mRegisters[i] = mMemoryBuffer.ReadByte(start);
                        } 
                        break;
                    }
                    case 0x33:
                    {
                        std::cout << "Executing instruction 0xFX33" << std::endl;
                        PrintInstruction(instruct);
                        uint16_t start = mIndexRegister;
                        std::stack<byte> stk;
                        byte num = mRegisters[instruct.X];
                        std::cout << "num=" << (int) num << std::endl;
                        while (num > 0)
                        {
                            std::cout << "pushing=" << (int) num % 10 << std::endl;
                            stk.push(num % 10);
                            num /= 10;
                        }
                        while (!stk.empty())
                        {
                            byte val = stk.top();
                            std::cout << "popping=" << (int) val << std::endl;
                            std::cout << "start=" << (int) start << std::endl;
                            mMemoryBuffer.WriteByte(start++, val);
                            stk.pop();
                        }
                        break;
                    }

                    case 0x07:
                        mRegisters[instruct.X] = mDelayTimer;
                        break;
                    case 0x15:
                        mDelayTimer = mRegisters[instruct.X];
                        break;
                    case 0x18:
                        mSoundTimer = mRegisters[instruct.X];
                        break;
                    case 0x1E:
                        mIndexRegister += mRegisters[instruct.X];
                        break;
                    // case 0x0A:

                    case 0x29:


                }
                break;
            case 0xC:
            {
                byte rnd = std::rand() % (instruct.NN + 1);
                rnd = rnd & instruct.NN;
                mRegisters[instruct.X] = rnd;
                break;
            }
            case 0xB:
            {
                mProgramCounter = instruct.NNN + mRegisters[0x0];
                break;
            }
            break;
        }
    }

    void Cpu::Draw(Instruction ins)
    {
        int draw_x = (mRegisters[ins.X] * DISPLAY_SCALE) % DISPLAY_WIDTH;
        int draw_y = (mRegisters[ins.Y] * DISPLAY_SCALE) % DISPLAY_HEIGHT;
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
