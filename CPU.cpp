/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                CPU.hpp
 * Description:             CPU implementation.
 */

#include "CPU.hpp"

CPU::CPU() {
  
  register_.resize(31, 0);
}
