/*
 * Author:                  Emanuel Aracena Beriguete
 * Date created:            May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                main.cpp
 * Description:
 */

#include <iostream>

#include "emulator.hpp"

int main(int argc, char** argv)
{
  if (argc < 3) {
    std::cout << "Usage: 6502-emu </path/to/file> <flags>" << std::endl;
    std::cout << "Available flags/options:" << std::endl;
    std::cout << "\t--debug\t\tOutput debug information and run unit tests." << std::endl;
    return 0;
  }

  const std::string filename(argv[1]);
  const std::string flag(argv[2]);
  uint8_t debug_flag = 0;
  
  if (flag == "--debug")
    debug_flag = 1;

  Emulator emulator( filename, debug_flag);

  // Main emulator execution loop
  emulator.loop();
  
  return 0;
}
