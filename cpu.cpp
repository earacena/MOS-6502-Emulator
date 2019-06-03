/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                cpu.hpp
 * Description:             CPU implementation.
 */

#include "cpu.hpp"

CPU::CPU() {
  cycle_time_ = std::chrono::nanoseconds(513);
  MEM_.resize(65536, 0);

  addr_modes_ = {
    { 0x00, [this]{ addr_implied_(); op_BRK_();}},     { 0x01 ,[this]{ addr_izx_(); op_ORA_();}},
    { 0x02, [this]{ addr_implied_(); op_KIL_(); }},    { 0x03 ,[this]{ addr_izx_(); op_SLO_();}},
    { 0x04, [this]{ addr_zero_page_(); op_NOP_();}},   { 0x05 ,[this]{ addr_zero_page_(); op_ORA_();}},
    { 0x06, [this]{ addr_zero_page_(); op_ASL_();}},   { 0x07 ,[this]{ addr_zero_page_(); op_SLO_();}},
    { 0x08, [this]{ addr_implied_(); op_PHP_();}},     { 0x09 ,[this]{ addr_immediate_(); op_ORA_();}},
    { 0x0A, [this]{ addr_implied_(); op_ASL_();}},     { 0x0B ,[this]{ addr_immediate_(); op_ANC_();}},
    { 0x0C, [this]{ addr_implied_(); op_NOP_();}},     { 0x0D ,[this]{ addr_absolute_(); op_ORA_();}},
    { 0x0E, [this]{ addr_absolute_(); op_ASL_();}},    { 0x0F ,[this]{ addr_absolute_(); op_SLO_();}},
    { 0x10, [this]{ addr_relative_(); op_BPL_();}},    { 0x11 ,[this]{ addr_izy_(); op_ORA_();}},
    { 0x12, [this]{ addr_implied_(); op_KIL_();}},     { 0x13 ,[this]{ addr_izy_(); op_SLO_();}},
    { 0x14, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0x15, [this]{ addr_zero_page_x_(); op_ORA_();}},
    { 0x16, [this]{ addr_zero_page_x_(); op_ASL_();}}, { 0x17 ,[this]{ addr_zero_page_x_(); op_SLO_();}},
    { 0x18, [this]{ addr_implied_(); op_CLC_();}},     { 0x19 ,[this]{ addr_absolute_y_(); op_ORA_();}},
    { 0x1A, [this]{ addr_implied_(); op_NOP_();}},     { 0x1B ,[this]{ addr_absolute_y_(); op_SLO_();}},
    { 0x1C, [this]{ addr_implied_(); op_NOP_();}},     { 0x1D ,[this]{ addr_absolute_x_(); op_ORA_();}},
    { 0x1E, [this]{ addr_absolute_x_(); op_ASL_();}},  { 0x1F ,[this]{ addr_absolute_x_(); op_SLO_();}},
    { 0x20, [this]{ addr_absolute_(); op_JSR_();}},    { 0x21 ,[this]{ addr_izx_(); op_AND_();}},
    { 0x22, [this]{ addr_implied_(); op_KIL_();}},     { 0x23 ,[this]{ addr_izx_(); op_RLA_();}},
    { 0x24, [this]{ addr_zero_page_(); op_BIT_();}},   { 0x25 ,[this]{ addr_zero_page_(); op_AND_();}},
    { 0x26, [this]{ addr_zero_page_(); op_ROL_();}},   { 0x27 ,[this]{ addr_zero_page_(); op_RLA_();}},
    { 0x28, [this]{ addr_implied_(); op_PLP_();}},     { 0x29 ,[this]{ addr_immediate_(); op_AND_();}},
    { 0x2A, [this]{ addr_implied_(); op_ROL_();}},     { 0x2B ,[this]{ addr_immediate_(); op_ANC_();}},
    { 0x2C, [this]{ addr_absolute_(); op_BIT_();}},    { 0x2D ,[this]{ addr_absolute_(); op_AND_();}},
    { 0x2E, [this]{ addr_absolute_(); op_ROL_();}},    { 0x2F ,[this]{ addr_absolute_(); op_RLA_();}},
    { 0x30, [this]{ addr_relative_(); op_BMI_();}},    { 0x31 ,[this]{ addr_izy_(); op_AND_();}},
    { 0x32, [this]{ addr_implied_(); op_KIL_();}},     { 0x33 ,[this]{ addr_izy_(); op_RLA_();}},
    { 0x34, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0x35 ,[this]{ addr_zero_page_x_(); op_AND_();}},
    { 0x36, [this]{ addr_zero_page_x_(); op_ROL_();}}, { 0x37 ,[this]{ addr_zero_page_x_(); op_RLA_();}},
    { 0x38, [this]{ addr_implied_(); op_SEC_();}},     { 0x39 ,[this]{ addr_absolute_y_(); op_AND_();}},
    { 0x3A, [this]{ addr_implied_(); op_NOP_();}},     { 0x3B ,[this]{ addr_absolute_y_(); op_RLA_();}},
    { 0x3C, [this]{ addr_absolute_x_(); op_NOP();}},   { 0x3D ,[this]{ addr_absolute_x_(); op_AND_();}},
    { 0x3E, [this]{ addr_absolute_x_(); op_ROL_();}},  { 0x3F ,[this]{ addr_absolute_x_(); op_RLA_();}},
    { 0x40, [this]{ addr_implied_(); op_RTI_();}},     { 0x41 ,[this]{ addr_izx_(); op_EOR_(); }},
    { 0x42, [this]{ addr_implied_(); op_KIL_();}},     { 0x43 ,[this]{ addr_izx_(); op_SRE_();}},
    { 0x44, [this]{ addr_zero_page_(); op_NOP_();}},   { 0x45 ,[this]{ addr_zero_page_(); op_EOR_();}},
    { 0x46, [this]{ addr_zero_page_(); op_LSR_();}},   { 0x47 ,[this]{ addr_zero_page_(); op_SRE_();}},
    { 0x48, [this]{ addr_implied_(); op_PHA_();}},     { 0x49 ,[this]{ addr_immediate_(); op_EOR_();}},
    { 0x4A, [this]{ addr_implied_(); op_LSR_();}},     { 0x4B ,[this]{ addr_immediate_(); op_ALR_();}},
    { 0x4C, [this]{ addr_absolute_(); op_JMP_();}},    { 0x4D ,[this]{ addr_absolute_(); op_EOR_();}},
    { 0x4E, [this]{ addr_absolute_(); op_LSR_();}},    { 0x4F ,[this]{ addr_absolute_(); op_SRE_();}},
    { 0x50, [this]{ addr_relative_(); op_BVC_();}},    { 0x51 ,[this]{ addr_izy_(); op_EOR_();}},
    { 0x52, [this]{ addr_implied_(); op_KIL_();}},     { 0x53 ,[this]{ addr_izy_(); op_SRE_();}},
    { 0x54, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0x55 ,[this]{ addr_zero_page_x_(); op_EOR_();}},
    { 0x56, [this]{ addr_zero_page_x_(); op_LSR_();}}, { 0x57 ,[this]{ addr_zero_page_x_(); op_SRE_();}},
    { 0x58, [this]{ addr_implied_(); op_CLI_();}},     { 0x59 ,[this]{ addr_absolute_y_(); op_EOR_();}},
    { 0x5A, [this]{ addr_implied_(); op_NOP_();}},     { 0x5B ,[this]{ addr_absolute_y_(); op_SRE_();}},
    { 0x5C, [this]{ addr_absolute_x_(); op_NOP_();}},  { 0x5D ,[this]{ addr_absolute_x_(); op_EOR_();}},
    { 0x5E, [this]{ addr_absolute_x_(); op_LSR_();}},  { 0x5F ,[this]{ addr_absolute_x_(); op_SRE_();}},
    { 0x60, [this]{ addr_implied_(); op_RTS_();}},     { 0x61 ,[this]{ addr_izx_(); op_ADC_();}},
    { 0x62, [this]{ addr_implied_(); op_KIL_();}},     { 0x63 ,[this]{ addr_izx_(); op_RRA_();}},
    { 0x64, [this]{ addr_zero_page_(); op_NOP_();}},   { 0x65 ,[this]{ addr_zero_page_(); op_ADC_();}},
    { 0x66, [this]{ addr_zero_page_(); op_ROR_();}},   { 0x67 ,[this]{ addr_zero_page_(); op_RRA_();}},
    { 0x68, [this]{ addr_implied_(); op_PLA_();}},     { 0x69 ,[this]{ addr_immediate_(); op_ADC_();}},
    { 0x6A, [this]{ addr_implied_(); op_ROR_();}},     { 0x6B ,[this]{ addr_immediate_(); op_ARR_();}},
    { 0x6C, [this]{ addr_indirect_(); op_JMP_();}},    { 0x6D ,[this]{ addr_absolute_(); op_ADC_();}},
    { 0x6E, [this]{ addr_absolute_(); op_ROR_();}},    { 0x6F ,[this]{ addr_absolute_(); op_RRA_();}},
    { 0x70, [this]{ addr_relative_(); op_BVS_();}},    { 0x71 ,[this]{ addr_izy_(); op_ADC_();}},
    { 0x72, [this]{ addr_implied_(); op_KIL_();}},     { 0x73 ,[this]{ addr_izy_(); op_RRA_();}},
    { 0x74, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0x75 ,[this]{ addr_zero_page_x_(); op_ADC_();}},
    { 0x76, [this]{ addr_zero_page_x_(); op_ROR_();}}, { 0x77 ,[this]{ addr_zero_page_x_(); op_RRA_();}},
    { 0x78, [this]{ addr_implied_(); op_SEI_();}},     { 0x79 ,[this]{ addr_absolute_y_(); op_ADC_();}},
    { 0x7A, [this]{ addr_implied_(); op_NOP_();}},     { 0x7B ,[this]{ addr_absolute_y_(); op_RRA_();}},
    { 0x7C, [this]{ addr_absolute_x_(); op_NOP_();}},  { 0x7D ,[this]{ addr_absolute_x_(); op_ADC_();}},
    { 0x7E, [this]{ addr_absolute_x_(); op_ROR_();}},  { 0x7F ,[this]{ addr_absolute_x_(); op_RRA_();}},
    { 0x80, [this]{ addr_immediate_(); op_NOP_();}},   { 0x81 ,[this]{ addr_izx_(); op_STA_();}},
    { 0x82, [this]{ addr_immediate_(); op_NOP_();}},   { 0x83 ,[this]{ addr_izx_(); op_SAX_();}},
    { 0x84, [this]{ addr_zero_page_(); op_STY_();}},   { 0x85 ,[this]{ addr_zero_page_(); op_STA_();}},
    { 0x86, [this]{ addr_zero_page_(); op_STX_();}},   { 0x87 ,[this]{ addr_zero_page_(); op_SAX_();}},
    { 0x88, [this]{ addr_implied_(); op_DEY_();}},     { 0x89 ,[this]{ addr_immediate_(); op_NOP_();}},
    { 0x8A, [this]{ addr_implied_(); op_TXA_();}},     { 0x8B ,[this]{ addr_immediate_(); op_XAA_();}},
    { 0x8C, [this]{ addr_absolute_(); op_STY_();}},    { 0x8D ,[this]{ addr_absolute_(); op_STA_();}},
    { 0x8E, [this]{ addr_absolute_(); op_STX_();}},    { 0x8F ,[this]{ addr_absolute_(); op_SAX_();}},
    { 0x90, [this]{ addr_relative_(); op_BCC_();}},    { 0x91 ,[this]{ addr_izy_(); op_STA_();}},
    { 0x92, [this]{ addr_implied_(); op_KIL_();}},     { 0x93 ,[this]{ addr_izy_(); op_AHX_();}},
    { 0x94, [this]{ addr_zero_page_x_(); op_STY_();}}, { 0x95 ,[this]{ addr_zero_page_x_(); op_STA_();}},
    { 0x96, [this]{ addr_zero_page_y_(); op_STX_();}}, { 0x97 ,[this]{ addr_zero_page_y_(); op_SAX_();}},
    { 0x98, [this]{ addr_implied_(); op_TYA_();}},     { 0x99 ,[this]{ addr_absolute_y_(); op_STA_();}},
    { 0x9A, [this]{ addr_implied_(); op_TXS_();}},     { 0x9B ,[this]{ addr_absolute_y_(); op_TAS_();}},
    { 0x9C, [this]{ addr_absolute_x_(); op_SHY_();}},  { 0x9D ,[this]{ addr_absolute_x_(); op_STA_();}},
    { 0x9E, [this]{ addr_absolute_y_(); op_SHX_();}},  { 0x9F ,[this]{ addr_absolute_y_(); op_AHX_();}},
    { 0xA0, [this]{ addr_immediate_(); op_LDY_();}},   { 0xA1 ,[this]{ addr_izx_(); op_LDA_();}},
    { 0xA2, [this]{ addr_immediate_(); op_LDX_();}},   { 0xA3 ,[this]{ addr_izx_(); op_LAX_();}},
    { 0xA4, [this]{ addr_zero_page_(); op_LDY_();}},   { 0xA5 ,[this]{ addr_zero_page_(); op_LDA_();}},
    { 0xA6, [this]{ addr_zero_page_(); op_LDX_();}},   { 0xA7 ,[this]{ addr_zero_page_(); op_LAX_();}},
    { 0xA8, [this]{ addr_implied_(); op_TAY_();}},     { 0xA9 ,[this]{ addr_immediate_(); op_LDA_();}},
    { 0xAA, [this]{ addr_implied_(); op_TAX_();}},     { 0xAB ,[this]{ addr_immediate_(); op_LAX_();}},
    { 0xAC, [this]{ addr_absolute_(); op_LDY_();}},    { 0xAD ,[this]{ addr_absolute_(); op_LDA_();}},
    { 0xAE, [this]{ addr_absolute_(); op_LDX_();}},    { 0xAF ,[this]{ addr_absolute_(); op_LAX_();}},
    { 0xB0, [this]{ addr_relative_(); op_BCS_();}},    { 0xB1 ,[this]{ addr_izy_(); op_LDA_();}},
    { 0xB2, [this]{ addr_implied_(); op_KIL_();}},     { 0xB3 ,[this]{ addr_izy_(); op_LAX_();}},
    { 0xB4, [this]{ addr_zero_page_x_(); op_LDY_();}}, { 0xB5 ,[this]{ addr_zero_page_x_(); op_LDA_();}},
    { 0xB6, [this]{ addr_zero_page_y_(); op_LDX_();}}, { 0xB7 ,[this]{ addr_zero_page_y_(); op_LAX_();}},
    { 0xB8, [this]{ addr_implied_(); op_CLV_();}},     { 0xB9 ,[this]{ addr_absolute_y_(); op_LDA_();}},
    { 0xBA, [this]{ addr_implied_(); op_TSX_();}},     { 0xBB ,[this]{ addr_absolute_y_(); op_LAS_();}},
    { 0xBC, [this]{ addr_absolute_x_(); op_LDY_();}},  { 0xBD ,[this]{ addr_absolute_x_(); op_LDA_();}},
    { 0xBE, [this]{ addr_absolute_y_(); op_LDX_();}},  { 0xBF ,[this]{ addr_absolute_y_(); op_LAX_();}},
    { 0xC0, [this]{ addr_immediate_(); op_CPY_();}},   { 0xC1 ,[this]{ addr_izx_(); op_CMP_();}},
    { 0xC2, [this]{ addr_implied_(); op_NOP_();}},     { 0xC3 ,[this]{ addr_izx_(); op_DCP_();}},
    { 0xC4, [this]{ addr_zero_page_(); op_CPY_();}},   { 0xC5 ,[this]{ addr_zero_page_(); op_CMP_();}},
    { 0xC6, [this]{ addr_zero_page_(); op_DEC_();}},   { 0xC7 ,[this]{ addr_zero_page_(); op_DCP_();}},
    { 0xC8, [this]{ addr_implied_(); op_INY_();}},     { 0xC9 ,[this]{ addr_immediate_(); op_CMP_();}},
    { 0xCA, [this]{ addr_implied_(); op_DEX_();}},     { 0xCB ,[this]{ addr_immediate_(); op_AXS_();}},
    { 0xCC, [this]{ addr_absolute_(); op_CPY_();}},    { 0xCD ,[this]{ addr_absolute_(); op_CMP_();}},
    { 0xCE, [this]{ addr_absolute_(); op_DEC_();}},    { 0xCF ,[this]{ addr_absolute_(); op_DCP_();}},
    { 0xD0, [this]{ addr_relative_(); op_BNE_();}},    { 0xD1 ,[this]{ addr_izy_(); op_CMP_();}},
    { 0xD2, [this]{ addr_implied_(); op_KIL_();}},     { 0xD3 ,[this]{ addr_izy_(); op_DCP_();}},
    { 0xD4, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0xD5 ,[this]{ addr_zero_page_x_(); op_CMP_();}},
    { 0xD6, [this]{ addr_zero_page_x_(); op_DEC_();}}, { 0xD7 ,[this]{ addr_zero_page_x_(); op_DCP_();}},
    { 0xD8, [this]{ addr_implied_(); op_CLD_();}},     { 0xD9 ,[this]{ addr_absolute_y_(); op_CMP_();}},
    { 0xDA, [this]{ addr_implied_(); op_NOP_();}},     { 0xDB ,[this]{ addr_absolute_y_(); op_DCP_();}},
    { 0xDC, [this]{ addr_absolute_x_(); op_NOP_();}},  { 0xDD ,[this]{ addr_absolute_x_(); op_CMP_();}},
    { 0xDE, [this]{ addr_absolute_x_(); op_DEC_();}},  { 0xDF ,[this]{ addr_absolute_x_(); op_DCP_();}},
    { 0xE0, [this]{ addr_immediate_(); op_CPX_();}},   { 0xE1 ,[this]{ addr_izx_(); op_SBC_();}},
    { 0xE2, [this]{ addr_implied_(); op_NOP_();}},     { 0xE3 ,[this]{ addr_izx_(); op_ISC_();}},
    { 0xE4, [this]{ addr_zero_page_(); op_CPX_();}},   { 0xE5 ,[this]{ addr_zero_page_(); op_SBC_();}},
    { 0xE6, [this]{ addr_zero_page_(); op_INC_();}},   { 0xE7 ,[this]{ addr_zero_page_(); op_ISC_();}},
    { 0xE8, [this]{ addr_implied_(); op_INX_();}},     { 0xE9 ,[this]{ addr_immediate_(); op_SBC_();}},
    { 0xEA, [this]{ addr_implied_(); op_NOP_();}},     { 0xEB ,[this]{ addr_immediate_(); op_SBC_();}},
    { 0xEC, [this]{ addr_absolute_(); op_CPX_();}},    { 0xED ,[this]{ addr_absolute_(); op_SBC_();}},
    { 0xEE, [this]{ addr_absolute_(); op_INC_();}},    { 0xEF ,[this]{ addr_absolute_(); op_ISC_();}},
    { 0xF0, [this]{ addr_relative_(); op_BEQ_();}},    { 0xF1 ,[this]{ addr_izy_(); op_SBC_();}},
    { 0xF2, [this]{ addr_implied_(); op_KIL_();}},     { 0xF3 ,[this]{ addr_izy_(); op_ISC_();}},
    { 0xF4, [this]{ addr_zero_page_x_(); op_NOP_();}}, { 0xF5 ,[this]{ addr_zero_page_x_(); op_SBC_();}},
    { 0xF6, [this]{ addr_zero_page_x_(); op_INC_();}}, { 0xF7 ,[this]{ addr_zero_page_x_(); op_ISC_();}},
    { 0xF8, [this]{ addr_implied_(); op_SED_();}},     { 0xF9 ,[this]{ addr_absolute_y_(); op_SBC_();}},
    { 0xFA, [this]{ addr_implied_(); op_NOP_();}},     { 0xFB ,[this]{ addr_absolute_y_(); op_ISC_();}},
    { 0xFC, [this]{ addr_absolute_x_(); op_NOP_();}},  { 0xFD ,[this]{ addr_absolute_x_(); op_SBC_();}},
    { 0xFE, [this]{ addr_absolute_x_(); op_INC_();}},  { 0xFF ,[this]{ addr_absolute_x_(); op_ISC_();}},
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
  PC_ += pc_offset_;
  exec_end_time_ = std::chrono::steady_clock::now();
  elapsed_exec_time_ = exec_end_time_ - exec_start_time_;
}


// Addressing modes implementation
void CPU::addr_implied_() {
  //... depends on opcode, synonymous (within the context of this project) to accumulator mode
  current_addr_mode_ = "implied";
}

void CPU::addr_immediate_() {
  current_addr_mode_ = "immediate";
  immediate_ = MEM_[PC_];
}

void CPU::addr_zero_page_() {
  current_addr_mode_ = "zero page";
  target_addr_ = MEM_[ MEM_[PC_] ];
}

void CPU::addr_zero_page_x_() {
  current_addr_mode_ = "zero page x";
  target_addr_ = MEM_[ MEM_[PC_] + X_ ];
}

void CPU::addr_zero_page_y_() {
  current_addr_mode_ = "zero page y";
  target_addr_ = MEM_[ MEM_[PC_] + Y_ ];
}

void CPU::addr_izx_() {
  current_addr_mode_ =  "izx";
  target_addr_ = (MEM_[ MEM_[PC_] + X_ + 1 ] << 8) | (MEM_[ MEM_[PC_] + X_ ]);
}

void CPU::addr_izy_() {
  current_addr_mode_ = "izy";
  target_addr_ = (MEM_[ MEM_[PC_] + 1 ] << 8) | (MEM_[ MEM_[PC_] ]);
  target_addr_ = target_addr_ + Y_;
}

void CPU::addr_absolute_() {
  current_addr_mode_ = "absolute";
  target_addr_ = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
}

void CPU::addr_absolute_x_() {
  current_addr_mode_ = "absolute x";
  target_addr_ = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr_ = target_addr_ + X_;
}

void CPU::addr_absolute_y_() {
  current_addr_mode_ = "absolute y";
  target_addr_ = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr_ = target_addr_ + Y_;
}

void CPU::addr_indirect_() {
  current_addr_mode_ = "indirect";
  target_addr_ = (MEM_[PC_+3] << 8) | (MEM_[PC_+2]);
  target_addr_ = (MEM_[target_addr_+1] << 8) | (MEM_[target_addr_]);
}

void CPU::addr_relative_() {
  current_addr_mode_ = "relative";
  immediate_ = MEM_[PC_];
}



/**
 * Official opcodes implementations
 */
void CPU::op_ADC_() {
  // Add with Carry
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;
  

  uint8_t value_to_add = MEM_[target_addr_];
  uint8_t old_carry_bit = P_ & 0x01;
  uint8_t new_carry_bit = 0x00;
  if ((A_ + value_to_add + old_carry_bit) > 0xFF)
    new_carry_bit = 0x01;

  // Instruction function
  A_ = A_ + value_to_add + old_carry_bit;

  // Set flags
  // C flag
  P_ = (P_ & 0xFE) + new_carry_bit;
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_AND_() {
  // Logical AND
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  A_ = A_ & MEM_[target_addr_];

  // Set flags
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  
  // N Flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_ASL_() {
  // Arithmetic Shift left
  if (current_addr_mode_ == "implied") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 7;


  uint8_t bit_7;
  // Instruction function
  if (current_addr_mode_ == "implied") {
    bit_7 = A_ & 0x80;
    A_ = A_ << 1;
  } else {
    bit_7 = MEM_[target_addr_] & 0x80;
    MEM_[target_addr_] = MEM_[target_addr_] << 1;
  }

  // Set flags as necessary
  // C flag
  P_ = (P_ & 0xFE ) + bit_7;

  // Z flag
  if (current_addr_mode_ == "implied" && A_ == 0)
    P_ = P_ | 0x2;
  else if (MEM_[target_addr_] == 0)
    P_ = P_ | 0x2;

  if ((current_addr_mode_ == "absolute")  ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else if (current_addr_mode_ == "implied")
    pc_offset_ = 1;
  else
    pc_offset_ = 2;
}

void CPU::op_BCC_() {
  // Branch if Carry Clear
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 cycles if succeeds, 4 if to a new page

  // Instruction function
  if ((P_ & 0x01) == 0) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }

}

void CPU::op_BCS_() {
  // Branch if Carry Set
    if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 cycles if succeeds, 4 if to a new page

  // Instruction function
  if ((P_ & 0x01) == 1) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }

}

void CPU::op_BEQ_() {
  // Branch if Equal, (if Z flag is set)
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 cycles if succeeds, 4 if to a new page
  
  // Instruction function
  if (((P_ & 0x02) >> 1) == 1) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }

}

