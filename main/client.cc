
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

int main() {

  Renderer renderer("hockey");

  PlayerController player(2);

  std::vector<sf::Sprite> sprites;

  sf::Texture texture;
  if (!texture.loadFromFile("data/sprites/h2.png")) {
    return -1; // Vérifie si le fichier est bien chargé
  }
  sf::Sprite sprite(texture);
  sprite.setPosition(sf::Vector2f (0,0));


;
  sprites.push_back(sprite);

  if (!ImGui::SFML::Init(renderer.Window())) {
    std::cerr<<"window creation error";
  }

  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;

  std::vector<std::string> receivedMessages;

  ImGui::SetNextWindowSize({300, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({20.0f, 20.0f}, ImGuiCond_Always);

  while (isOpen) {
    while (const std::optional event = renderer.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(renderer.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    player.Update(deltaClock.getElapsedTime().asSeconds());

    ImGui::SFML::Update(renderer.Window(), deltaClock.restart());
    auto [x, y] = renderer.Window().getSize();

    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::End();
    renderer.Clear();

    ImGui::SFML::Render(renderer.Window());

    renderer.Render(sprites);
  }

  ImGui::SFML::Shutdown();
}