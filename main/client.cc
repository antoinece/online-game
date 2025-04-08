
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <iostream>

#include "const.h"

enum class Status {
  NOT_CONNECTED,
  CONNECTED
};

int main() {
  sf::RenderWindow window(sf::VideoMode({kWindowWidth, kWindowLength}), "hockey");
  window.setFramerateLimit(60);


  sf::Texture texture;
  if (!texture.loadFromFile("data/sprites/h2.png")) {
    return -1; // Vérifie si le fichier est bien chargé
  }
  sf::Sprite sprite(texture);
  sprite.setPosition(sf::Vector2f (0,0));



  if (!ImGui::SFML::Init(window)) {
    std::cerr<<"window creation error";
  }

  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;

  std::vector<std::string> receivedMessages;

  ImGui::SetNextWindowSize({300, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({20.0f, 20.0f}, ImGuiCond_Always);

  while (isOpen) {
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    auto [x, y] = window.getSize();

    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::End();
    window.clear();

    ImGui::SFML::Render(window);


    window.draw(sprite);


    window.display();
  }

  ImGui::SFML::Shutdown();
}