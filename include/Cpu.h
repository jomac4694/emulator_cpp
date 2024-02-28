#include <vector>
#include <stack>


namespace emulator
{

const int16_t MEM_SIZE = 4096;
const int16_t ROM_OFFSET_START = 0x200;

struct Memory
{
    Memory()
    {
        mBuffer.reserve(MEM_SIZE);
    }
    std::vector<unsigned char> mBuffer;
};


typedef std::array<unsigned char, MEM_SIZE> RAM;
typedef std::stack<unsigned short> ProgramStack;
typedef std::array<unsigned char, 16> Registers;

class Cpu
{

public:
    Cpu();
    void Fetch();
    void Decode(uint16_t);
    void Execute();

private:
    unsigned short mProgramCounter;
    unsigned short mIndexRegister;
    Registers mRegisters;
    ProgramStack mStack;
    RAM mMemoryBuffer;
};

}