void CPU::op_BIT_() {
  // Bit Test
  if (current_addr_mode_ == "zero page")
    cycles_to_emulate_ = 3; // 3 cycles if succeeds, 4 if to a new page
  else if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 4;

  // Instruction function
  // Z flag
  if ((A_ & MEM_[target_addr_]) == 0)
    P_ = P_ | 0x02;

  //N flag
  if (((P_ & 0x80) >> 7) == 1)
    P_ = P_ & (0xFF - (MEM_[target_addr_] & 0x80));
  else
    P_ = P_ | (MEM_[target_addr_] & 0x80);
  
  //V flag
  if (((P_ & 0x40) >> 6) == 1)
    P_ = P_ & (0xFF - (MEM_[target_addr_] & 0x40));
  else
    P_ = P_ | (MEM_[target_addr_] & 0x40);

  
  if (current_addr_mode_ == "absolute")
    pc_offset_ = 3;
  else
    pc_offset_ = 2;

}

void CPU::op_BMI_() {
  // Branch if Minus
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 if branch succeeds, 4 if to a new page

  // Instruction function
  if (((P_ & 0x80) >> 7) == 1) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }
}

void CPU::op_BNE_() {
  // Branch if Equal, (if Z flag is not set)
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 cycles if succeeds, 4 if to a new page
  
  // Instruction function
  if (((P_ & 0x02) >> 1) == 0) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }
  
}

