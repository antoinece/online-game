//
// Created by ROG on 08.04.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_GAME_H_
#define ONLINEGAME_GAME_INCLUDE_GAME_H_

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "player_controler.h"

#include <iostream>

#include "const.h"
#include "renderer.h"

enum class Status {
  NOT_CONNECTED,
  CONNECTED
};

class Game{
  Game(){};
  void Init();
  void Loop();
 private:
  Renderer renderer = Renderer("hockey");
  bool isOpen = true;
  PlayerController player = PlayerController(100);
  std::vector<sf::Sprite> sprites;
  sf::Clock deltaClock;
  sf::RectangleShape playerShape;
};

#endif //ONLINEGAME_GAME_INCLUDE_GAME_H_
