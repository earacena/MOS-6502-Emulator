/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                emulator.hpp
 * Description:             Emulator interface.
 */

#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <cinttypes>
#include <string>

// Component headers
#include "cpu.hpp"

// Unit Test headers
//#include "TestEmulator.hpp"
#include "catch2/catch.hpp"

class Emulator {
public:
  Emulator( const std::string & ROM_filename,  
            const uint8_t & debug_flag,        // Will set flag to output debug info
            const uint8_t & unit_test_flag);   // Will set flag for running all written unit tests

  void loop();  // Main emulation loop

 private:
  // Flags
  uint8_t debug_flag_;
  uint8_t unit_test_flag_;

  // Components
  CPU cpu_;
  
};

#endif // EMULATOR_HPP
