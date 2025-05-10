
#include "network.h"
#include "LoadBalancing-cpp/inc/ConnectOptions.h"


static const ExitGames::Common::JString appID = L"75e8e7c9-aa68-43c5-b7f3-f6b4ef9f3849";
static const ExitGames::Common::JString appVersion = L"1.0";

static std::unique_ptr<ExitGames::LoadBalancing::Client> loadBalancingClient_;

void NetworkManager::Begin(ClientInterface *client,
                           const ExitGames::LoadBalancing::ClientConstructOptions &clientConstructOptions) {
// connect() is asynchronous -
// the actual result arrives in the Listener::connectReturn()
// or the Listener::connectionErrorReturn() callback
  loadBalancingClient_ =
      std::make_unique<ExitGames::LoadBalancing::Client>(*client, appID, appVersion, clientConstructOptions);
  ExitGames::LoadBalancing::ConnectOptions connectOptions_{};
  if (!loadBalancingClient_->connect(connectOptions_)) {
    std::cerr << "Could not connect. \n";
  }
}

void NetworkManager::Tick() {
  loadBalancingClient_->service();
}

void NetworkManager::End() {
  loadBalancingClient_->disconnect();
}

ExitGames::LoadBalancing::Client &NetworkManager::GetLoadBalancingClient() {
  return *loadBalancingClient_;
}
