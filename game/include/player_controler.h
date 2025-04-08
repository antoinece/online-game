
#ifndef MICROMACHINE_GAME_INCLUDE_PLAYER_CONTROLER_H_
#define MICROMACHINE_GAME_INCLUDE_PLAYER_CONTROLER_H_

#include <SFML/Graphics.hpp>

class PlayerController {
 public:
  PlayerController();

  sf::Vector2f position = {0,0};
  sf::Vector2f velocity = {0,0};

  void Update(float dt);

 private:

  float speed_ = 200.f; // Pixels par seconde
};


#endif //MICROMACHINE_GAME_INCLUDE_PLAYER_CONTROLER_H_
