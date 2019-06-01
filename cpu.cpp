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

  addr_modes_ = {
    { 0x00 ,[this]{ addr_implied_(); }},
    { 0x01 ,[this]{ addr_izx_(); }},
    { 0x02 ,[this]{ implied_(); }},
    { 0x03 ,[this]{ addr_izx_(); }},
    { 0x04 ,[this]{ addr_zero_page_(); }},
    { 0x05 ,[this]{ addr_zero_page_(); }},
    { 0x06 ,[this]{ addr_zero_page_(); }},
    { 0x07 ,[this]{ addr_zero_page_(); }},
    { 0x08 ,[this]{ addr_implied_(); }},
    { 0x09 ,[this]{ addr_immediate_(); }},
    { 0x0A ,[this]{ addr_implied_(); }},
    { 0x0B ,[this]{ addr_immediate_(); }},
    { 0x0C ,[this]{ implied_(); }},
    { 0x0D ,[this]{ addr_absolute_(); }},
    { 0x0E ,[this]{ addr_absolute_(); }},
    { 0x0F ,[this]{ addr_absolute_(); }},
    { 0x10 ,[this]{ addr_relative_(); }},
    { 0x11 ,[this]{ addr_izy_(); }},
    { 0x12 ,[this]{ implied_(); }},
    { 0x13 ,[this]{ addr_izy_(); }},
    { 0x14 ,[this]{ addr_zero_page_x_(); }},
    { 0x15 ,[this]{ addr_zero_page_x_(); }},
    { 0x16 ,[this]{ addr_zero_page_x_(); }},
    { 0x17 ,[this]{ addr_zero_page_x_(); }},
    { 0x18 ,[this]{ addr_implied_(); }},
    { 0x19 ,[this]{ addr_absolute_y_(); }},
    { 0x1A ,[this]{ implied_(); }},
    { 0x1B ,[this]{ addr_absolute_y_(); }},
    { 0x1C ,[this]{ implied_(); }},
    { 0x1D ,[this]{ addr_absolute_x_(); }},
    { 0x1E ,[this]{ addr_absolute_x_(); }},
    { 0x1F ,[this]{ addr_absolute_x_(); }},
    { 0x20 ,[this]{ addr_absolute_(); }},
    { 0x21 ,[this]{ addr_izx_(); }},
    { 0x22 ,[this]{ implied_(); }},
    { 0x23 ,[this]{ addr_izx_(); }},
    { 0x24 ,[this]{ addr_zero_page_(); }},
    { 0x25 ,[this]{ addr_zero_page_(); }},
    { 0x26 ,[this]{ addr_zero_page_(); }},
    { 0x27 ,[this]{ addr_zero_page_(); }},
    { 0x28 ,[this]{ addr_implied_(); }},
    { 0x29 ,[this]{ addr_immediate_(); }},
    { 0x2A ,[this]{ addr_implied_(); }},
    { 0x2B ,[this]{ addr_immediate_(); }},
    { 0x2C ,[this]{ addr_absolute_(); }},
    { 0x2D ,[this]{ addr_absolute_(); }},
    { 0x2E ,[this]{ addr_absolute_(); }},
    { 0x2F ,[this]{ addr_absolute_(); }},
    { 0x30 ,[this]{ addr_relative_(); }},
    { 0x31 ,[this]{ addr_izy_(); }},
    { 0x32 ,[this]{ implied_(); }},
    { 0x33 ,[this]{ addr_izy_(); }},
    { 0x34 ,[this]{ addr_zero_page_x_(); }},
    { 0x35 ,[this]{ addr_zero_page_x_(); }},
    { 0x36 ,[this]{ addr_zero_page_x_(); }},
    { 0x37 ,[this]{ addr_zero_page_x_(); }},
    { 0x38 ,[this]{ addr_implied_(); }},
    { 0x39 ,[this]{ addr_absolute_y_(); }},
    { 0x3A ,[this]{ implied_(); }},
    { 0x3B ,[this]{ addr_absolute_y_(); }},
    { 0x3C ,[this]{ addr_absolute_x_(); }},
    { 0x3D ,[this]{ addr_absolute_x_(); }},
    { 0x3E ,[this]{ addr_absolute_x_(); }},
    { 0x3F ,[this]{ addr_absolute_x_(); }},
    { 0x40 ,[this]{ addr_implied_(); }},
    { 0x41 ,[this]{ addr_izx_(); }},
    { 0x42 ,[this]{ implied_(); }},
    { 0x43 ,[this]{ addr_izx_(); }},
    { 0x44 ,[this]{ addr_zero_page_(); }},
    { 0x45 ,[this]{ addr_zero_page_(); }},
    { 0x46 ,[this]{ addr_zero_page_(); }},
    { 0x47 ,[this]{ addr_zero_page_(); }},
    { 0x48 ,[this]{ addr_implied_(); }},
    { 0x49 ,[this]{ addr_immediate_(); }},
    { 0x4A ,[this]{ addr_implied_(); }},
    { 0x4B ,[this]{ addr_immediate_(); }},
    { 0x4C ,[this]{ addr_absolute_(); }},
    { 0x4D ,[this]{ addr_absolute_(); }},
    { 0x4E ,[this]{ addr_absolute_(); }},
    { 0x4F ,[this]{ addr_absolute_(); }},
    { 0x50 ,[this]{ addr_relative_(); }},
    { 0x51 ,[this]{ addr_izy_(); }},
    { 0x52 ,[this]{ implied_(); }},
    { 0x53 ,[this]{ addr_izy_(); }},
    { 0x54 ,[this]{ addr_zero_page_x_(); }},
    { 0x55 ,[this]{ addr_zero_page_x_(); }},
    { 0x56 ,[this]{ addr_zero_page_x_(); }},
    { 0x57 ,[this]{ addr_zero_page_x_(); }},
    { 0x58 ,[this]{ addr_implied_(); }},
    { 0x59 ,[this]{ addr_absolute_y_(); }},
    { 0x5A ,[this]{ implied_(); }},
    { 0x5B ,[this]{ addr_absolute_y_(); }},
    { 0x5C ,[this]{ addr_absolute_x_(); }},
    { 0x5D ,[this]{ addr_absolute_x_(); }},
    { 0x5E ,[this]{ addr_absolute_x_(); }},
    { 0x5F ,[this]{ addr_absolute_x_(); }},
    { 0x60 ,[this]{ addr_implied_(); }},
    { 0x61 ,[this]{ addr_izx_(); }},
    { 0x62 ,[this]{ op_NOP(); }},
    { 0x63 ,[this]{ addr_izx_(); }},
    { 0x64 ,[this]{ addr_zero_page_(); }},
    { 0x65 ,[this]{ addr_zero_page_(); }},
    { 0x66 ,[this]{ addr_zero_page_(); }},
    { 0x67 ,[this]{ addr_zero_page_(); }},
    { 0x68 ,[this]{ addr_implied_(); }},
    { 0x69 ,[this]{ addr_immediate_(); }},
    { 0x6A ,[this]{ addr_implied_(); }},
    { 0x6B ,[this]{ addr_immediate_(); }},
    { 0x6C ,[this]{ addr_indirect_(); }},
    { 0x6D ,[this]{ addr_absolute_(); }},
    { 0x6E ,[this]{ addr_absolute_(); }},
    { 0x6F ,[this]{ addr_absolute_(); }},
    { 0x70 ,[this]{ addr_relative_(); }},
    { 0x71 ,[this]{ addr_izy_(); }},
    { 0x72 ,[this]{ implied_(); }},
    { 0x73 ,[this]{ addr_izy_(); }},
    { 0x74 ,[this]{ addr_zero_page_x_(); }},
    { 0x75 ,[this]{ addr_zero_page_x_(); }},
    { 0x76 ,[this]{ addr_zero_page_x_(); }},
    { 0x77 ,[this]{ addr_zero_page_x_(); }},
    { 0x78 ,[this]{ addr_implied_(); }},
    { 0x79 ,[this]{ addr_absolute_y_(); }},
    { 0x7A ,[this]{ implied_(); }},
    { 0x7B ,[this]{ addr_absolute_y_(); }},
    { 0x7C ,[this]{ addr_absolute_x_(); }},
    { 0x7D ,[this]{ addr_absolute_x_(); }},
    { 0x7E ,[this]{ addr_absolute_x_(); }},
    { 0x7F ,[this]{ addr_absolute_x(); }},
    { 0x80 ,[this]{ addr_immediate_(); }},
    { 0x81 ,[this]{ addr_izx_(); }},
    { 0x82 ,[this]{ addr_immediate_(); }},
    { 0x83 ,[this]{ addr_izx_(); }},
    { 0x84 ,[this]{ addr_zero_page_(); }},
    { 0x85 ,[this]{ addr_zero_page_(); }},
    { 0x86 ,[this]{ addr_zero_page_(); }},
    { 0x87 ,[this]{ addr_zero_page_(); }},
    { 0x88 ,[this]{ addr_implied_(); }},
    { 0x89 ,[this]{ addr_immediate_(); }},
    { 0x8A ,[this]{ addr_implied_(); }},
    { 0x8B ,[this]{ addr_immediate_(); }},
    { 0x8C ,[this]{ addr_absolute_(); }},
    { 0x8D ,[this]{ addr_absolute_(); }},
    { 0x8E ,[this]{ addr_absolute_(); }},
    { 0x8F ,[this]{ addr_absolute_(); }},
    { 0x90 ,[this]{ addr_relative_(); }},
    { 0x91 ,[this]{ addr_izy_(); }},
    { 0x92 ,[this]{ implied_(); }},
    { 0x93 ,[this]{ addr_izy_(); }},
    { 0x94 ,[this]{ addr_zero_page_x_(); }},
    { 0x95 ,[this]{ addr_zero_page_x_(); }},
    { 0x96 ,[this]{ addr_zero_page_y_(); }},
    { 0x97 ,[this]{ addr_zero_page_y_(); }},
    { 0x98 ,[this]{ addr_implied_(); }},
    { 0x99 ,[this]{ addr_absolute_y_(); }},
    { 0x9A ,[this]{ addr_implied_(); }},
    { 0x9B ,[this]{ addr_absolute_y_(); }},
    { 0x9C ,[this]{ addr_absolute_x_(); }},
    { 0x9D ,[this]{ addr_absolute_x_(); }},
    { 0x9E ,[this]{ addr_absolute_y_(); }},
    { 0x9F ,[this]{ addr_absolute_y_(); }},
    { 0xA0 ,[this]{ addr_immediate_(); }},
    { 0xA1 ,[this]{ addr_izx_(); }},
    { 0xA2 ,[this]{ addr_immediate_(); }},
    { 0xA3 ,[this]{ addr_izx_(); }},
    { 0xA4 ,[this]{ addr_zero_page_(); }},
    { 0xA5 ,[this]{ addr_zero_page_(); }},
    { 0xA6 ,[this]{ addr_zero_page_(); }},
    { 0xA7 ,[this]{ addr_zero_page_(); }},
    { 0xA8 ,[this]{ addr_implied_(); }},
    { 0xA9 ,[this]{ addr_immediate_(); }},
    { 0xAA ,[this]{ addr_implied_(); }},
    { 0xAB ,[this]{ addr_immediate_(); }},
    { 0xAC ,[this]{ addr_absolute_(); }},
    { 0xAD ,[this]{ addr_absolute_(); }},
    { 0xAE ,[this]{ addr_absolute_(); }},
    { 0xAF ,[this]{ addr_absolute_(); }},
    { 0xB0 ,[this]{ addr_relative_(); }},
    { 0xB1 ,[this]{ addr_izy_(); }},
    { 0xB2 ,[this]{ implied_(); }},
    { 0xB3 ,[this]{ addr_izy_(); }},
    { 0xB4 ,[this]{ addr_zero_page_x_(); }},
    { 0xB5 ,[this]{ addr_zero_page_x_(); }},
    { 0xB6 ,[this]{ addr_zero_page_y_(); }},
    { 0xB7 ,[this]{ addr_zero_page_y_(); }},
    { 0xB8 ,[this]{ addr_implied_(); }},
    { 0xB9 ,[this]{ addr_absolute_y_(); }},
    { 0xBA ,[this]{ addr_implied_(); }},
    { 0xBB ,[this]{ addr_absolute_y_(); }},
    { 0xBC ,[this]{ addr_absolute_x_(); }},
    { 0xBD ,[this]{ addr_absolute_x_(); }},
    { 0xBE ,[this]{ addr_absolute_y_(); }},
    { 0xBF ,[this]{ addr_absolute_y_(); }},
    { 0xC0 ,[this]{ addr_immediate_(); }},
    { 0xC1 ,[this]{ addr_izx_(); }},
    { 0xC2 ,[this]{ implied_(); }},
    { 0xC3 ,[this]{ addr_izx_(); }},
    { 0xC4 ,[this]{ addr_zero_page_(); }},
    { 0xC5 ,[this]{ addr_zero_page_(); }},
    { 0xC6 ,[this]{ addr_zero_page_(); }},
    { 0xC7 ,[this]{ addr_zero_page_(); }},
    { 0xC8 ,[this]{ addr_implied_(); }},
    { 0xC9 ,[this]{ addr_immediate_(); }},
    { 0xCA ,[this]{ addr_implied_(); }},
    { 0xCB ,[this]{ addr_immediate_(); }},
    { 0xCC ,[this]{ addr_absolute_(); }},
    { 0xCD ,[this]{ addr_absolute_(); }},
    { 0xCE ,[this]{ addr_absolute_(); }},
    { 0xCF ,[this]{ addr_absolute_(); }},
    { 0xD0 ,[this]{ addr_relative_(); }},
    { 0xD1 ,[this]{ addr_izy_(); }},
    { 0xD2 ,[this]{ implied_(); }},
    { 0xD3 ,[this]{ addr_izy_(); }},
    { 0xD4 ,[this]{ addr_zero_page_x_(); }},
    { 0xD5 ,[this]{ addr_zero_page_x_(); }},
    { 0xD6 ,[this]{ addr_zero_page_x_(); }},
    { 0xD7 ,[this]{ addr_zero_page_x_(); }},
    { 0xD8 ,[this]{ addr_implied_(); }},
    { 0xD9 ,[this]{ addr_absolute_y_(); }},
    { 0xDA ,[this]{ op_NOP(); }},
    { 0xDB ,[this]{ addr_absolute_y_(); }},
    { 0xDC ,[this]{ addr_absolute_x_(); }},
    { 0xDD ,[this]{ addr_absolute_x_(); }},
    { 0xDE ,[this]{ addr_absolute_x_(); }},
    { 0xDF ,[this]{ addr_absolute_x_(); }},
    { 0xE0 ,[this]{ addr_immediate_(); }},
    { 0xE1 ,[this]{ addr_izx_(); }},
    { 0xE2 ,[this]{ implied_(); }},
    { 0xE3 ,[this]{ addr_izx_(); }},
    { 0xE4 ,[this]{ addr_zero_page_(); }},
    { 0xE5 ,[this]{ addr_zero_page_(); }},
    { 0xE6 ,[this]{ addr_zero_page_(); }},
    { 0xE7 ,[this]{ addr_zero_page_(); }},
    { 0xE8 ,[this]{ addr_implied_(); }},
    { 0xE9 ,[this]{ addr_immediate_(); }},
    { 0xEA ,[this]{ addr_implied_(); }},
    { 0xEB ,[this]{ addr_immediate_(); }},
    { 0xEC ,[this]{ addr_absolute_(); }},
    { 0xED ,[this]{ addr_absolute_(); }},
    { 0xEE ,[this]{ addr_absolute_(); }},
    { 0xEF ,[this]{ addr_absolute_(); }},
    { 0xF0 ,[this]{ addr_relative_(); }},
    { 0xF1 ,[this]{ addr_izy_(); }},
    { 0xF2 ,[this]{ implied_(); }},
    { 0xF3 ,[this]{ addr_izy_(); }},
    { 0xF4 ,[this]{ addr_zero_page_x_(); }},
    { 0xF5 ,[this]{ addr_zero_page_x_(); }},
    { 0xF6 ,[this]{ addr_zero_page_x_(); }},
    { 0xF7 ,[this]{ addr_zero_page_x_(); }},
    { 0xF8 ,[this]{ addr_implied_(); }},
    { 0xF9 ,[this]{ addr_absolute_y_(); }},
    { 0xFA ,[this]{ implied_(); }},
    { 0xFB ,[this]{ addr_absolute_y_(); }},
    { 0xFC ,[this]{ addr_absolute_x_(); }},
    { 0xFD ,[this]{ addr_absolute_x_(); }},
    { 0xFE ,[this]{ addr_absolute_x_(); }},
    { 0xFF ,[this]{ addr_absolute_x_(); }},
  };
  
  // opcode_table_ = {
  //   [this]{ op_00_BRK_(); },     // 0x00
  //   [this]{ op_01_ORA_izx_(); }, // 0x01
  //   [this]{ op_NOP_(0, 2); },    // 0x02
  //   [this]{ op_03_SLO_izx_(); }, // 0x03
  //   [this]{ op_NOP_(3, 2); },    // 0x04
  //   [this]{ op_05_ORA_zp_(); },  // 0x05
  //   [this]{ op_06_ASL_zp_(); },  // 0x06
  //   [this]{ op_07_SLO_zp_(); },  // 0x07
  //   [this]{ op_08_PHP_(); },     // 0x08
  //   [this]{ op_09_ORA_imm_(); }, // 0x09
  //   [this]{ op_0A_ASL_(); },     // 0x0A
  //   [this]{ op_0B_ANC_imm_(); }, // 0x0B
  //   [this]{ op_NOP_(4, 3); },    // 0x0C
  //   [this]{ op_0D_ORA_abs_(); }, // 0x0D
  //   [this]{ op_0E_ASL_abs_(); }, // 0x0E
  //   [this]{ op_0F_SLO_abs_(); }, // 0x0F
  //   [this]{ op_10_BPL_rel_(); }, // 0x10
  //   [this]{ op_11_ORA_izy_(); }, // 0x11
  //   [this]{ op_NOP_(0, 2); },    // 0x12
  //   [this]{ op_13_SLO_izy_(); }, // 0x13
  //   [this]{ op_NOP_(4, 2); },    // 0x14
  //   [this]{ op_15_ORA_zpx_(); }, // 0x15
  //   [this]{ op_16_ASL_zpx_(); }, // 0x16
  //   [this]{ op_17_SLO_zpx_(); }, // 0x17
  //   [this]{ op_18_CLC_(); },     // 0x18
  //   [this]{ op_19_ORA_aby_(); }, // 0x19
  //   [this]{ op_NOP_(0, 2); },    // 0x1A
  //   [this]{ op_1B_SLO_aby_(); }, // 0x1B
  // };
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
  opcode_ = MEM_[PC_ + 1];
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
  uint8_t given_addr = MEM_[PC_] + X_;
  uint16_t zero_page_addr = (MEM_[given_addr + 1] << 8) | (MEM_[given_addr]);
  A_ = A_ | MEM_[zero_page_addr];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;      // Advance PC appropriately
}

