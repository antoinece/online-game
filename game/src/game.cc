#include "game.h"

void Game::Init() {



  sf::Texture texture;
  if (!texture.loadFromFile("data/sprites/h2.png")) {
    std::cerr<<"error while loading data/sprites/h2.png";
  }
  sf::Sprite sprite(texture);
  sprite.setPosition(sf::Vector2f (0,0));

  playerShape.setSize(sf::Vector2f(20.f, 20.f)); // Taille du carré
  playerShape.setFillColor(sf::Color::Red);
  sprites.push_back(sprite);

  if (!ImGui::SFML::Init(renderer.Window())) {
    std::cerr<<"window creation error";
  }



  Status status = Status::NOT_CONNECTED;

  std::vector<std::string> receivedMessages;

  ImGui::SetNextWindowSize({300, 100}, ImGuiCond_Always);
  ImGui::SetNextWindowPos({20.0f, 20.0f}, ImGuiCond_Always);
}


void Game::Loop() {
  while (isOpen) {
    while (const std::optional event = renderer.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(renderer.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    sf::Vector2f direction(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      direction.y = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      direction.y = 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      direction.x = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      direction.x = 1.f;

    }
    player.Move(direction);



    sf::Vector2f playerPos(static_cast<float>(player.GetPosition().x),
                           static_cast<float>(player.GetPosition().y));
    playerShape.setPosition(playerPos);

    player.Update(deltaClock.getElapsedTime().asSeconds());

    ImGui::SFML::Update(renderer.Window(), deltaClock.restart());
    auto [x, y] = renderer.Window().getSize();

    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::End();
    renderer.Clear();

    ImGui::SFML::Render(renderer.Window());


    renderer.Render(sprites);
    renderer.Draw(playerShape);
    renderer.Display();
  }
}