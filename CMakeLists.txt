cmake_minimum_required(VERSION 3.22.1)


project(
    Chip8Emulator
    VERSION 0.0.1
    DESCRIPTION "Likely a shitty implmentation of the chip8"
    LANGUAGES CXX)
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(SOURCES 
    src/Cpu.cpp
)

####### library ########
add_library(Chip8Emulator_lib STATIC ${SOURCES})
target_include_directories(Chip8Emulator_lib PUBLIC ${HEADERS})
####### executable ########
include_directories(${HEADERS})
add_executable(Chip8Emulator "src/main.cpp")
#target_include_directories(MacroRecorder PUBLIC ${HEADERS}) is this even needed?
target_link_libraries(Chip8Emulator PRIVATE sfml-graphics Chip8Emulator_lib)
target_compile_features(Chip8Emulator PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET Chip8Emulator
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Chip8Emulator>
        VERBATIM)
endif()

add_subdirectory(tests)
