//
// Created by ROG on 23.04.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_WALL_H_
#define ONLINEGAME_GAME_INCLUDE_WALL_H_

#include "body.h"
#include "physics_world.h"
#include "collider.h"
#include "shape.h"
#include <SFML/Graphics.hpp>

/**
 * crée les wall physique du bord de map et des cages
 */
class Wall {
 public:
  explicit Wall(crackitos_physics::physics::PhysicsWorld &world)
      : world_(world) {}

  crackitos_physics::physics::BodyHandle CreateWall(const crackitos_core::math::Vec2f &position,
                                                    const crackitos_core::math::Vec2f &size) {
    using namespace crackitos_physics::physics;
    using namespace crackitos_core::math;

    // Crée le body statique
    Body wall_body;
    wall_body.set_type(BodyType::Static);
    wall_body.set_position(position);
    wall_body.set_mass(0.0f);

    BodyHandle body_handle = world_.CreateBody(wall_body);
    Vec2f half_size_vec = size * 0.5f;
    Vec2f min_bound = position - half_size_vec; // Coin inférieur gauche
    Vec2f max_bound = position + half_size_vec; // Coin supérieur droit

    float half_size_length = half_size_vec.Magnitude();

    Vec2f centre = position;

    AABB shape(min_bound, max_bound, centre, half_size_vec, half_size_length);

    Collider collider(shape, 0.0f, 0.8f, false, body_handle);
    world_.CreateCollider(body_handle, collider);

    return body_handle;
  }

  void WallInit() {
    // Mur supérieur
    CreateWall(
        crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, 100.f),
        crackitos_core::math::Vec2f(kWindowWidthF, 20.f)
    );
// Mur inférieur
    CreateWall(
        crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, kWindowLengthF),
        crackitos_core::math::Vec2f(kWindowWidthF, 20.f)
    );
// Mur gauche
    CreateWall(
        crackitos_core::math::Vec2f(0.f, kWindowLengthF / 2.0f),
        crackitos_core::math::Vec2f(20.f, kWindowLengthF)
    );
// Mur droite
    CreateWall(
        crackitos_core::math::Vec2f(kWindowWidthF, kWindowLengthF / 2.0f),
        crackitos_core::math::Vec2f(20.f, kWindowLengthF)
    );
//mur cage gauche
    CreateWall({130.f, 407.f}, {5.f, 151.f});//fond
    CreateWall({155.f, 332.f}, {50.f, 5.f});//haut
    CreateWall({155.f, 483.f}, {50.f, 5.f});//bas
//mur cage droite
    CreateWall({kWindowWidthF - 132.f, 407.f}, {5.f, 151.f});//fond
    CreateWall({kWindowWidthF - 157.f, 332.f}, {50.f, 5.f});//haut
    CreateWall({kWindowWidthF - 157.f, 483.f}, {50.f, 5.f});//haut
//  auto leftGoalTriggerHandle = wall.CreateWall({155.f, 407.f},{40.f, 140.f});
//  auto rightGoalTriggerHandle = wall.CreateWall({kWindowWidthF-157.f, 407.f},{40.f, 140.f});
  }

 private:
  crackitos_physics::physics::PhysicsWorld &world_;
};

#endif //ONLINEGAME_GAME_INCLUDE_WALL_H_
