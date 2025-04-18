
/*
#include "game.h"

int main() {

  Game game;
  game.Init();
  game.Loop();

  ImGui::SFML::Shutdown();
  return 0;
}*/

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <iostream>
#include "const.h"
#include "player_controler.h"
#include "physics_world.h"

enum class Status {
  NOT_CONNECTED,
  CONNECTED
};

int main() {
  sf::RenderWindow window(sf::VideoMode({kWindowWidth, kWindowLength}), "hockey");
  window.setFramerateLimit(60);
  crackitos_physics::physics::PhysicsWorld physics_world_;

  // Initialisation du monde physique
  crackitos_core::math::AABB worldBounds(
      crackitos_core::math::Vec2f(0, 0),
      crackitos_core::math::Vec2f(kWindowWidthF, kWindowLengthF)
  );
  physics_world_.Initialize(worldBounds, true, crackitos_core::math::Vec2f(0, 0));

  // Création des murs avec leur collider
  // Mur supérieur
  crackitos_physics::physics::Body topWallBody;
  topWallBody.set_position(crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, 105.f)); // Position Y à 5.f (moitié de l'épaisseur)
  topWallBody.set_mass(0.0f);
  auto topWallHandle = physics_world_.CreateBody(topWallBody);
  crackitos_physics::physics::Collider topWallCollider(
      crackitos_core::math::AABB(crackitos_core::math::Vec2f(-kWindowWidthF/2, -5.f), crackitos_core::math::Vec2f(kWindowWidthF/2, 5.f)),
      0.0f, 1.0f, false, topWallHandle
  );
  physics_world_.CreateCollider(topWallHandle, topWallCollider);

  // Mur inférieur
  crackitos_physics::physics::Body bottomWallBody;
  bottomWallBody.set_position(crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, kWindowLengthF - 5.f));
  bottomWallBody.set_mass(0.0f);
  auto bottomWallHandle = physics_world_.CreateBody(bottomWallBody);
  crackitos_physics::physics::Collider bottomWallCollider(
      crackitos_core::math::AABB(crackitos_core::math::Vec2f(-kWindowWidthF/2, -5.f), crackitos_core::math::Vec2f(kWindowWidthF/2, 5.f)),
      0.0f, 1.0f, false, bottomWallHandle
  );
  physics_world_.CreateCollider(bottomWallHandle, bottomWallCollider);

  // Mur gauche
  crackitos_physics::physics::Body leftWallBody;
  leftWallBody.set_position(crackitos_core::math::Vec2f(5.f, kWindowLengthF / 2.0f));
  leftWallBody.set_mass(0.0f);
  auto leftWallHandle = physics_world_.CreateBody(leftWallBody);
  crackitos_physics::physics::Collider leftWallCollider(
      crackitos_core::math::AABB(crackitos_core::math::Vec2f(-5.f, -kWindowLengthF/2), crackitos_core::math::Vec2f(5.f, kWindowLengthF/2)),
      0.0f, 1.0f, false, leftWallHandle
  );
  physics_world_.CreateCollider(leftWallHandle, leftWallCollider);

  // Mur droit
  crackitos_physics::physics::Body rightWallBody;
  rightWallBody.set_position(crackitos_core::math::Vec2f(kWindowWidthF - 5.f, kWindowLengthF / 2.0f));
  rightWallBody.set_mass(0.0f);
  auto rightWallHandle = physics_world_.CreateBody(rightWallBody);
  crackitos_physics::physics::Collider rightWallCollider(
      crackitos_core::math::AABB(crackitos_core::math::Vec2f(-5.f, -kWindowLengthF/2), crackitos_core::math::Vec2f(5.f, kWindowLengthF/2)),
      0.0f, 1.0f, false, rightWallHandle
  );
  physics_world_.CreateCollider(rightWallHandle, rightWallCollider);

  // Chargement de la texture
  sf::Texture texture_background;
  if (!texture_background.loadFromFile("data/sprites/h2.png")) {
    return -1;
  }
  sf::Sprite sprite_background(texture_background);
  sprite_background.setPosition(sf::Vector2f(0, 100));

  sf::Texture texture_cages;
  if (!texture_cages.loadFromFile("data/sprites/cages.png")) {
    return -1;
  }
  sf::Sprite sprite_cages(texture_cages);
  sprite_cages.setScale(sf::Vector2f(0.15f,0.2f));
  sprite_cages.setPosition(sf::Vector2f(130, 332));

  sf::Sprite sprite_cages2(texture_cages);
  sprite_cages2.setRotation(sf::degrees(180));
  sprite_cages2.setScale(sf::Vector2f(0.15f,0.2f));
  sprite_cages2.setPosition(sf::Vector2f(kWindowWidthF-132, 483));



  PlayerController player(100, physics_world_);

  if (!ImGui::SFML::Init(window)) {
    std::cerr << "window creation error";
  }

  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;

  ImGui::SetNextWindowSize({300, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({20.0f, 20.0f}, ImGuiCond_Always);

  // Préparation des formes des murs pour le rendu
  sf::RectangleShape topWallShape(sf::Vector2f(kWindowWidthF, 10.f));
  topWallShape.setFillColor(sf::Color::Blue);
  topWallShape.setPosition(sf::Vector2f(0.f, 100.f));

  sf::RectangleShape bottomWallShape(sf::Vector2f(kWindowWidthF, 10.f));
  bottomWallShape.setFillColor(sf::Color::Blue);
  bottomWallShape.setPosition(sf::Vector2f(0.f, kWindowLengthF - 10.f));

  sf::RectangleShape leftWallShape(sf::Vector2f(10.f, kWindowLengthF));
  leftWallShape.setFillColor(sf::Color::Blue);
  leftWallShape.setPosition(sf::Vector2f(0.f, 0.f));

  sf::RectangleShape rightWallShape(sf::Vector2f(10.f, kWindowLengthF));
  rightWallShape.setFillColor(sf::Color::Blue);
  rightWallShape.setPosition(sf::Vector2f(kWindowWidthF - 10.f, 0.f));

  while (isOpen) {
    physics_world_.Update(deltaClock.getElapsedTime().asSeconds());

    // Gestion des événements
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    // Gestion des inputs
    sf::Vector2f direction(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x = 1.f;
    player.Move(direction);
    player.Update(deltaClock.getElapsedTime().asSeconds());

    // Rendu du joueur
    sf::RectangleShape playerShape(sf::Vector2f(50.f, 50.f));
    playerShape.setFillColor(sf::Color::Green);
    auto playerPos = player.GetPosition();
    // Conversion de Vec2f en Vector2f
    sf::Vector2f visualPos(playerPos.x - 25.f, playerPos.y - 25.f);
    playerShape.setPosition(visualPos);



    // Mise à jour de l'UI
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();

    // Rendu
    window.clear();
    window.draw(sprite_background);
    window.draw(topWallShape);
    window.draw(bottomWallShape);
    window.draw(leftWallShape);
    window.draw(rightWallShape);
    window.draw(playerShape);
    window.draw(sprite_cages);
    window.draw(sprite_cages2);

    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}