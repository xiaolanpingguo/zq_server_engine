#include "game_server/db_server/internal_server_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/db_server/db_server.h"

#include "game_common/txt_table_reader.h"


namespace zq{



struct tagResAwardItem
{
	int iItemType = 0;
	int iItemId = 0;
	int iItemNum = 0;
};

struct tagResSevenDayLogin
{

	int iDay = 0; //第几天
	int iLimitType = 0; //限制类型
	int iLimitParam = 0; //限制类型参数
	std::vector<tagResAwardItem> vecAwardItem; //登录基础奖励
};

class CTableSevenDayLogin
{
public:

	int Init(const char* pszFileName)
	{
		DBCFile dbcFile;
		if (!dbcFile.OpenFromTXT(pszFileName) || dbcFile.GetRecordsNum() == 0)
		{
			return -1;
		}

		int iRowNum = dbcFile.GetRecordsNum();
		for (int i = 0; i < iRowNum; i++)
		{
			tagResSevenDayLogin* pUnit = new tagResSevenDayLogin;
			pUnit->iDay = dbcFile.Search_Name(i, "day")->iValue;
			pUnit->iLimitType = dbcFile.Search_Name(i, "limit_type")->iValue;
			pUnit->iLimitParam = dbcFile.Search_Name(i, "limit_param")->iValue;
			for (int j = 0; j < 4; ++j)
			{
				tagResAwardItem stAwardItem;
				stAwardItem.iItemType = dbcFile.Search_Name(i, "award_type", j)->iValue;
				stAwardItem.iItemId = dbcFile.Search_Name(i, "award_id", j)->iValue;
				stAwardItem.iItemNum = dbcFile.Search_Name(i, "award_num", j)->iValue;
				if (stAwardItem.iItemType <= 0 || stAwardItem.iItemNum <= 0)
				{
					continue;
				}

				pUnit->vecAwardItem.push_back(stAwardItem);
			}
			for (int j = 0; j < 3; ++j)
			{
				tagResAwardItem stAwardItem;
				stAwardItem.iItemType = dbcFile.Search_Name(i, "award_type_ex", j)->iValue;
				stAwardItem.iItemId = dbcFile.Search_Name(i, "award_id_ex", j)->iValue;
				stAwardItem.iItemNum = dbcFile.Search_Name(i, "award_num_ex", j)->iValue;
				if (stAwardItem.iItemType <= 0 || stAwardItem.iItemNum <= 0)
				{
					continue;
				}

				pUnit->vecAwardItem.push_back(stAwardItem);
			}

			m_mapContainer[pUnit->iDay] = pUnit;
		}

	}

	std::map<int, tagResSevenDayLogin*> m_mapContainer;
};


InternalServerModule::InternalServerModule(DBServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

InternalServerModule::~InternalServerModule()
{

}

bool InternalServerModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&InternalServerModule::onS2SHeatBeatReq>(this, S2S::MSG_ID_HEARTBEAT);

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().internalIp, m_thisServer->getConfig().internalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&InternalServerModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&InternalServerModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&InternalServerModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	CTableSevenDayLogin aa;
	aa.Init("assets/table_cfg/table_seven_day_login.txt");

	return true;
}

bool InternalServerModule::finalize()
{
	return true;
}

void InternalServerModule::onClientConnected(TcpConnectionPtr connection)
{	
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void InternalServerModule::onS2SHeatBeatReq(TcpConnectionPtr connection, const S2S::S2SHeartBeat& msg)
{

}


}

