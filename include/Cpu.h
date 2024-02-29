#include <vector>
#include <stack>
#include <array>
#include <cstdint>

namespace emulator
{

typedef unsigned char byte;
const int16_t MEM_SIZE = 4096;
const int16_t ROM_OFFSET_START = 0x200;
static const byte DISPLAY_WIDTH = 64;
static const byte DISPLAY_HEIGHT = 32;
typedef std::array<byte, MEM_SIZE> RAM;
typedef std::stack<unsigned short> ProgramStack;
typedef std::array<byte, 16> Registers;
typedef std::array<std::array<bool, DISPLAY_WIDTH>, DISPLAY_HEIGHT> PixelBuffer;

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
    Registers mRegisters;
    ProgramStack mStack;
    Memory mMemoryBuffer;
    PixelBuffer mPixelBuffer = {};
};

}