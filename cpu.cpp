/*
 * Author:                  Emanuel Aracena
 * Date:                    May 27, 2019
 * Project (working) title: NES Emulator
 * Filename:                CPU.hpp
 * Description:             CPU implementation.
 */

#include "CPU.hpp"

CPU::CPU() {
  cycle_time_ = std::chrono::nanoseconds(513);
  MEM_.resize(65536, 0);
  opcode_table_ = {
    
  };
}

void CPU::powerup() {
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
}

void CPU::emulate_cycle_timing() {
  std::this_thread::sleep_for((cycles_to_emulate_ * cycle_time_) - elapsed_exec_time_);
}

void CPU::fetch() {
  opcode_ = MEM_[PC_] << 8 | MEM_[PC_+1];
}

void CPU::decode_execute() {

}


// Opcode implementations

void op_00_BRK_() {
  // (0x00) BRK - Force Interrupt - 7 cycles
  // program counter and processor status pushed onto stack
  // IRQ interrupt vector at $FFFE/F is loaded into the PC and
  // break flag in status register is set to one.
  //
}
