//
// Created by ROG on 10.05.2025.
//

#ifndef ONLINEGAME_GAME_INCLUDE_CLIENT_INTERFACE_H_
#define ONLINEGAME_GAME_INCLUDE_CLIENT_INTERFACE_H_

#include "LoadBalancing-cpp/inc/Listener.h"
#include <iostream>

class ClientInterface : public ExitGames::LoadBalancing::Listener
{
 public:
  ClientInterface() = default;
  ~ClientInterface() override = default;

  // Callback: connexion réussie
  void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override;

  // Callback: déconnexion
  void disconnectReturn() override;

  // Callback: message reçu
  void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

  // Callback: réponse à une opération
  void operationReturn(const ExitGames::Photon::OperationResponse& operationResponse);
};


#endif //ONLINEGAME_GAME_INCLUDE_CLIENT_INTERFACE_H_
