/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                cpu.hpp
 * Description:             CPU interface.
 *                          **The MOS 6502 is a little-endian 8-bit processor.**
 */

#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <vector>
#include <cinttypes>
#include <chrono>
#include <thread>
#include <functional>
#include <unordered_map>
#include <fstream>

class CPU {
  friend class Logger;
  
 public:
  // Initialization
  CPU();
  void initialize();
  void powerup();                               // power-up state
  void load_file(const std::string & filename); // Load file into memory
  
  // CPU emulation
  void emulate();              // single fetch, decode, execute loop

 private:
  uint8_t cpu_running_;
  void emulate_cycle_timing(); // Attempt to accurately emulate cycle timings (513~ ns per CPU cycle)
  void fetch();                // fetch next instruction
  void decode_execute();       // Look for opcode function, execute, update program counter
  
  // Memory, 0000-FFFF map
  // RANGE       |  SIZE  |  Device
  // $0000-$07FF   $0800      2KB internal RAM
  // $0800-$0FFF   $0800    ]
  // $1000-$17FF   $0800    ] Mirriors of $0000-$07FF
  // $1800-$1FFF   $0800    ]
  // $2000-$2007   $0008    NES PPU registers
  // $2008-$3FFF   $1FF8    Mirrors of $2000-$2007
  // $4000-$4017   $0018    NES APU and I/O registers
  // $4018-$401F   $0008    APU and I/O functionality (normally disabled)
  // $4020-$FFFF   $BFE0    Cartridge space: PRG ROM, PRG RAM, and mapper registers
  std::vector<uint8_t> MEM_;
  
  // Registers
  uint8_t  A_;   // Accumalator
  uint8_t  X_;   // Register X
  uint8_t  Y_;   // Register Y
  uint16_t PC_;  // Program counter
  uint8_t  SP_;  // Stack pointer, hardcoded to work in range $0100-01FF
  uint8_t  P_;   // Status register, MSB-LSB : NV(1)BDIZC
                 //    C : Carry
                 //    Z : Zero
                 //    I : Interrupt Disable
                 //    D : Decimal
                 //    B : B flag
                 //    1 : no effect, always one
                 //    V : Overflow
                 //    N : Negative
  

  // Cycle timings
  std::chrono::nanoseconds cycle_time_;
  uint8_t cycles_to_emulate_;
  std::chrono::nanoseconds time_to_emulate_;
  std::chrono::nanoseconds elapsed_exec_time_;
  std::chrono::steady_clock::time_point exec_start_time_;
  std::chrono::steady_clock::time_point exec_end_time_;

  
  
  // Opcode
  uint16_t opcode_;
  std::unordered_map<uint8_t, std::function<void()>> opcode_table_;
  std::string current_addr_mode_;
  uint16_t target_addr_;
  uint8_t pc_offset_;
  uint8_t immediate_;

  // Opcode implementations
  // Addressing modes :
  // Ex: op_OPCODEHEX_OPNAME_POSTFIX_
  //   implied/accumalator -> none
  //   immediate           -> imm
  //   zero page           -> zp
  //   zero page, x        -> zpx
  //   zero page, y        -> zpy
  //   indexed indirect    -> izx
  //   indirect indexed    -> izy
  //   absolute            -> abs
  //   absolute, x         -> abx
  //   absolute, y         -> aby
  //   indirect            -> ind
  //   relative            -> rel
  void addr_implied_();
  void addr_immediate_();
  void addr_zero_page_();
  void addr_zero_page_x_();
  void addr_zero_page_y_();
  void addr_izx_();
  void addr_izy_();
  void addr_absolute_();
  void addr_absolute_x_();
  void addr_absolute_y_();
  void addr_indirect_();
  void addr_relative_();

  // Official opcodes
  void op_ADC_();
  void op_AND_();
  void op_ASL_();
  void op_BCC_();
  void op_BCS_();
  void op_BEQ_();
  void op_BIT_();
  void op_BMI_();
  void op_BNE_();
  void op_BPL_();
  void op_BRK_();
  void op_BVC_();
  void op_BVS_();
  void op_CLC_();
  void op_CLD_();
  void op_CLI_();
  void op_CLV_();
  void op_CMP_();
  void op_CPX_();
  void op_CPY_();
  void op_DEC_();
  void op_DEX_();
  void op_DEY_();
  void op_EOR_();
  void op_INC_();
  void op_INX_();
  void op_INY_();
  void op_JMP_();
  void op_JSR_();
  void op_LDA_();
  void op_LDX_();
  void op_LDY_();
  void op_LSR_();
  void op_NOP_();
  void op_ORA_();
  void op_PHA_();
  void op_PHP_();
  void op_PLA_();
  void op_PLP_();
  void op_ROL_();
  void op_ROR_();
  void op_RTI_();
  void op_RTS_();
  void op_SBC_();
  void op_SEC_();
  void op_SED_();
  void op_SEI_();
  void op_STA_();
  void op_STX_();
  void op_STY_();
  void op_TAX_();
  void op_TAY_();
  void op_TSX_();
  void op_TXA_();
  void op_TXS_();
  void op_TYA_();

  // Unofficial opcodes
  void op_SLO_();
  void op_RLA_();
  void op_SRE_();
  void op_RRA_();
  void op_SAX_();
  void op_LAX_();
  void op_DCP_();
  void op_ISC_();
  void op_ANC_();
  void op_ALR_();
  void op_ARR_();
  void op_XAA_();
  void op_AXS_();
  void op_AHX_();
  void op_SHY_();
  void op_SHX_();
  void op_TAS_();
  void op_LAS_();

  // Special opcodes
  void op_KIL_();

};

#endif // CPU_HPP
