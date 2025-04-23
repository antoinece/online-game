
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
//#include <misc/cpp/imgui_stdlib.h>
//#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <iostream>
#include "const.h"
#include "player_controler.h"
#include "physics_world.h"
#include "wall.h"
#include "ball.h"

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

  Wall wall(physics_world_);

// Mur supérieur
wall.CreateWall(
      crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, 100.f),
      crackitos_core::math::Vec2f(kWindowWidthF, 20.f)
  );

// Mur inférieur
  wall.CreateWall(
      crackitos_core::math::Vec2f(kWindowWidthF / 2.0f, kWindowLengthF),
      crackitos_core::math::Vec2f(kWindowWidthF, 20.f)
  );
// Mur gauche
  wall.CreateWall(
      crackitos_core::math::Vec2f(0.f, kWindowLengthF / 2.0f),
      crackitos_core::math::Vec2f(20.f, kWindowLengthF)
  );
// Mur droite
  wall.CreateWall(
      crackitos_core::math::Vec2f(kWindowWidthF, kWindowLengthF / 2.0f),
      crackitos_core::math::Vec2f(20.f, kWindowLengthF)
  );
//mur cage gauche
  wall.CreateWall({130.f, 407.f},{5.f, 151.f});//fond
  wall.CreateWall({155.f, 332.f},{50.f, 5.f});//haut
  wall.CreateWall({155.f, 483.f}, {50.f, 5.f});//bas
//mur cage droite
  wall.CreateWall({kWindowWidthF-132.f, 407.f}, {5.f, 151.f});//fond
  wall.CreateWall({kWindowWidthF-157.f, 332.f}, {50.f, 5.f});//haut
  wall.CreateWall({kWindowWidthF-157.f, 483.f}, {50.f, 5.f});//haut



//  auto leftGoalTriggerHandle = wall.CreateWall({155.f, 407.f},{40.f, 140.f});
//  auto rightGoalTriggerHandle = wall.CreateWall({kWindowWidthF-157.f, 407.f},{40.f, 140.f});

  // Chargement de la texture
  sf::Texture texture_background;
  if (!texture_background.loadFromFile("data/sprites/h2.png")) {
    return -1;
  }
  sf::Sprite sprite_background(texture_background);
  sprite_background.setPosition(sf::Vector2f(0.f, 100.f));

  sf::Texture texture_cages;
  if (!texture_cages.loadFromFile("data/sprites/cages.png")) {
    return -1;
  }
  sf::Sprite sprite_cages(texture_cages);
  sprite_cages.setScale(sf::Vector2f(0.15f,0.2f));
  sprite_cages.setPosition(sf::Vector2f(130.f, 332.f));

  sf::Sprite sprite_cages2(texture_cages);
  sprite_cages2.setRotation(sf::degrees(180));
  sprite_cages2.setScale(sf::Vector2f(0.15f,0.2f));
  sprite_cages2.setPosition(sf::Vector2f(kWindowWidthF-132.f, 483.f));



  PlayerController player(1000, physics_world_,{200,408});
  PlayerController player2(1000, physics_world_,{kWindowWidthF-200,408});
  Ball ball(physics_world_);

  if (!ImGui::SFML::Init(window)) {
    std::cerr << "window creation error";
  }


  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;


  ImGui::SetNextWindowSize({kWindowWidthF, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({0.f, 0.f}, ImGuiCond_Always);



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

  sf::RectangleShape playerShape(sf::Vector2f(50.f, 50.f));
  playerShape.setFillColor(sf::Color::Green);

  sf::RectangleShape playerShape2(sf::Vector2f(50.f, 50.f));
  playerShape2.setFillColor(sf::Color::Red);

  sf::RectangleShape ballShape(sf::Vector2f(25.f, 25.f));
  ballShape.setFillColor(sf::Color::Black);

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
    sf::Vector2f direction2(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) direction2.y = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) direction2.y = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction2.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction2.x = 1.f;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) std::cout<< sf::Mouse::getPosition(window).x << " : " << sf::Mouse::getPosition(window).y<<"\n";
    player.Move(direction);
    player2.Move(direction2);
    //player.Update(deltaClock.getElapsedTime().asSeconds());

    // Rendu des joueur
    auto playerPos = player.GetPosition();
    playerShape.setPosition(sf::Vector2f (playerPos.x - 25.f, playerPos.y - 25.f));

    auto player2Pos = player2.GetPosition();
    playerShape2.setPosition(sf::Vector2f (player2Pos.x - 25.f, player2Pos.y - 25.f));

    auto ballPos = ball.GetPosition();
    ballShape.setPosition(sf::Vector2f (ballPos.x - 12.5f, ballPos.y - 12.5f));

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
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
    window.draw(sprite_cages);
    window.draw(sprite_cages2);
    window.draw(playerShape);
    window.draw(playerShape2);
    window.draw(ballShape);

    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}