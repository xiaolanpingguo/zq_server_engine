#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_client.hpp"
#include "game_common/proto_bason_handler.h"


namespace zq {


class LoginServer;
class LoginToDBModule : public IModule, public ProtoBasonHandler
{
	INIT_MODULE_NAME(LoginToDBModule);

public:
	LoginToDBModule(LoginServer* thisServer);
	~LoginToDBModule();

public:
	bool init() override;
	bool finalize() override;

	bool sendToDBServer(uint16_t msgId, const char* data, uint32_t len) override;

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SMongoInsertRes(TcpConnectionPtr connection, const S2S::MongoInsertRes& res);
	void onS2SMongoRemoveRes(TcpConnectionPtr connection, const S2S::MongoRemoveRes& res);
	void onS2SMongoSaveRes(TcpConnectionPtr connection, const S2S::MongoSaveRes& res);
	void onS2SMongoFindRes(TcpConnectionPtr connection, const S2S::MongoFindRes& res);
	void onS2SMongoBatchFindRes(TcpConnectionPtr connection, const S2S::MongoBatchFindRes& res);

private:

	LoginServer* m_thisServer;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;
	TcpConnectionPtr m_connection;

	constexpr static std::string_view s_logCategory = "LoginToDBModule";
};

}
