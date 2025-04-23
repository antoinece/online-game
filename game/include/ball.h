#ifndef ONLINEGAME_GAME_INCLUDE_BALL_H_
#define ONLINEGAME_GAME_INCLUDE_BALL_H_

#include <SFML/Graphics.hpp>
#include <body.h>
#include <physics_world.h>

class Ball {
 public:
  Ball(crackitos_physics::physics::PhysicsWorld& world)
      : world_(world) {

    crackitos_physics::physics::Body playerBody;
    playerBody.set_position(crackitos_core::math::Vec2f(719.f, 408.f)) ;
    playerBody.set_mass(1.0f);
    body_handle_ = world_.CreateBody(playerBody);

    crackitos_physics::physics::Collider playerCollider(
        crackitos_core::math::AABB( crackitos_core::math::Vec2f(0.0f, 0.0f), crackitos_core::math::Vec2f(25.0f, 25.0f)),
        0.5f,  // Bounciness (restitution)
        0.5f,  // Friction
        false, // Pas un trigger
        body_handle_ // Handle du corps
    );

    world_.CreateCollider(body_handle_, playerCollider);
  }


  crackitos_core::math::Vec2<float> GetPosition() {
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_handle_);
    return body.position();
  }

 private:
  float move_speed = 3;
  crackitos_physics::physics::PhysicsWorld& world_;  // Référence vers le monde physique
  crackitos_physics::physics::BodyHandle body_handle_;  // Handle vers le body du joueur
};

#endif //ONLINEGAME_GAME_INCLUDE_BALL_H_