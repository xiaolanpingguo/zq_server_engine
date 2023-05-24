#include "test.h"
#include "network/tcp_client.hpp"
#include "protocol/c2s/c2s_login.pb.h"
#include "protocol/s2s/s2s_global_data.pb.h"
#include "game_common/message_helper.hpp"
using namespace zq;


class TestClient
{
public:
	TestClient()
	{
		using namespace std::placeholders;
		m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_ioContext, m_ip, m_port);
		m_tcpClient->setConnectToServerCb(std::bind(&TestClient::onConnectToServerCallback, this, _1, _2));
		m_tcpClient->setDisconnectFromServerCb(std::bind(&TestClient::onDisconnectedFromServer, this, _1));
		m_tcpClient->setDataReceivedCb(std::bind(&TestClient::onDataReceivedFromServer, this, _1, _2, _3, _4));
		m_tcpClient->asyncConnect();

		MessageHelper& messageHelper = MessageHelper::getInstance();
		messageHelper.registerHandler<&TestClient::onC2SLoginRes>(this, C2S::MSG_ID_LOGIN_RES);
	}
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
	{
		LOG_INFO(s_logCategory, "connect to server success!");
		m_connection = connection;
		login();
	}

	void onDisconnectedFromServer(TcpConnectionPtr connection)
	{
		LOG_INFO(s_logCategory, "disconnected from server: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
	}

	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
	{
		MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
	}

	void run()
	{
		m_ioContext.run();
	}

	void login()
	{
		C2S::C2SLoginReq req;
		req.set_sdk_user_id("abcd");
		req.set_sdk_token("edf");
		req.set_channel_id(0);
		std::string strReq;
		if (!req.SerializeToString(&strReq))
		{
			LOG_ERROR(s_logCategory, "SerializeToString error!");
			return;
		}

		m_connection->sendData(C2S::MSG_ID_LOGIN_REQ, strReq.data(), (uint32_t)strReq.size());
	}
	void onC2SLoginRes(TcpConnectionPtr connection, const C2S::C2SLoginRes& res)
	{
		int errorCode = res.error_code();
		std::string ip = res.ip();
		int port = res.port();
		std::string profileId = res.profile_id();

		LOG_INFO(s_logCategory, "onC2SLoginRes, errorCode:{},zoneToken:{},ip:{},port:{},profileId:{}",
				errorCode, ip, port, profileId);
	}

private:
	asio::io_context m_ioContext;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;
	TcpConnectionPtr m_connection;
	std::string m_ip = "127.0.0.1";
	uint16_t m_port = 22001;
};

bool testLogin()
{
	TestClient clinet;
	clinet.run();

	return true;
}