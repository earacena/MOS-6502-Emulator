/*
 * Author:                  Emanuel Aracena Beriguete
 * Date:                    May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                emulator.hpp
 * Description:             Emulator interface.
 */

#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <cinttypes>
#include <string>


// Component headers
#include "cpu.hpp"
#include "rom.hpp"

// Debug headers
#include "logger.hpp"

#include "gui.hpp"

class Emulator {
public:
  Emulator( const std::string & filename, const uint8_t & debug_flag);

  void loop();  // Main emulation loop

 private:
  // Flags
  uint8_t debug_flag_;
  uint8_t unit_test_flag_;

  // Emulator
  GUI gui_;
  
  // Debug
  Logger logger_;
  
  // Components
  CPU cpu_;
  
};

#endif // EMULATOR_HPP
