/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                cpu.hpp
 * Description:             CPU implementation.
 */

#include "cpu.hpp"

CPU::CPU() {
  cycle_time_ = std::chrono::nanoseconds(513);
  MEM_.resize(65536, 0);
  
  opcode_table_ = {
    [this]{ op_00_BRK_(); },     // 0x00
    [this]{ op_01_ORA_izx_(); }, // 0x01
    [this]{ op_NOP_(); },        // 0x02
    [this]{ op_NOP_(); },        // 0x03
    [this]{ op_NOP_(); },        // 0x04
    [this]{ op_05_ORA_zp_(); },  // 0x05
    [this]{ op_06_ASL_zp_(); },  // 0x06
    [this]{ op_NOP_(); },        // 0x07
    [this]{ op_08_PHP_(); }      // 0x08
    
  };
}

void CPU::powerup() {
  // Clear memory since it cannot be relied on
  for (uint32_t position = static_cast<uint32_t>(0x0000); position <= 0xFFFF; ++position)
    MEM_[position] = 0x00;

  P_  = 0x34;
  A_  = 0x0;
  X_  = 0x0;
  Y_  = 0x0;
  SP_ = 0xFD;
  PC_ = 0x4020;
  // LFSR = 0x0000
}

void CPU::emulate() {
  fetch();
  decode_execute();
  emulate_cycle_timing();
}

void CPU::emulate_cycle_timing() {
  if (elapsed_exec_time_ < time_to_emulate_) {
    time_to_emulate_ = (cycles_to_emulate_ * cycle_time_) - elapsed_exec_time_;
    std::this_thread::sleep_for(time_to_emulate_);
  }
}

void CPU::fetch() {
  opcode_ = MEM_[PC_+1];
}

void CPU::decode_execute() {
  exec_start_time_ = std::chrono::steady_clock::now();
  opcode_table_[opcode_]();
  exec_end_time_ = std::chrono::steady_clock::now();
  elapsed_exec_time_ = exec_end_time_ - exec_start_time_;
}


// Opcode implementations

void CPU::op_00_BRK_() {
  // (0x00) BRK - Force Interrupt - 7 cycles
  cycles_to_emulate_ = 7;
  // program counter and processor status pushed onto stack
  // IRQ interrupt vector at $FFFE/F is loaded into the PC and
  // break flag in status register is set to one.
  MEM_[ (0x01 << 8) | SP_] = (PC_ & 0xFF00) >> 8;
  MEM_[ (0x01 << 8) | ++SP_] = PC_ & 0x00FF;
  MEM_[ (0x01 << 8) | ++SP_] = P_;
  // Prepare stack register for next instruction
  SP_ += 1;
  
  PC_ = MEM_[0xFFFE]; // Swap 0xFFFE to 0xFFFF if error
  P_ = P_ | 0x30; // Set bit 5 and 4 to 1 for BRK instruction
}

void CPU::op_01_ORA_izx_() {
  // (0x01) ORA - Logical Inclusive OR - indexed indirect - 6 cycles
  cycles_to_emulate_ = 6;
  // Add register X and $NN (2nd byte) to get a zero page address
  // OR register A with the address found at the zero page memory address
  // if A = 0, set Z flag to 1
  // if bit 7 is 1, set N flag to 1
  // EX: A = A | MEMORY[ (MEMORY[X + $NN + 1] << 8 | MEMORY[X + $NN])]
  uint8_t given_addr = MEM_[PC_];
  uint16_t zero_page_addr = (MEM_[X_ + given_addr + 1] << 8) | (MEM_[X_ + given_addr]);
  A_ = A_ | MEM_[zero_page_addr];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;      // Advance PC appropriately
}

void CPU::op_NOP_() {
  // (various opcodes) NOP - Do nothing - - 2 cycles
  cycles_to_emulate_ = 2;
  PC_ += 2;
}

void CPU::op_05_ORA_zp_() {
  // (0x05) ORA - Logical Inclusive OR - zero page - 3 cycles
  cycles_to_emulate_ = 3;

  // OR A with the value stored in $NN
  // Set status Z to 1 if A_ is 0
  // Set status N to 1 if bit 7 of A_ is 1
  A_ = A_ | MEM_[PC_];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}

void CPU::op_06_ASL_zp_() {
  // (0x06) ASL - Arithmetic Shift Left - zero page - 5 cycles
  cycles_to_emulate_ = 5;
  // SHIFT LEFT one bit A_
  // Set carry flag to the content of bit 7 before shift
  // Set zero flag to 0 if A_ = 0 post shift
  // Set negative flag to 1 if bit 7 is 1 post shift

  P_ = P_ | ((MEM_[PC_] & 0x80) >> 7); // Set carry flag to bit 7 of A_ pre shift
  MEM_[PC_] = MEM_[PC_] << 1;

  // Set flags as needed
  if (MEM_[PC_] == 0)
    P_ = P_ | 0x02;
  if (((MEM_[PC_] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}

void CPU::op_08_PHP_() {
  // (0x08) PHP - Push Processor Status - - 3 cycles
  cycles_to_emulate_ = 3;

  // Push a copy of the status flags on to the stack
  MEM_[(0x01 << 8) | SP_] = P_;
  // Prepare stack register for next instruction
  SP_ += 1;

  PC_ += 2;
}

void CPU::op_09_ORA_imm_() {
  // (0x09) ORA - Logical Inclusive OR - immediate - 2 cycles
  cycles_to_emulate_ = 2;
  // OR A_ and the immediate given
  A_ = A_ | MEM_[PC_];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}

void CPU::op_0A_ASL_() {
  // (0x0A) ASL - Arithmetic Shift Left - - 2 cycles
  cycles_to_emulate_ = 5;
  // SHIFT LEFT one bit A_
  // Set carry flag to the content of bit 7 before shift
  // Set zero flag to 0 if A_ = 0 post shift
  // Set negative flag to 1 if bit 7 is 1 post shift

  P_ = P_ | ((A_ & 0x80) >> 7); // Set carry flag to bit 7 of A_ pre shift
  A_ = A_ << 1;

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}
