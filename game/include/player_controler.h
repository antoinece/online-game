
#ifndef PLAYER_CONTROLER_H_
#define PLAYER_CONTROLER_H_


#include <SFML/Graphics.hpp>
#include <body.h>
#include <physics_world.h>
/**
 * utilisé pour la physique du player
 */
class PlayerController {
 public:
  PlayerController(float m, crackitos_physics::physics::PhysicsWorld& world,crackitos_core::math::Vec2f a = {100.f, 200.f})
      : move_speed(m), world_(world) {

    crackitos_physics::physics::Body playerBody;
    playerBody.set_position(a) ;
    playerBody.set_mass(10.0f);
    body_handle_ = world_.CreateBody(playerBody);

    crackitos_physics::physics::Collider playerCollider(
        crackitos_core::math::AABB( crackitos_core::math::Vec2f(0.0f, 0.0f), crackitos_core::math::Vec2f(50.0f, 50.0f)),
        0.5f,  // Bounciness (restitution)
        0.5f,  // Friction
        false, // Pas un trigger
        body_handle_ // Handle du corps
    );

    world_.CreateCollider(body_handle_, playerCollider);
  }

  void Update(float dt) {
    // Met à jour le body dans le monde physique
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_handle_);
    body.Update(dt);
  }

  void Move(sf::Vector2f direction) {
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_handle_);
    body.ApplyForce(crackitos_core::math::Vec2f((direction * move_speed).x, (direction * move_speed).y));
  }

  void SetPos(sf::Vector2f pos){
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_handle_);
    body.set_position({pos.x,pos.y});
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


#endif //PLAYER_CONTROLER_H_
