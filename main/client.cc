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
#include <LoadBalancing-cpp/inc/Client.h>
#include "my_client.h"

enum class Status {
  NOT_CONNECTED,
  CONNECTED
};

int main() {

  bool isOpen = true;
  sf::Clock deltaClock;
  Status status = Status::NOT_CONNECTED;

  MyClient client;
  ExitGames::LoadBalancing::ClientConstructOptions options;
  NetworkManager::Begin(&client, options);


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

  crackitos_core::timer::Timer timer;
  timer.SetFixedDeltaTime(1.0f / 60.0f);

  while (isOpen) {



    NetworkManager::Tick();

    // Gestion des événements
    while (const std::optional event = renderer.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(renderer.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    // Gestion des inputs
    float directionX = 0;
    float directionY = 0;
    sf::Vector2f direction2(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) directionY = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) directionY = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) directionX = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) directionX = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) direction2.y = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) direction2.y = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction2.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction2.x = 1.f;
    //if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) std::cout<< sf::Mouse::getPosition(window).x << " : " << sf::Mouse::getPosition(window).y<<"\n";
    ///TODO input manager
    //update physique
    timer.Tick();

    //--------------FIXED UPDATE----------------
    while (timer.FixedDeltaTimeStep())
    {
      physics_world_.Update();
    }

    sf::Vector2f dir = client.direction();


    player.Move({directionX,directionY});
    player2.Move(dir);

    //imgui
    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SFML::Update(renderer.Window(), deltaClock.restart());
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();





    static char message[256] = "";
    ImGui::InputText("Message", message, IM_ARRAYSIZE(message));

    if (ImGui::Button("Send"))
    {
      std::cout << "[Photon] Trying to send message: " << message << "\n";
      bool success = NetworkManager::GetLoadBalancingClient().opRaiseEvent(true, ExitGames::Common::JString(message), 1);
      std::cout << "[Photon] Message sent status: " << (success ? "Success" : "Failure") << "\n";
    }
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%f,%f", directionX, directionY);
    ExitGames::Common::JString jsDir(buf);

    NetworkManager::GetLoadBalancingClient()
        .opRaiseEvent(false, jsDir, 2);









    renderer.RendererUpdate(player.GetPosition(), player2.GetPosition(), ball.GetPosition());
    ImGui::SFML::Render(renderer.Window());
    renderer.Display();
  }

  NetworkManager::End();
  ImGui::SFML::Shutdown();
  return 0;
}