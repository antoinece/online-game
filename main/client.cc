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


int main() {
  bool desyncButGoodPhysic = false;
  int frameCount = 0;
  bool isOpen = true;
  sf::Clock deltaClock;

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
    if (frameCount%20==0){
      desyncButGoodPhysic = false;
    } else{
      desyncButGoodPhysic = true;
    }
    int remoteNr = client.getRemotePlayerNr();
    int localNr = client.getLocalPlayerNr();
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
    if (renderer.Window().hasFocus()){
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) directionY = -1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) directionY = 1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) directionX = -1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) directionX = 1.f;
    }

    timer.Tick();
    //--------------FIXED UPDATE----------------
    while (timer.FixedDeltaTimeStep())
    {
      physics_world_.Update();
    }

    //imgui
    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SFML::Update(renderer.Window(), deltaClock.restart());
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Local Player : %d", localNr);
    ImGui::Text("Remote Player : %d", remoteNr);
    ImGui::Text("frame : %d", frameCount);
    ImGui::End();

    //multiplayer
    if (!desyncButGoodPhysic) {
      char buf[64];
      if (localNr == 1) {
        std::snprintf(buf, sizeof(buf), "%f,%f", player.GetPosition().x, player.GetPosition().y);
      } else if (localNr == 2) {
        std::snprintf(buf, sizeof(buf), "%f,%f", player2.GetPosition().x, player2.GetPosition().y);
      }
      ExitGames::Common::JString jsDir(buf);
      NetworkManager::GetLoadBalancingClient().opRaiseEvent(false, jsDir, 4);
      if (localNr == 1) {
        auto ballPos = ball.GetPosition();
        auto ballVel = ball.GetVelocity();
        char ballBuf[64];
        std::snprintf(ballBuf, sizeof(ballBuf), "ball:%f,%f", ballPos.x, ballPos.y);
        ExitGames::Common::JString jsBallPos(ballBuf);
        NetworkManager::GetLoadBalancingClient().opRaiseEvent(false, jsBallPos, 3);
//        char ballBuf2[64];
//        std::snprintf(ballBuf2, sizeof(ballBuf2), "ball:%f,%f", ballPos.x, ballPos.y);
//        ExitGames::Common::JString jsBallVel(ballBuf2);
//        NetworkManager::GetLoadBalancingClient().opRaiseEvent(false, jsBallPos, 5);
      }
      if (localNr == 1) {
        player.Move({directionX, directionY});
        player2.SetPos(client.pos2());
      } else {
        player.SetPos(client.pos1());
        player2.Move({directionX, directionY});
        ball.SetPos(client.getBallPos());
        //ball.SetVelocity(client.getBallVel());
      }
      frameCount=0;
    }else{
      char buf[64];
      std::snprintf(buf, sizeof(buf), "%f,%f", directionX, directionY);
      ExitGames::Common::JString jsDir(buf);
      NetworkManager::GetLoadBalancingClient().opRaiseEvent(false, jsDir, 2);
      if (localNr == 1) {
        player.Move({directionX, directionY});
        player2.Move(client.direction2());
      } else {
        player.Move(client.direction1());
        player2.Move({directionX, directionY});
      }
    }
    frameCount++;

    //render
    renderer.RendererUpdate(player.GetPosition(), player2.GetPosition(), ball.GetPosition(),remoteNr);
    ImGui::SFML::Render(renderer.Window());
    renderer.Display();
  }

  NetworkManager::End();
  ImGui::SFML::Shutdown();
  return 0;
}