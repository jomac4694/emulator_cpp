#include <SFML/Graphics.hpp>
#include "Cpu.h"
#include <fstream>
#include <iostream>
using namespace emulator;

std::vector<char> LoadRom(const std::string& file_path)
{

    std::ifstream infile(file_path, std::ios_base::binary);

    return std::vector<char>( std::istreambuf_iterator<char>(infile),
                            std::istreambuf_iterator<char>() );
}


void PrintPixels(Cpu& cpu)
{
    for (int i = 0; i < DISPLAY_HEIGHT; i++)
    {
        for (int j = 0; j < DISPLAY_WIDTH; j++)
        {
            if (cpu.mPixelBuffer[i][j])
            {
                std::cout << "> ";
            }
            else
            {
                std::cout << "# ";
            }
        }
        std::cout << std::endl;
    }
}
int main()
{

    std::vector<char> rom_buffer = LoadRom("C:\\Users\\Joe Mcilvaine\\Documents\\emulator_cpp\\src\\roms\\test_opcode.ch8");
    std::cout << "size=" << rom_buffer.size() << std::endl;

    Cpu cpu;
    for (int i = 0; i < rom_buffer.size(); i++)
    {
        cpu.mMemoryBuffer.mBuffer[ROM_OFFSET_START + i] = (byte) rom_buffer[i];
    }
    std::cout << "size2=" << cpu.mMemoryBuffer.mBuffer.size() << std::endl;
    //for (int i = 0; i < cpu.mMemoryBuffer.mBuffer.size(); i++)
    //    std::cout << "yo=" << i << cpu.mMemoryBuffer.mBuffer[i] << std::endl;
    auto window = sf::RenderWindow{ { emulator::DISPLAY_WIDTH, emulator::DISPLAY_HEIGHT }, "CMake SFML Project" };
    window.setFramerateLimit(60);
    sf::Sprite sprite;
    sprite.setTextureRect(sf::IntRect(5,5,5,5));
    sprite.setColor(sf::Color(0, 255, 0)); // green
    sprite.setPosition(sf::Vector2f(20, 50));




    while (window.isOpen())
    {
        cpu.Decode(cpu.Fetch());
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                PrintPixels(cpu);
                window.close();
            }
        }

        window.clear();
        for (int i = 0; i < DISPLAY_HEIGHT; i++)
        {
            for (int j = 0; j < DISPLAY_WIDTH; j++)
            {
                    sf::CircleShape shape(1);
                    if (cpu.mPixelBuffer[i][j])
                    {
                        
                        shape.setFillColor(sf::Color(100, 250, 50));
                    }
                    else
                    {
                        shape.setFillColor(sf::Color(0, 0, 0));
                    }
                    shape.setPosition(j, i);
                    window.draw(shape);

            }
        }
        
        window.display();
    }
}