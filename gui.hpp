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

#include "SFML/Graphics.hpp"


class GUI {
  // friend class Logger;
  // friend class Emulator;

 public:
  GUI();
  void initialize();
  void show_logger_window();
  void cleanup();
  
 private:
  //SDL_Window   * window_;
  sf::RenderWindow window_;

  //SDL_Renderer * renderer_;
  
};

#endif // GUI_HPP