void CPU::op_NOP_(uint8_t num_of_cycles, uint8_t pc_offset) {
  // (various opcodes) NOP - Do nothing - - variable cycles
  cycles_to_emulate_ = num_of_cycles;
  PC_ += pc_offset;
}

void CPU::op_03_SLO_izx_() {
  // (0x03) SLO - Shift left then OR - indexed indirect - 8 cycles
  cycles_to_emulate_ = 8;
  // zero_page_addr = (mem[given_addr + 1] << 8) | (mem[given_addr]);
  // zero_page_addr = zero_page_addr << 1;
  // A_ = A_ | zero_page_addr;
  uint8_t given_addr = MEM_[PC_] + X_;
  uint16_t zero_page_addr = (MEM_[given_addr+1] << 8) | (MEM_[given_addr]);
  P_ = P_ | ((MEM_[zero_page_addr] & 0x80) >> 7); // Set carry flag bit 7 of zero_page_addr pre shift
  MEM_[zero_page_addr] = MEM_[zero_page_addr] << 1;
  A_ = A_ | MEM_[zero_page_addr];

  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
 
  PC_ += 2;
}

void CPU::op_05_ORA_zp_() {
  // (0x05) ORA - Logical Inclusive OR - zero page - 3 cycles
  cycles_to_emulate_ = 3;

  // OR A with the value stored in $NN
  // Set status Z to 1 if A_ is 0
  // Set status N to 1 if bit 7 of A_ is 1
  uint8_t given_addr = MEM_[PC_];
  A_ = A_ | MEM_[given_addr];

  // Set flags as needed
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}

