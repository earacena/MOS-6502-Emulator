/*
 * Author:                  Emanuel Aracena
 * Date:                    June 4, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                rom.cpp
 * Description:             ROM implementation.
 *                          
 */

#include "rom.hpp"

ROM::ROM() { /* */ }


void ROM::load_ROM_(const std::string & filename) {
  std::ifstream file(filename, std::ios::binary);

  if (file) {
    file.seekg(0, std::ios::beg);
    file.read(&rom_[0], 50000);
    file.close();
  }
}

void ROM::process_ROM_() {
  // Size of PRG ROM in 16 bit units
  size_of_PRG_ = rom_[4];

  // Size of CHR ROM in 8kb units, 0 means board uses CHR RAM
  size_of_CHR_ = rom_[5];
}

void ROM::verify_ROM_header_() {

  std::cout << "[*] Checking the first 4 header bytes..." << std::endl;
  
  if (rom_[0] != 0x4E )
    invalid_header_err_(0x4E, rom_[0]);
  if (rom_[1] != 0x45 )
    invalid_header_err_(0x45, rom_[1]);
  if (rom_[2] != 0x53 )
    invalid_header_err_(0x53, rom_[2]);
  if (rom_[3] != 0x1A )
    invalid_header_err_(0x1A, rom_[3]);
}

void ROM::invalid_header_err_(const uint8_t & expected, const uint8_t & actual) {
  std::cout << "[-] ERROR: INVALID HEADER ~> "
            << "Expected: 0x" << std::hex << std::uppercase << expected;
  std::cout << " Actual: 0x" << std::hex << std::uppercase << actual << std::endl;
}