void CPU::op_BPL_() {
  // Branch if Positive
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 if branch succeeds, 4 if to a new page
  
  // Instruction function
  if (((P_ & 0x80) >> 7) == 0) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }
}

void CPU::op_BRK_() {
  // BRK - Force Interrupt
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 7;

  // Instruction function
  MEM_[0x0100 | SP_++] = PC_;
  MEM_[0x0100 | SP_++] = P_;

  PC_ = MEM_[0xFFFE];   // Interrupt vector IRQ is at 0xFFFE/F
  P_ = P_ | 0x30;       // Set B flag to 1
  pc_offset_ = 1;
}

void CPU::op_BVC_() {
  // Branch if Overflow Clear
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 if succeeds, 4 if to a new page

  // Instruction function
  if (((P_ & 0x40) >> 6) == 0) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }
}

void CPU::op_BVS_() {
  // Branch if Overflow Set
  if (current_addr_mode_ == "relative")
    cycles_to_emulate_ = 2; // 3 if succeeds, 4 if to a new page

  // Instruction function
  if (((P_ & 0x40) >> 6) == 1) {
    PC_ += immediate_;
    pc_offset_ = 0;
  } else {
    pc_offset_ = 2;
  }
}

void CPU::op_CLC_() {
  // Clear Carry Flag
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  P_ = P_ & 0xFE;

  pc_offset_ = 1;
}

