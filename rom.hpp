/*
 * Author:                  Emanuel Aracena Beriguete
 * Date:                    June 4, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                rom.hpp
 * Description:             ROM interface.
 *                          ROM class processes, stores, and provides easy access to information
 *                          relevant to ROM files loaded onto emulator.
 */

#ifndef ROM_HPP
#define ROM_HPP

#include <iostream>
#include <cinttypes>
#include <string>
#include <fstream>

class ROM {
  // Allow Emulator and CPU objects to access information
  friend class Emulator;
  friend class CPU;
 public:
  ROM();
  
 private:
  void load_ROM_(const std::string & filename);
  void process_ROM_();
  
  void invalid_header_err_(const uint8_t & expected, const uint8_t & actual);
  void verify_ROM_header_(); // Returns 1 if ROM contains proper header, 0 otherwise


  
  char rom_[50000];     //  flag 0-3 is the 16 byte header
  uint8_t size_of_PRG_; //  flag 4
  uint8_t size_of_CHR_; //  flag 5
  uint8_t flag_6_;
  uint8_t flag_7_;
  uint8_t flag_8_;
  uint8_t flag_9_;
  uint8_t flag_10_;
};

#endif // ROM_HPP