void CPU::op_06_ASL_zp_() {
  // (0x06) ASL - Arithmetic Shift Left - zero page - 5 cycles
  cycles_to_emulate_ = 5;
  // SHIFT LEFT one bit addr
  // Set carry flag to the content of bit 7 before shift
  // Set zero flag to 0 if addr = 0 post shift
  // Set negative flag to 1 if bit 7 is 1 post shift
  uint8_t given_addr = MEM_[PC_];
 
  P_ = P_ | ((MEM_[given_addr] & 0x80) >> 7); // Set carry flag to bit 7 of A_ pre shift
  MEM_[given_addr] = MEM_[given_addr] << 1;

  // Set flags as needed
  if (MEM_[given_addr] == 0)
    P_ = P_ | 0x02;
  if (((MEM_[given_addr] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}

void CPU::op_07_SLO_zp_() {
  // (0x07) SLO - Shift left then OR - zero page - 5 cycles
  cycles_to_emulate_ = 5;
  // zero_page_addr = zero_page_addr << 1;
  // A_ = A_ | zero_page_addr;
  uint8_t given_addr = MEM_[PC_];
  uint16_t zero_page_addr = (MEM_[given_addr+1] << 8) | (MEM_[given_addr]);
  P_ = P_ | ((MEM_[zero_page_addr] & 0x80) >> 7); // Set carry flag bit 7 of zero_page_addr pre shift
  MEM_[zero_page_addr] = MEM_[zero_page_addr] << 1;
  A_ = A_ | MEM_[zero_page_addr];

  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
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

void CPU::op_0B_ANC_imm_() {
  // (0x0B) ANC - AND A and immediate - immediate - 2 cycles
  cycles_to_emulate_ = 2;

  // A = A | MEM[PC_]
  // set Flags N, Z, C as needed
  
  // C flag
  P_ = P_ | ((A_ & 0x80) >> 7); // Set carry flag to bit 7 of A_ pre op
  A_ = A_ & MEM_[PC_];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}

void CPU::op_0D_ORA_abs_() {
  // (0x0D) ORA - Logical Inclusive OR - absolute - 4 cycles
  cycles_to_emulate_ = 4;
  // abs means following two bytes are used
  // ... 00 OP LS MS OP
  // ... PC +1 +2 +3 +4
  // A = A | MEM[$NNNN]
  uint16_t target_addr = (MEM_[PC_ + 3] << 8) | (MEM_[PC_+2]);

  A_ = A_ | MEM_[target_addr];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3;
}

void CPU::op_0E_ASL_abs_() {
  // (0x0E) Arithmetic Shift Left - absolute - 6 cycles
  cycles_to_emulate_ = 6;
  // using the following two bits
  // MEM[$NNNN] = MEM[$NNNN} << 1
  // Set carry flag to the content of bit 7 before shift
  // Set zero flag to 0 if A_ = 0 post shift
  // Set negative flag to 1 if bit 7 is 1 post shift
  uint16_t target_addr = (MEM_[PC_ + 3] << 8) | (MEM_[PC_+2]);
  
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                               // A_ pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;

  // Set flags as appropriate
  // Z flag
  if (MEM_[target_addr] == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((MEM_[target_addr] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3;
}

void CPU::op_0F_SLO_abs_() {
  // (0x0F) SLO - Shift left then OR - abs - 6 cycles
  cycles_to_emulate_ = 6;
  // zero_page_addr = zero_page_addr << 1;
  // A_ = A_ | zero_page_addr;
  uint8_t target_addr = (MEM_[PC_+ 3] << 8) | MEM_[PC_+ 2];
  
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                               // zero_page_addr pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;
  A_ = A_ | MEM_[target_addr];

  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
 
  PC_ += 3;
 
}

void CPU::op_10_BPL_rel_() {
  // (0x10) BPL - Branch if Positive - relative - 2 cycles
  cycles_to_emulate_ = 2;
  // If N flag == 0, PC += immediate
  if ((P_ & 0x80) == 0)
    PC_ = PC_ + MEM_[PC_];
  else
    PC_ += 2;
}

void CPU::op_11_ORA_izy_() {
  // (0x11) ORA - Logical Inclusive OR - indirect indexed - 5 cycles
  //
  uint16_t given_addr = (MEM_[PC_+1] << 8) | MEM_[PC_];
  given_addr += Y_;
  A_ = A_ | MEM_[given_addr];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}

void CPU::op_13_SLO_izy_() {
  // (0x13) SLO - Shift left then OR - indirect indexed - 8 cycles
  cycles_to_emulate_ = 8;
  uint16_t target_addr = (MEM_[PC_+1]) | (MEM_[PC_]);
  target_addr += Y_;
   P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7
                                                // of zero_page_addr pre shift 
  MEM_[target_addr] = MEM_[target_addr] << 1;

  A_ = A_ | MEM_[target_addr];

   // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}

void CPU::op_15_ORA_zpx_() {
  // (0x15) ORA - Logical Inclusive OR - zero page, x - 4 cycles
  cycles_to_emulate_ = 4;
  // target_addr = $NN + X
  // A = A or MEM[$target_addr]
  uint16_t target_addr = MEM_[PC_] + X_;

  A_ = A_ | MEM_[target_addr];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}

void CPU::op_16_ASL_zpx_() {
  // (0x16) ASL - Arithmetic Shift Left - zero page, x - 6 cycles
  cycles_to_emulate_ = 6;
  // target_addr = $NN + X
  // MEM[$target_addr] = MEM[$target_addr] << 1
   uint16_t target_addr = MEM_[PC_] + X_;
   P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                                // zero_page_addr pre shift
   MEM_[target_addr] = MEM_[target_addr] << 1;

   // Set flags as appropriate
   // Z flag
   if (MEM_[target_addr] == 0)
     P_ = P_ | 0x02;
   // N flag
   if (((MEM_[target_addr] & 0x80) >> 7) == 1)
     P_ = P_ | 0x80;

   PC_ += 2;
}

void CPU::op_17_SLO_zpx_() {
  // (0x17) SLO - Shift Left then OR - zero page, x - 6 cycles
  cycles_to_emulate_ = 6;
  // $target_addr = $NN + X
  // MEM[$target_addr] = MEM[$target_addr] << 1
  // A = A | MEM[$target_addr]
  uint8_t target_addr = MEM_[PC_] + X_;
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                               // zero_page_addr pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;
  A_ = A_ | MEM_[target_addr];

  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;
}

void CPU::op_18_CLC_() {
  // (0x18) Clear Carry Flag - implied - 2 cycles
  cycles_to_emulate_ = 2;
  // C flag = 0;
  P_ = P_ & 0xFE;

  PC_ += 2;
}

void CPU::op_19_ORA_aby_() {
  // (0x19) Logical Inclusive OR - absolute, y - 4 cycles
  cycles_to_emulate_ = 4;
  uint16_t target_addr = (MEM_[PC_ + 3] << 8) | (MEM_[PC_+2]);
  target_addr = target_addr + Y_;
  A_ = A_ | MEM_[target_addr];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3;
}

void CPU::op_1B_SLO_aby_() {
  // (0x1B) SLO - Shift left then OR - absolute, y - 7 cycles
  cycles_to_emulate_ = 7;
  // target_addr = following two bytes
  // target_addr = target_addr + Y
  // MEM[target_addr] = MEM[target_addr] << 1
  // A = A | MEM[target_addr]
  uint16_t target_addr = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr = target_addr + Y_;
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                               // zero_page_addr pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;
  A_ = A_ | MEM_[target_addr];

  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3;
}

void CPU::op_1D_ORA_abx_() {
  // (0x1D) ORA - Logical Inclusive OR - absolute, x - 4 cycles
  cycles_to_emulate_ = 4;
  // target_addr = following two bytes
  // A = MEM[target_addr + Y]
  uint16_t target_addr = (MEM_[PC_ + 3] << 8) | (MEM_[PC_+2]);
  target_addr = target_addr + X_;
  A_ = A_ | MEM_[target_addr];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3; 
}

void CPU::op_1E_ASL_abx_() {
  // (0x1E) ASL - Arithmetic Shift Left - absolute, x - 7 cycles
  cycles_to_emulate_ = 7;
  // target_addr = following two bytes
  // $target_addr += X
  // MEM[$target_addr] = MEM[$target_addr] << 1;
  uint16_t target_addr = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr = target_addr + X_;
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
  // zero_page_addr pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;
  
  // Set flags as appropriate
  // Z flag
  if (MEM_[target_addr] == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((MEM_[target_addr] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 3; 
}

void CPU::op_1F_SLO_abx_() {
  // (0x1F) Shift left then OR - absolute, x - 7 cycles
  cycles_to_emulate_ = 7;
    // target_addr = following two bytes
  // target_addr = target_addr + X
  // MEM[target_addr] = MEM[target_addr] << 1
  // A = A | MEM[target_addr]
  uint16_t target_addr = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr = target_addr + X_;
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of
                                               // zero_page_addr pre shift
  MEM_[target_addr] = MEM_[target_addr] << 1;
  A_ = A_ | MEM_[target_addr];
  
  // Set flags as appropriate
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 3;
}

void CPU::op_20_JSR_abs_() {
  // (0x20) JSR - Jump to subroutine - absolute - 6 cycles
  cycles_to_emulate_ = 6;
  // pushed the address of return, minus one, onto stack
  // sets PC to target_addr
  uint16_t target_addr = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  
  MEM_[(0x01 << 8)|(SP_++)] = PC_-1;
  
  PC_ = target_addr;
}

void CPU::op_21_AND_izx_() {
  // (0x21) AND - Logical AND - indexed indirect - 6 cycles
    cycles_to_emulate_ = 6;
  // Add register X and $NN (2nd byte) to get a zero page address
  // AND register A with the address found at the zero page memory address
  // if A = 0, set Z flag to 1
  // if bit 7 is 1, set N flag to 1
  // EX: A = A & MEMORY[ (MEMORY[X + $NN + 1] << 8 | MEMORY[X + $NN])]
  uint8_t given_addr = MEM_[PC_] + X_;
  uint16_t zero_page_addr = (MEM_[given_addr + 1] << 8) | (MEM_[given_addr]);
  A_ = A_ & MEM_[zero_page_addr];

  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  PC_ += 2;      // Advance PC appropriately

}

void CPU::op_23_RLA_izx_() {
  // (0x23) RLA - Rotate Left then AND - indexed indirect - 8 cycles
  cycles_to_emulate_ = 8;
  uint8_t given_addr = MEM_[PC_] + X_;
  uint16_t zero_page_addr = (MEM_[given_addr + 1] << 8) | (MEM_[given_addr]);
  uint8_t old_bit_7 = (MEM_[zero_page_addr] & 0x80) >> 7;
  MEM_[zero_page_addr] = (MEM_[zero_page_addr] << 1) | (P_ & 0x01);
  if (old_bit_7 == 1)
    P_ = P_ | 0x01;
  else
    P_ = P_ & 0x01;

  A_ = A_ & MEM_[zero_page_addr];
  
  PC_ += 2;
  
}

void CPU::op_24_BIT_zp_() {
  // (0x24) BIT - Bit Test - zero page - 3 cycles
}

void CPU::op_25_AND_zp_() {
  // (0x25) AND - Logical AND - zero page - 3 cycles
}

void CPU::op_26_ROL_zp_() {
  // (0x26) ROL - Rotate Left - zero page - 5 cycles
}