void CPU::op_CLD_() {
  // Clear Decimal Mode
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Clear D flag
  P_ = P_ & (0xFF - 0x08);

  pc_offset_ = 1;
}

void CPU::op_CLI_() {
  // Clear Interrupt Disable
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;
  
  // Clear I flag
  P_ = P_ & (0xFF - 0x04);

  pc_offset_ = 1; 
}

void CPU::op_CLV_() {
  // Clear Overflow Flag
    if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Clear V flag
  P_ = P_ & (0xFF - 0x40);

  pc_offset_ = 1;
}

void CPU::op_CMP_() {
  // Compare
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate") {
    if (A_ >= immediate_)
      P_ = P_ | 0x01;
    if (A_ == immediate_)
      P_ = P_ | 0x02;
    if ((((A_ - immediate_) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
  } else {
    if (A_ >= MEM_[target_addr_])
      P_ = P_ | 0x01;
    if (A_ == MEM_[target_addr_])
      P_ = P_ | 0x02;
    if ((((A_ - MEM_[target_addr_]) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;   
  }
    

  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y") )
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_CPX_() {
  // Compare X
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate") {
    if (X_ >= immediate_)
      P_ = P_ | 0x01;
    if (X_ == immediate_)
      P_ = P_ | 0x02;
    if ((((X_ - immediate_) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
  } else {
    if (X_ >= MEM_[target_addr_])
      P_ = P_ | 0x01;
    if (X_ == MEM_[target_addr_])
      P_ = P_ | 0x02;
    if ((((X_ - MEM_[target_addr_]) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;   
  }
    

  if ((current_addr_mode_ == "absolute"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;

}

void CPU::op_CPY_() {
  // Compare Y
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate") {
    if (Y_ >= immediate_)
      P_ = P_ | 0x01;
    if (Y_ == immediate_)
      P_ = P_ | 0x02;
    if ((((Y_ - immediate_) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
  } else {
    if (Y_ >= MEM_[target_addr_])
      P_ = P_ | 0x01;
    if (Y_ == MEM_[target_addr_])
      P_ = P_ | 0x02;
    if ((((Y_ - MEM_[target_addr_]) & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;   
  }
    
  if ((current_addr_mode_ == "absolute"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_DEC_() {
  // Decrement Memory
  if (current_addr_mode_ == "zero page")
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute x")
    cycles_to_emulate_ = 7;

  // Instruction function

  MEM_[target_addr_] = MEM_[target_addr_] - 1;
  
  // Set flags
  // N flag
  if (((MEM_[target_addr_] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (MEM_[target_addr_] == 0)
    P_ = P_ | 0x02;

  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_DEX_() {
  // Decrement X register
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  X_ = X_ - 1;
  
  // Set flags
  // N flag
  if (((X_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (X_ == 0)
    P_ = P_ | 0x02;

  if (current_addr_mode_ == "implied")
    pc_offset_ = 2;
}

void CPU::op_DEY_() {
  // Decrement Y register
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  Y_ = Y_ - 1;
  
  // Set flags
  // N flag
  if (((Y_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (Y_ == 0)
    P_ = P_ | 0x02;

  if (current_addr_mode_ == "implied")
    pc_offset_ = 2;
}

void CPU::op_EOR_() {
  // Exclusive OR
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  A_ = A_ ^ MEM_[target_addr_];

  // Set flags
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  
  // N Flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_INC_() {
 // Decrement Memory
  if (current_addr_mode_ == "zero page")
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute x")
    cycles_to_emulate_ = 7;

  // Instruction function
  MEM_[target_addr_] = MEM_[target_addr_] + 1;
  
  // Set flags
  // N flag
  if (((MEM_[target_addr_] & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (MEM_[target_addr_] == 0)
    P_ = P_ | 0x02;

  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_INX_() {
  // Increment X register
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  X_ = X_ + 1;
  
  // Set flags
  // N flag
  if (((X_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (X_ == 0)
    P_ = P_ | 0x02;

  if (current_addr_mode_ == "implied")
    pc_offset_ = 2;
}

void CPU::op_INY_() {
  // Increment Y register
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  Y_ = Y_ - 1;
  
  // Set flags
  // N flag
  if (((Y_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // Z flag
  if (Y_ == 0)
    P_ = P_ | 0x02;

  if (current_addr_mode_ == "implied")
    pc_offset_ = 2;
}

void CPU::op_JMP_() {
  // Jump
  if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "indirect")
    cycles_to_emulate_ = 5;

  // Instruction function
  if  (current_addr_mode_ == "absolute") 
    PC_ = target_addr_;
  else if (current_addr_mode_ == "indirect") 
    PC_ = target_addr_;
  
  pc_offset_ = 0;
}

void CPU::op_JSR_() {
  // Jump to subroutine
  if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 6;

  MEM_[0x0100 | SP_++] = PC_ + 2;   // return address minus 1, 3 byte offset

  // 
  PC_ = target_addr_;

  if (current_addr_mode_ == "absolute")
    pc_offset_ = 0;
}

void CPU::op_LDA_() {
  // Load accumalator
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate")
    A_ = immediate_;
  else
    A_ = MEM_[target_addr_];

  // Set flags
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_LDX_() {
  // Load X register
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page y")
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate")
    X_ = immediate_;
  else
    X_ = MEM_[target_addr_];

  // Set flags
  // Z flag
  if (X_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((X_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_LDY_() {
  // Load Y register
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;

  // Instruction function
  if (current_addr_mode_ == "immediate")
    Y_ = immediate_;
  else
    Y_ = MEM_[target_addr_];

  // Set flags
  // Z flag
  if (Y_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((Y_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_LSR_() {
  // Logical Shift Right
  if (current_addr_mode_ == "implied") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 7;

  uint8_t carry_bit = 0;
  
  // Instruction function
  if (current_addr_mode_ == "implied") {
    carry_bit = (A_ & 0x80) >> 7;
    A_ = A_ << 1;
    // Set flags
    if (A_ == 0)
      P_ = P_ | 0x02;
    if (((A_ & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
  } else {
    carry_bit = (MEM_[target_addr_] & 0x80) >> 7;
    MEM_[target_addr_] = MEM_[target_addr_] << 1;

    // Set flags
    if (MEM_[target_addr_] == 0)
      P_ = P_ | 0x02;
    if (((MEM_[target_addr_] & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
  }

  // C flag, set to old contents
  P_ = (P_ & 0xFE) + carry_bit;
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_NOP_() {
  // NOP, do nothing. Since some NOP instructions vary in
  // cycles used, this serves as a way to delay the CPU
  // or progress the PC
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  pc_offset_ = 1;
}

void CPU::op_ORA_() {
  // Logical inclusive OR
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;

  // Instruction function
  A_ = A_ | MEM_[target_addr_];

  // Set flags
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  
  // N Flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;

  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_PHA_() {
  // Push Accumulator
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 3;

  MEM_[0x0100 | SP_++] = A_;
  
  pc_offset_ = 1;
}

void CPU::op_PHP_() {
  // Push processor status
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 3;

  MEM_[0x0100 | SP_++] = P_;
  
  pc_offset_ = 1;
}

void CPU::op_PLA_() {
  // Pull Accumulator
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 4;

  MEM_[0x0100 | SP_--] = A_;

  // Set flags
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  pc_offset_ = 1;
}

void CPU::op_PLP_() {
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 4;

  MEM_[0x0100 | SP_--] = P_;
  
  pc_offset_ = 1;
}

void CPU::op_ROL_() {
  // Rotate Left
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;
  else if (current_addr_mode_ == "zero page")
    cycles_to_emulate_ = 5; 
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute x")
    cycles_to_emulate_ = 7;

  // Instruction function
  uint8_t new_carry_bit;
  uint8_t old_carry_bit = (P_ & 0x80) >> 7;
  if (current_addr_mode_ == "implied") {
    new_carry_bit = (A_ & 0x01);
    A_ = (A_ << 1) + old_carry_bit;

    // Z flag
    if (A_ == 0)
      P_ = P_ | 0x02;
    // N flag
    if (((A_ & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
    
  } else {
    new_carry_bit = (MEM_[target_addr_] & 0x01);
    MEM_[target_addr_] = (MEM_[target_addr_] << 1) + old_carry_bit;

    // Z flag
    if (MEM_[target_addr_] == 0)
      P_ = P_ | 0x02;
    // N flag
    if (((MEM_[target_addr_] & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
    
  }
    
  // C flag
  P_ = (P_ & 0xFE) + new_carry_bit;
  
  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_ROR_() {
 // Rotate Right
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;
  else if (current_addr_mode_ == "zero page")
    cycles_to_emulate_ = 5; 
  else if (current_addr_mode_ == "zero page x")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute")
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute x")
    cycles_to_emulate_ = 7;

  // Instruction function
  uint8_t new_carry_bit;
  uint8_t old_carry_bit = (P_ & 0x80) >> 7;
  if (current_addr_mode_ == "implied") {
    new_carry_bit = (A_ & 0x01);
    A_ = (A_ >> 1) + (old_carry_bit << 7);

    // Z flag
    if (A_ == 0)
      P_ = P_ | 0x02;
    // N flag
    if (((A_ & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
    
  } else {
    new_carry_bit = (MEM_[target_addr_] & 0x01);
    MEM_[target_addr_] = (MEM_[target_addr_] >> 1) + (old_carry_bit << 7);

    // Z flag
    if (MEM_[target_addr_] == 0)
      P_ = P_ | 0x02;
    // N flag
    if (((MEM_[target_addr_] & 0x80) >> 7) == 1)
      P_ = P_ | 0x80;
    
  }
    
  // C flag
  P_ = (P_ & 0xFE) + new_carry_bit;
  
  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_RTI_() {
  // Return from Interrupt
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 6;

  P_ = MEM_[0x0100 | SP_--];
  PC_ = MEM_[0x0100 | SP_--];

  pc_offset_ = 1;
}

void CPU::op_RTS_() {
  // Return from Subroutine
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 6;

  PC_ = MEM_[0x0100 | SP_--];
  
  pc_offset_ = 1;
}

void CPU::op_SBC_() {
  // Subtract with Carry
  if (current_addr_mode_ == "immediate") 
    cycles_to_emulate_ = 2;   
  else if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 4;
  

  uint8_t value_to_add = MEM_[target_addr_];
  uint8_t old_carry_bit = P_ & 0x01;
  uint8_t new_carry_bit = 0x00;
  if ((A_ - value_to_add - (1 - old_carry_bit)) > 0xFF)
    new_carry_bit = 0x01;
  
  // Instruction function
  A_ = A_ - value_to_add - (1 - old_carry_bit);

  // Set flags
  // C flag
  P_ = (P_ & 0xFE) + new_carry_bit;
  
  // Z flag
  if (A_ == 0)
    P_ = P_ | 0x02;
  
  // N flag
  if (((A_ & 0x80) >> 7) == 1)
    P_ = P_ | 0x80;
  
  // V flags
  P_ = (P_ & (0xFF -0x40)) + (new_carry_bit << 6);
  
  // PC increment
  if ((current_addr_mode_ == "absolute")   ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else 
    pc_offset_ = 2;
}

void CPU::op_SEC_() {
  // Set Carry Flag
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  P_ = P_ | 0x01;
  
  pc_offset_ = 1;
}

void CPU::op_SED_() {
  // Set decimal flag
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  P_ = P_ | 0x08;
  
  pc_offset_ = 1;
}

void CPU::op_SEI_() {
  // Set Interrupt disable
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  P_ = P_ | 0x04;
  
  pc_offset_ = 1; 
}

void CPU::op_STA_() {
  // Store accumulator
  if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page x") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "izx") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "izy") 
    cycles_to_emulate_ = 6;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute x") 
    cycles_to_emulate_ = 5;
  else if (current_addr_mode_ == "absolute y") 
    cycles_to_emulate_ = 5;

  // Instruction main function
  MEM_[target_addr_] = A_;

  if ((current_addr_mode_ == "absolute") ||
      (current_addr_mode_ == "absolute x") ||
      (current_addr_mode_ == "absolute y"))
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_STX_() {
  // Store X register
  if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page y") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;

  // Instruction main function
  MEM_[target_addr_] = X_;

  if (current_addr_mode_ == "absolute")
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_STY_() {
  // Store Y register
  if (current_addr_mode_ == "zero page") 
    cycles_to_emulate_ = 3;
  else if (current_addr_mode_ == "zero page y") 
    cycles_to_emulate_ = 4;
  else if (current_addr_mode_ == "absolute") 
    cycles_to_emulate_ = 4;

  // Instruction main function
  MEM_[target_addr_] = Y_;

  if (current_addr_mode_ == "absolute")
    pc_offset_ = 3;
  else
    pc_offset_ = 2;
}

void CPU::op_TAX_() {
  // Transfer accumulator to X
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  X_ = A_;

  // Set flags
  // Z flags
  if (X_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((X_ & 0x80) >>7) == 1)
    P_ = P_ | 0x80;
  
  pc_offset_ = 1;
}

void CPU::op_TAY_() {
  // Transfer accumulator to Y
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  Y_ = A_;
  // Set flags
  // Z flags
  if (Y_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((Y_ & 0x80) >>7) == 1)
    P_ = P_ | 0x80;
   
  pc_offset_ = 1;
}

void CPU::op_TSX_() {
  // Transfer stack pointer to X
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  X_ = SP_;
  // Set flags
  // Z flags
  if (X_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((X_ & 0x80) >>7) == 1)
    P_ = P_ | 0x80;
  
  pc_offset_ = 1;
}

void CPU::op_TXA_() {
  // Transfer X to Accumulator
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  A_ = X_;
  
  // Set flags
  // Z flags
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >>7) == 1)
    P_ = P_ | 0x80;
  
  pc_offset_ = 1;
}

void CPU::op_TXS_() {
  // Transfer X to Stack pointer
   if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  SP_ = X_;
 
  pc_offset_ = 1; 
}

void CPU::op_TYA_(){
  // Transfer Y to Accumulator
  if (current_addr_mode_ == "implied")
    cycles_to_emulate_ = 2;

  // Instruction function
  A_ = Y_;
  
  // Set flags
  // Z flags
  if (A_ == 0)
    P_ = P_ | 0x02;
  // N flag
  if (((A_ & 0x80) >>7) == 1)
    P_ = P_ | 0x80;
  
  pc_offset_ = 1;
}

// Unofficial opcodes
void CPU::op_SLO_(){
  op_ASL_();
  op_ORA_();

}

void CPU::op_RLA_(){

  op_ROL_();
  op_AND_();
  
}

void CPU::op_SRE_(){
  op_ROR_();
  op_EOR_();
}

void CPU::op_RRA_(){
  op_ROR_();
  op_AND_();
}

void CPU::op_SAX_(){
  // mem[target_addr_] = A_ & X_
}

void CPU::op_LAX_(){
  // A,X = mem[target_addr_]

}

void CPU::op_DCP_(){
  // mem[target_addr_] = mem[target_addr_] - 1
  // A = A - mem[target_addr_]
}

void CPU::op_ISC_(){
  // mem[target_addr_] = mem[target_addr_] + 1
  // A = A - mem[target_addr_]
}

void CPU::op_ANC_(){
  // A = A & immediate
}

void CPU::op_ALR_(){
  // A = (A & immediate) >> 1
}

void CPU::op_ARR_(){
  // A = (A & immediate) >> 1
  // difference between ARR and ALR is the flags set
}


void CPU::op_XAA_(){
  // A = X & immediate
}

void CPU::op_AXS_(){
  // A = (A & X) - immediate
}

void CPU::op_AHX_(){
  // mem[target_addr_] = A & X & H(?)
}

void CPU::op_SHY_(){
  // mem[target_addr_] = Y & H(?)
}

void CPU::op_SHX_(){
  // mem[target_addr_] = X & H
}

void CPU::op_TAS_(){
  // SP = A & X
  // mem[target_addr_] = SP & H(?)
}

void CPU::op_LAS_(){
  // A,X,SP = mem[target_addr_] & SP
}

// Special opcodes
void CPU::op_KIL_() {
  // Jam the machine, will stop the machines execution loop
  // running = false;
}

