
#ifndef PLAYER_CONTROLER_H_
#define PLAYER_CONTROLER_H_


#include <SFML/Graphics.hpp>
#include <body.h>

class PlayerController {
 public:
  PlayerController(float m){move_speed=m;};


  void Update(float dt){body_.Update(dt);};
  void Move(sf::Vector2f direction){body_.ApplyForce(crackitos_core::math::Vec2f((direction * move_speed).x,(direction * move_speed).y));};

 private:
  float move_speed = 3;
  crackitos_physics::physics::Body body_;
};


#endif //PLAYER_CONTROLER_H_
