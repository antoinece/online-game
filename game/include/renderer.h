//
// Created by ROG on 07.04.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_RENDERER_H_
#define ONLINEGAME_GAME_INCLUDE_RENDERER_H_

#include <SFML/Graphics.hpp>
#include "const.h"
/**
 * gère tout la window les sprites et tout les draw
 */
class Renderer {
 private:
  sf::RenderWindow window_;

  sf::RectangleShape playerShape;
  sf::RectangleShape playerShape2;
  sf::RectangleShape ballShape;
  sf::RectangleShape rightWallShape;
  sf::RectangleShape leftWallShape;
  sf::RectangleShape bottomWallShape;
  sf::RectangleShape topWallShape;
  sf::Texture texture_background;
  sf::Texture texture_cages;
  std::vector<sf::Sprite> sprites{};

  void Clear(const sf::Color color = sf::Color::Black) { window_.clear(color); }

  void Draw(const sf::Drawable &drawable) { window_.draw(drawable); }

  void Render(const std::vector<sf::Sprite> &sprites) {
    for (auto &sprite : sprites) {
      Draw(sprite);
    }
  }

  sf::RectangleShape InitShape(sf::Vector2f size, sf::Color color = sf::Color::Magenta, sf::Vector2f pos = {0, 0}) {
    sf::RectangleShape shape;
    shape.setSize(size);
    shape.setFillColor(color);
    shape.setPosition(pos);
    return shape;
  }

 public:
  Renderer(std::string_view name) {
    auto window_size = sf::VideoMode({kWindowWidth, kWindowLength});
    window_.create(window_size, name.data());
  }

  sf::RenderWindow &Window() { return window_; }

  void Display() { window_.display(); }

  void RenderInit() {
    // Chargement des texture et sprites
    if (!texture_background.loadFromFile("data/sprites/h2.png")) {
      std::cerr << "error loading texture";
    }
    sf::Sprite sprite_background(texture_background);
    sprite_background.setPosition(sf::Vector2f(0.f, 100.f));
    sprites.push_back(sprite_background);

    if (!texture_cages.loadFromFile("data/sprites/cages.png")) {
      std::cerr << "error loading texture";
    }
    sf::Sprite sprite_cages(texture_cages);
    sprite_cages.setScale(sf::Vector2f(0.15f, 0.2f));
    sprite_cages.setPosition(sf::Vector2f(130.f, 332.f));
    sprites.push_back(sprite_cages);

    sf::Sprite sprite_cages2(texture_cages);
    sprite_cages2.setRotation(sf::degrees(180));
    sprite_cages2.setScale(sf::Vector2f(0.15f, 0.2f));
    sprite_cages2.setPosition(sf::Vector2f(kWindowWidthF - 132.f, 483.f));
    sprites.push_back(sprite_cages2);

    //chargement des rendus des walls
    topWallShape = InitShape({kWindowWidthF, 10.f}, sf::Color::Blue, {0.f, 100.f});
    bottomWallShape = InitShape({kWindowWidthF, 10.f}, sf::Color::Blue, {0.f, kWindowLengthF - 10.f});
    leftWallShape = InitShape({10.f, kWindowLengthF}, sf::Color::Blue, {0.f, 0.f});
    rightWallShape = InitShape({10.f, kWindowLengthF}, sf::Color::Blue, {kWindowWidthF - 10.f, 0.f});
    playerShape = InitShape({50.f, 50.f}, sf::Color::Green);
    playerShape2 = InitShape({50.f, 50.f}, sf::Color::Red);
    ballShape = InitShape({25.f, 25.f}, sf::Color::Black);
  }

  void RendererUpdate(auto playerPos, auto player2Pos, auto ballPos) {
    //update pose graphique des player
    playerShape.setPosition(sf::Vector2f(playerPos.x - 25.f, playerPos.y - 25.f));
    playerShape2.setPosition(sf::Vector2f(player2Pos.x - 25.f, player2Pos.y - 25.f));
    ballShape.setPosition(sf::Vector2f(ballPos.x - 12.5f, ballPos.y - 12.5f));

    //draw
    Clear();
    Render(sprites);
    Draw(playerShape);
    Draw(playerShape2);
    Draw(ballShape);
    Draw(rightWallShape);
    Draw(leftWallShape);
    Draw(topWallShape);
    Draw(bottomWallShape);
  }
};

#endif //ONLINEGAME_GAME_INCLUDE_RENDERER_H_
