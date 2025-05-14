
#ifndef ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_
#define ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "network.h"
#include "client_interface.h"
#include "const.h"

class MyClient : public ClientInterface {
 private:
  sf::Vector2f pos_1{200, 408};
  sf::Vector2f direction_1{};
  sf::Vector2f pos_2{kWindowWidthF - 200, 408};
  sf::Vector2f direction_2{};
  sf::Vector2f ballPos_{719.f, 408.f};
  sf::Vector2f ballVel_{};
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
    if(eventCode == 1)
    {
      auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
    }
    if (eventCode == 2)
    {
       auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      DecryptMess(message,playerNr,direction_1,direction_2);
    }
    if (eventCode == 3) {
      ExitGames::Common::JString message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      std::string s = message.UTF8Representation().cstr();
      if (s.starts_with("ball:")) {
        float bx = 0.f, by = 0.f;
        sscanf_s(s.c_str(), "ball:%f,%f", &bx, &by);
        ballPos_ = {bx, by};
      }
    }
    if (eventCode == 4)
    {
      auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      DecryptMess(message,playerNr,pos_1,pos_2);
    }
    if (eventCode == 5) {
      ExitGames::Common::JString message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      std::string s = message.UTF8Representation().cstr();
      if (s.starts_with("ball:")) {
        float bx = 0.f, by = 0.f;
        sscanf_s(s.c_str(), "ball:%f,%f", &bx, &by);
        ballVel_ = {bx, by};
      }
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

  void DecryptMess(const ExitGames::Common::JString& message,int playerNr,sf::Vector2f & vec1,sf::Vector2f & vec2){
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
    if (playerNr==1) {
      vec1 = direction;
    }
    if (playerNr==2) {
      vec2 = direction;
    }
  }

  std::vector<int> getAllPlayerNrs() const {
    std::vector<int> result;
    // Accède à la room jointe
    const auto& room = NetworkManager::GetLoadBalancingClient()
        .getCurrentlyJoinedRoom();
    // Photon renvoie un JVector de pointeurs Player*
    ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*> players = room.getPlayers();
    // Itère dessus
    for (unsigned int i = 0; i < players.getSize(); ++i) {
      result.push_back(players[i]->getNumber());
    }
    return result;
  }

  int getRemotePlayerNr() const {
    auto all   = getAllPlayerNrs();
    int  local = getLocalPlayerNr();
    for (int n : all) {
      if (n != local)
        return n;
    }
    return 0;
  }

  int getLocalPlayerNr() const {
    return NetworkManager::GetLoadBalancingClient()
        .getLocalPlayer()
        .getNumber();
  }


  sf::Vector2f getBallPos() const { return ballPos_; }
  sf::Vector2f getBallVel() const { return ballVel_; }
  sf::Vector2f& pos1() {return pos_1;}
  sf::Vector2f& pos2() {return pos_2;}
  sf::Vector2f& direction1() {return direction_1;}
  sf::Vector2f& direction2() {return direction_2;}
};

#endif //ONLINEGAME_GAME_INCLUDE_MY_CLIENT_H_
