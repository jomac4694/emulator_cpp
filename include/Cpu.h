#include <vector>
#include <stack>
#include <array>
#include <cstdint>
#include <chrono>

namespace emulator
{

typedef unsigned char byte;
const int16_t MEM_SIZE = 4096;
const int16_t ROM_OFFSET_START = 0x200;

const byte DISPLAY_SCALE = 1;
static const byte DISPLAY_WIDTH = 64 * DISPLAY_SCALE;
static const byte DISPLAY_HEIGHT = 32 * DISPLAY_SCALE;
typedef std::array<byte, MEM_SIZE> RAM;
typedef std::stack<unsigned short> ProgramStack;
typedef std::array<byte, 16> Registers;
typedef std::array<std::array<bool, DISPLAY_WIDTH * DISPLAY_SCALE>, DISPLAY_HEIGHT * DISPLAY_SCALE> PixelBuffer;
const byte TIMER_SPEED = 60;
const uint16_t TICK_DELAY = 1000 / TIMER_SPEED;


static std::vector<byte> FONT_BUFFER{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

    struct Instruction
    {
        Instruction(uint16_t instruction)
        {
            ins = instruction;
            I = instruction >> 12 & 0xF;
            X = instruction >> 8 & 0xF;
            Y = instruction >> 4 & 0xF;
            N = instruction & 0xF;
            NN = instruction & 0xFF;
            NNN = instruction & 0xFFF;
        }
        
        uint16_t ins;
        uint16_t I;
        uint16_t X;
        uint16_t Y;
        uint16_t N;
        uint16_t NN;
        uint16_t NNN;

    };

struct Memory
{
    Memory()
    {

    }
    RAM mBuffer = {};
    byte ReadByte(uint16_t& address);
    uint16_t ReadNextInstruction(uint16_t& address);
    void WriteByte(uint16_t address, byte val);
};


 

class Cpu
{

public:
    Cpu();
    Instruction Fetch();
    void Decode(Instruction);
    void Execute();
    void Draw(Instruction);
    unsigned short mProgramCounter;
    unsigned short mIndexRegister;
    Registers mRegisters = {};
    ProgramStack mStack;
    Memory mMemoryBuffer;
    PixelBuffer mPixelBuffer = {};
    byte mSoundTimer;
    byte mDelayTimer;
    uint64_t mNextTick;
    int32_t mTickCount{-1};
};

}