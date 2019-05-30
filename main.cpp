/*
 * Author:                  Emanuel Aracena
 * Date created:            May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                main.cpp
 * Description:
 */

#include "emulator.hpp"

int main(int argc, char** argv)
{
  // if (argc < 2) {
  //   std::cout << "Usage: nes-emu </path/to/ROM> <flags>" << std::endl;
  //   std::cout << "Available flags/options:" << std::endl;
  //   std::cout << "\t--debug\t\tOutput debug information and run unit tests." << std::endl;
  //   return 0;
  // }

  const std::string ROM_filename(argv[1]);  
  uint8_t debug_flag = 1;
  uint8_t unit_test_flag = 1;

  Emulator emulator( ROM_filename, debug_flag, unit_test_flag );

  // Main emulator execution loop
  emulator.loop();
  
  return 0;
}
