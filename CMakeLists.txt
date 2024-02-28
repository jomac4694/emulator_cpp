cmake_minimum_required(VERSION 3.22.1)


project(
    Chip8Emulator
    VERSION 0.0.1
    DESCRIPTION "Likely a shitty implmentation of the chip8"
    LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(SOURCES 
    src/main.cpp 
)

include_directories(${HEADERS})
add_executable(Chip8Emulator ${SOURCES})
#target_include_directories(MacroRecorder PUBLIC ${HEADERS}) is this even needed?
target_link_libraries(Chip8Emulator PRIVATE sfml-graphics)
target_compile_features(Chip8Emulator PRIVATE cxx_std_20)