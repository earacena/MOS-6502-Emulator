/*
 * Author:                  Emanuel Aracena Beriguete
 * Date:                    June 6, 2019
 * Project (working) title: MOS 6502 Emulator
 * Filename:                gui.cpp
 * Description:             GUI implementation.
 *                          Using SDL and ImGui.
 */

#include "gui.hpp"

GUI::GUI() { /*  */  }

void GUI::initialize() {
  // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
  //   std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
  //   return 0x01;
  // }

  // window_ = SDL_CreateWindow("MOS 6502 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
  //                           1000, 600, SDL_WINDOW_OPENGL);
  // if(window_ == nullptr) {
  //   std::cout << "Could not create window: " << SDL_GetError() << std::endl;
  //   return 0x01;
  // }

  // renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // if (renderer_ == nullptr) {
  //   SDL_DestroyWindow(window_);
  //   std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
  //   SDL_Quit();
  //   return 0x01;
  // }

  // return 0x00;
  
  window_.create(sf::VideoMode(1000, 600), "MOS-6502 Emulator"); 
}

void GUI::show_logger_window() {
}

void GUI::cleanup() {
 // SDL_DestroyRenderer(renderer_);
 // SDL_DestroyWindow(window_);
 // SDL_Quit();

 
}

