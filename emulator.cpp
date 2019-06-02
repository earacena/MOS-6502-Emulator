/* 
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                Emulator.cpp
 * Description:             Emulator implementation.
 */

#include "emulator.hpp"

Emulator::Emulator( const std::string & filename, const uint8_t & debug_flag,
                    const uint8_t & unit_test_flag) {

  // Set flags
  debug_flag_ = debug_flag;
  unit_test_flag_ = unit_test_flag;

  // Initialize components
  //  cpu_.initialize();
  
}

void Emulator::loop() {
  //  cpu_.emulate();
}
