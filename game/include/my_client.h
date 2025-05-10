//
// Created by ROG on 10.05.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_
#define ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "network.h"
#include "client_interface.h"

class MyClient : public ClientInterface {
 private:
  sf::Vector2f direction_{};


 public:
  // ✅ Méthodes obligatoires de Listener
  void debugReturn(int /*debugLevel*/, const ExitGames::Common::JString & /*string*/) override {}

  void connectionErrorReturn(int /*errorCode*/) override {}

  void clientErrorReturn(int /*errorCode*/) override {}

  void warningReturn(int /*warningCode*/) override {}

  void serverErrorReturn(int /*errorCode*/) override {}

  void joinRoomEventAction(int /*playerNr*/,
                           const ExitGames::Common::JVector<int> & /*playernrs*/,
                           const ExitGames::LoadBalancing::Player & /*player*/) override {}

  void leaveRoomEventAction(int /*playerNr*/, bool /*isInactive*/) override {}

  void joinRoomReturn(int /*localPlayerNr*/,
                      const ExitGames::Common::JVector<int> & /*playernrs*/,
                      const ExitGames::LoadBalancing::Player & /*player*/) {}

  void leaveRoomReturn(int /*errorCode*/, const ExitGames::Common::JString & /*errorString*/) override {}

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString &errorString,
                     const ExitGames::Common::JString &cluster) {
    std::cout << "Connected to Photon Cloud!\n";
    ExitGames::LoadBalancing::RoomOptions roomOptions;
    NetworkManager::GetLoadBalancingClient().opJoinOrCreateRoom(L"test-room", roomOptions);
  }

  void disconnectReturn() override {
    std::cout << "Disconnected from Photon Cloud.\n";
  }

  void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object &eventContent) override {
    //std::cout << "[Photon] customEventAction triggered!\n";
    if(eventCode == 1)
    {
      auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      //std::cout << "[Photon] Message from player " << playerNr << ": " << message.UTF8Representation().cstr() << std::endl;
    }
    if (eventCode == 2)
    {
       auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
       //std::cout << "[Photon] Message from player " << playerNr << ": " << message.UTF8Representation().cstr() << std::endl;
      DecryptMess(message);
    }
  }

  void roomListUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::Room> & /*roomList*/) {}

  void roomPropertiesChange(const ExitGames::Common::Hashtable & /*properties*/) {}

  void playerPropertiesChange(int /*playerNr*/, const ExitGames::Common::Hashtable & /*properties*/) {}

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString &errorString,
                     const ExitGames::Common::JString &region,
                     const ExitGames::Common::JString &cluster) override {

    std::cout << "[Photon] connectReturn code: " << errorCode << ", message: " << errorString.UTF8Representation().cstr() << "\n";
    if(errorCode == 0)
    {
      std::cout << "[Photon] Connected successfully! Joining room...\n";
      ExitGames::LoadBalancing::RoomOptions roomOptions;
      NetworkManager::GetLoadBalancingClient().opJoinOrCreateRoom(L"test-room", roomOptions);
    }
  }

  void DecryptMess(const ExitGames::Common::JString& message){
    std::string dirStr = message.UTF8Representation().cstr();
    size_t commaPos = dirStr.find(',');
    float directionX = 0.0f;
    float directionY = 0.0f;
    if (commaPos != std::string::npos) {
      std::string xStr = dirStr.substr(0, commaPos);
      std::string yStr = dirStr.substr(commaPos + 1);

      directionX = std::stof(xStr);
      directionY = std::stof(yStr);
    }
    const auto direction = sf::Vector2f {directionX,directionY};
    direction_ = direction;
  }

  sf::Vector2f& direction() {return direction_;}
};

#endif //ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_
