/*
 * Author:                  Emanuel Aracena
 * Date Created:            June 3, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                logger.hpp
 * Description:             Logger interface
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>

#include "cpu.hpp"

class Logger {
 public:
  Logger();
  void show_CPU_emulation_result(const CPU & cpu);
  void save_snapshot_of_ram(const CPU & cpu);
  
  //  void show_PPU_emulation_result();
  //  void show_APU_emulation_result();
};

#endif // LOGGER_HPP
