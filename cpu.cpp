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
    [this]{ op_NOP_(0, 2); },    // 0x02
    [this]{ op_03_SLO_izx_(); }, // 0x03
    [this]{ op_NOP_(3, 2); },    // 0x04
    [this]{ op_05_ORA_zp_(); },  // 0x05
    [this]{ op_06_ASL_zp_(); },  // 0x06
    [this]{ op_07_SLO_zp_(); },  // 0x07
    [this]{ op_08_PHP_(); },     // 0x08
    [this]{ op_09_ORA_imm_(); }, // 0x09
    [this]{ op_0A_ASL_(); },     // 0x0A
    [this]{ op_0B_ANC_imm_(); }, // 0x0B
    [this]{ op_NOP_(4, 3); },    // 0x0C
    [this]{ op_0D_ORA_abs_(); }, // 0x0D
    [this]{ op_0E_ASL_abs_(); }, // 0x0E
    [this]{ op_0F_SLO_abs_(); }, // 0x0F
    [this]{ op_10_BPL_rel_(); }, // 0x10
    [this]{ op_11_ORA_izy_(); }, // 0x11
    [this]{ op_NOP_(0, 2); },    // 0x12
    [this]{ op_13_SLO_izy_(); }, // 0x13
    [this]{ op_NOP_(4, 2); },    // 0x14
    [this]{ op_15_ORA_zpx_(); }, // 0x15
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
  uint8_t zero_page_addr_lsb = MEM_[given_addr];
  A_ = A_ | MEM_[zero_page_addr_lsb];

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
  
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of A_ pre shift
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
  
  P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of zero_page_addr pre shift
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
   P_ = P_ | ((MEM_[target_addr] & 0x80) >> 7); // Set carry flag to bit 7 of zero_page_addr pre shift 
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

  uint16_t target_addr = MEM_[PC_] + X_;

  A_ = A_ | MEM_[target_addr];
  
  // Set flags as needed
  if (A_ == 0)
    P_ = P_ | 0x02;
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  PC_ += 2;
}
