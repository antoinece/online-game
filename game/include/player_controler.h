
#ifndef PLAYER_CONTROLER_H_
#define PLAYER_CONTROLER_H_

#include <SFML/Graphics.hpp>
#include <crackitos_physics/physics/include/body.h>

class PlayerController : crackitos_physics::physics::Body  {
 public:
  PlayerController();

  sf::Vector2f position = {0,0};
  sf::Vector2f velocity = {0,0};

  void Update(float dt);

 private:


};


#endif //PLAYER_CONTROLER_H_
