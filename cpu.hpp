/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                cpu.hpp
 * Description:             CPU interface.
 *                          **The MOS 6502 is a little-endian 8-bit processor.**
 */

#ifndef CPU_HPP
#define CPU_HPP

#include <vector>
#include <cinttypes>
#include <chrono>
#include <thread>
#include <functional>
#include <unordered_map>

class CPU {
 public:
  // Initialization
  CPU();
  void powerup();              // power-up state

  // CPU emulation
  void emulate();              // single fetch, decode, execute loop

 private:
  void emulate_cycle_timing(); // Attempt to accurately emulate cycle timings (513~ ns per CPU cycle)
  void fetch();                // fetch next instruction
  void decode_execute();       // Look for opcode function, execute, update program counter
  
  // Memory, 0000-FFFF map
  // RANGE       |  SIZE  |  Device
  // $0000-$07FF   $0800    2KB internal RAM
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
  uint8_t  P_;   // Status register, MSB-LSB : NVssDIZC
                 //    C : Carry
                 //    Z : Zero
                 //    I : Interrupt Disable
                 //    D : Decimal
                 //    s : no CPU effect, B flag
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

  // Opcode table, implemented in sequential order for simplicity of
  // function lookup with byte hex offset,
  // 256 total (official) opcode entries
  //std::vector<std::function<void()>> opcode_table_;

  std::unordered_map<uint8_t, std::function<void()>> addr_modes_;
  std::unordered_map<uint8_t, std::function<void()>> opcode_table_;
  std::string current_addr_mode_;
  uint16_t target_address_;
  
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
  void addr_ixy_();
  void addr_absolute_();
  void addr_absolute_x_();
  void addr_absolute_y_();
  void addr_indirect_();
  void addr_relative_();




  
  // void op_00_BRK_();                   // 00 - BRK - none - Force Interrupt
  // void op_01_ORA_izx_();               // 01 - ORA - izx  - Logical Inclusive OR
  // void op_NOP_(uint8_t num_of_cycles,
  //              uint8_t pc_offset);     // ~  - NOP - none - Do nothing
  // void op_03_SLO_izx_();               // 03 - SLO - izx  - Shift Left then OR
  // void op_05_ORA_zp_();                // 05 - ORA - zp   - Logical Inclusive OR
  // void op_06_ASL_zp_();                // 06 - ASL - zp   - Arithmetic Shift Left
  // void op_07_SLO_zp_();                // 07 - SLO - zp   - Shift Left then OR
  // void op_08_PHP_();                   // 08 - PHP - none - Push Processor Status
  // void op_09_ORA_imm_();               // 09 - ORA - imm  - Logical Inclusive OR
  // void op_0A_ASL_();                   // 0A - ASL - none - Arithmetic Shift Left
  // void op_0B_ANC_imm_();               // 0B - ANC - imm  - AND A and immediate
  // void op_0D_ORA_abs_();               // 0D - ORA - abs  - Logical Inclusive OR
  // void op_0E_ASL_abs_();               // 0E - ASL - abs  - Arithmetic Shift Left
  // void op_0F_SLO_abs_();               // 0F - SLO - abs  - Shift Left then OR
  // void op_10_BPL_rel_();               // 10 - BPL - rel  - Branch if Positive
  // void op_11_ORA_izy_();               // 11 - ORA - izy  - Logical Inclusive OR
  // void op_13_SLO_izy_();               // 13 - SLO - izy  - Shift left then OR
  // void op_15_ORA_zpx_();               // 15 - ORA - zpx  - Logical Inclusive OR
  // void op_16_ASL_zpx_();               // 16 - ASL - zpx  - Arithmetic Shift Left
  // void op_17_SLO_zpx_();               // 17 - SLO - zpx  - Shift left then OR
  // void op_18_CLC_();                   // 18 - CLC - none - Clear Carry Flag
  // void op_19_ORA_aby_();               // 19 - ORA - aby  - Logical Inclusive OR
  // void op_1B_SLO_aby_();               // 1B - SLO - aby  - Shift left then OR
  // void op_1D_ORA_abx_();               // 1D - ORA - abx  - Logical Inclusive OR
  // void op_1E_ASL_abx_();               // 1E - ASL - abx  - Arithmetic Shift Left
  // void op_1F_SLO_abx_();               // 1F - SLO - abx  - Shift left then OR
  // void op_20_JSR_abs_();               // 20 - JSR - abs  - Jump to subroutine
  // void op_21_AND_izx_();               // 21 - AND - izx  - Logical AND  
  // void op_23_RLA_izx_();               // 23 - RLA - izx  - Rotate Left then AND
  // void op_24_BIT_zp_();                // 24 - BIT - zp   - Bit Test
  // void op_25_AND_zp_();                // 25 - AND - zp   - Logical AND
  // void op_26_ROL_zp_();                // 26 - ROT - zp   - Rotate Left
};

#endif // CPU_HPP
