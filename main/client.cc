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
#include "renderer.h"

enum class Status {
  NOT_CONNECTED,
  CONNECTED
};

int main() {
  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;

  // Initialisation du monde physique
  crackitos_physics::physics::PhysicsWorld physics_world_;
  crackitos_core::math::AABB worldBounds(
      crackitos_core::math::Vec2f(0, 0),
      crackitos_core::math::Vec2f(kWindowWidthF, kWindowLengthF)
  );
  physics_world_.Initialize(worldBounds, true, crackitos_core::math::Vec2f(0, 0));
  PlayerController player(10, physics_world_, {200, 408});
  PlayerController player2(10, physics_world_, {kWindowWidthF - 200, 408});
  Ball ball(physics_world_);

  //creating Walls
  Wall wall(physics_world_);
  wall.WallInit();

  //starting renderer
  Renderer renderer("hockey");
  renderer.RenderInit();

  //imgui
  if (!ImGui::SFML::Init(renderer.Window())) {
    std::cerr << "window creation error";
  }
  ImGui::SetNextWindowSize({kWindowWidthF, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({0.f, 0.f}, ImGuiCond_Always);

  while (isOpen) {
    // Gestion des événements
    while (const std::optional event = renderer.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(renderer.Window(), *event);
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
    //if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) std::cout<< sf::Mouse::getPosition(window).x << " : " << sf::Mouse::getPosition(window).y<<"\n";

    //update physique
    physics_world_.Update(deltaClock.getElapsedTime().asSeconds());
    player.Move(direction);
    player2.Move(direction2);

    //imgui
    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SFML::Update(renderer.Window(), deltaClock.restart());
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();

    renderer.RendererUpdate(player.GetPosition(), player2.GetPosition(), ball.GetPosition());
    ImGui::SFML::Render(renderer.Window());
    renderer.Display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}