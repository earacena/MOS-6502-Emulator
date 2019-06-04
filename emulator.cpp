/* 
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                Emulator.cpp
 * Description:             Emulator implementation.
 */

#include "emulator.hpp"

Emulator::Emulator( const std::string & filename, const uint8_t & debug_flag) {

  // Set flags
  debug_flag_ = debug_flag;
   
  // "turn on" components
  cpu_.initialize();
  cpu_.powerup();

  // load file into CPU memory
  cpu_.load_file(filename); 
  
  cpu_running_ = 0x01;
}

void Emulator::loop() {
  if (debug_flag_ == 1)
    logger_.save_snapshot_of_ram(cpu_);
  
  while (cpu_running_ == 0x01) {
    // Emulate CPU 
    cpu_.emulate();

    // Debug
    if (debug_flag_ == 1) {
      logger_.show_CPU_emulation_result(cpu_);
      //logger_.show_PPU_emulation_result();
      //logger_.show_APU_emulation_result();
    }
  }
}
