/*
 * Author:                  Emanuel Aracena Beriguete
 * Date Created:            June 3, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                logger.cpp
 * Description:             Logger class implementation
 */

#include "logger.hpp"


Logger::Logger() { /* ... */  }

void Logger::show_CPU_emulation_result(const CPU & cpu) {
  std::cout << "\nPC: 0x" << std::hex << std::uppercase << (uint16_t) cpu.PC_ - cpu.pc_offset_;
  std::cout << "  OPCODE:: 0x" << std::hex << std::uppercase << cpu.opcode_;
  std::cout << "\t[addr mode: " << cpu.current_addr_mode_ << " ] "
            << "[target addr: " << cpu.target_addr_ << "] "
            << "[immediate: 0x" << std::hex << std:: uppercase << (uint16_t) cpu.immediate_ << "] ";
  std::cout << "[PC offset: " << (uint16_t) cpu.pc_offset_ << "]";
}

void Logger::save_snapshot_of_ram(const CPU & cpu) {
  std::ofstream output_file("6502_RAM_SNAPSHOT.txt", std::ios::out);

  if(output_file.is_open()) {
    for(uint32_t pos = 0x0000; pos <= 0xFFFF; ++pos) {
      output_file << "POS: 0x" << std::hex << std::uppercase << pos;
      output_file << "\t" << std::hex << std::uppercase << (uint16_t) cpu.MEM_[pos] << std::endl;
      
    }
  }
    
}
