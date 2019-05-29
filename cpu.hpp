/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                CPU.hpp
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

class CPU {
 public:
  // Initialization
  CPU();
  void powerup();              // power-up state

  // CPU emulation
  void emulate();              // fetch, decode, execute
  void emulate_cycle_timing(); // Attempt to accurately emulate cycle timings (513~ ns per CPU cycle)
  void fetch();                // fetch next instruction, update program counter
  void decode_execute();       // Look for opcode function, execute
  
 private:
  
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
  uint8_t  P_;   // Status register

  // Cycle timings
  std::chrono::nanoseconds cycle_time_;
  uint8_t cycles_to_emulate_;
  std::chrono::nanoseconds elapsed_exec_time_;
  std::chrono::high_resolution_clock::time_point exec_start_time_;
  std::chrono::high_resolution_clock::time_point exec_end_time_;

  // Opcode
  uint16_t opcode_;

  // Opcode table, implemented in sequential order for simplicity of
  // function lookup with byte hex offset,
  // 256 total (official) opcode entries
  std::vector<std::function<void()>> opcode_table_;

  // Opcode implementations
  // Addressing modes (post-fixes):
  // Ex: op_OPCODEHEX_OPNAME_POSTFIX_
  //   implied          -> none
  //   accumalator      -> acc
  //   immediate        -> imm
  //   zero page        -> zp
  //   zero page, x     -> zpx
  //   zero page, y     -> zpy
  //   indexed indirect -> izx
  //   indirect indexed -> izy
  //   absolute         -> abs
  //   absolute, x      -> abx
  //   absolute, y      -> aby
  //   indirect         -> ind
  //   relative         -> rel

  void op_00_BRK_();
};

#endif // CPU_HPP
