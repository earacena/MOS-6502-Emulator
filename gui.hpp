/*
 * Author:                  Emanuel Aracena
 * Date:                    June 6, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                gui.hpp
 * Description:             GUI interface.
 *                          Using SDL and ImGui.
 */

#ifndef GUI_HPP
#define GUI_HPP

#include <iostream>

#include "SDL2/SDL.h"
#include "imgui/imgui.h"


class GUI {
  friend class Logger;
  friend class Emulator;

 public:
  GUI();
  uint8_t initialize();
  void show_logger_window();
  void cleanup();
  
 private:
  SDL_Window   * window_;
  SDL_Renderer * renderer_;
  
};

#endif // GUI_HPP

