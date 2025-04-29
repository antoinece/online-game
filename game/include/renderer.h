//
// Created by ROG on 07.04.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_RENDERER_H_
#define ONLINEGAME_GAME_INCLUDE_RENDERER_H_

#include <SFML/Graphics.hpp>
#include "const.h"

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

 public:
  Renderer(std::string_view name) {
    auto window_size = sf::VideoMode({kWindowWidth, kWindowLength});
    window_.create(window_size, name.data());
  }

  sf::RenderWindow& Window(){return window_;}

  void Clear(const sf::Color color = sf::Color::Black){window_.clear(color);}

  void Display(){window_.display();}

  void Draw(const sf::Drawable& drawable){window_.draw(drawable);}

  void Render(const std::vector<sf::Sprite>& sprites)
  {
    for (auto& sprite:sprites) {
      Draw(sprite);
    }
  }

  void RenderInit(){
    // Chargement de la texture

    if (!texture_background.loadFromFile("data/sprites/h2.png")) {
      std::cerr<<"error loading texture";
    }
    sf::Sprite sprite_background(texture_background);
    sprite_background.setPosition(sf::Vector2f(0.f, 100.f));
    sprites.push_back(sprite_background);


    if (!texture_cages.loadFromFile("data/sprites/cages.png")) {
      std::cerr<<"error loading texture";
    }
    sf::Sprite sprite_cages(texture_cages);
    sprite_cages.setScale(sf::Vector2f(0.15f,0.2f));
    sprite_cages.setPosition(sf::Vector2f(130.f, 332.f));
    sprites.push_back(sprite_cages);

    sf::Sprite sprite_cages2(texture_cages);
    sprite_cages2.setRotation(sf::degrees(180));
    sprite_cages2.setScale(sf::Vector2f(0.15f,0.2f));
    sprite_cages2.setPosition(sf::Vector2f(kWindowWidthF-132.f, 483.f));
    sprites.push_back(sprite_cages2);

    topWallShape.setSize(sf::Vector2f(kWindowWidthF, 10.f));
    topWallShape.setFillColor(sf::Color::Blue);
    topWallShape.setPosition(sf::Vector2f(0.f, 100.f));

    bottomWallShape.setSize(sf::Vector2f(kWindowWidthF, 10.f));
    bottomWallShape.setFillColor(sf::Color::Blue);
    bottomWallShape.setPosition(sf::Vector2f(0.f, kWindowLengthF - 10.f));

    leftWallShape.setSize(sf::Vector2f(10.f, kWindowLengthF));
    leftWallShape.setFillColor(sf::Color::Blue);
    leftWallShape.setPosition(sf::Vector2f(0.f, 0.f));

    rightWallShape.setSize(sf::Vector2f(10.f, kWindowLengthF));
    rightWallShape.setFillColor(sf::Color::Blue);
    rightWallShape.setPosition(sf::Vector2f(kWindowWidthF - 10.f, 0.f));

    playerShape.setSize(sf::Vector2f(50.f, 50.f));
    playerShape.setFillColor(sf::Color::Green);

    playerShape2.setSize(sf::Vector2f(50.f, 50.f));
    playerShape2.setFillColor(sf::Color::Red);

    ballShape.setSize(sf::Vector2f(25.f, 25.f));
    ballShape.setFillColor(sf::Color::Black);
  }

  void RendererUpdate(auto playerPos,auto player2Pos,auto ballPos){
    playerShape.setPosition(sf::Vector2f (playerPos.x - 25.f, playerPos.y - 25.f));
    playerShape2.setPosition(sf::Vector2f (player2Pos.x - 25.f, player2Pos.y - 25.f));
    ballShape.setPosition(sf::Vector2f (ballPos.x - 12.5f, ballPos.y - 12.5f));

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
