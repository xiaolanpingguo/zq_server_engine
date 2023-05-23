#pragma once


#include "game_server/zone_server/player/i_player_module.h"


namespace zq {


struct SDKAccountInfo
{
	std::string sdkUserId;
	std::string sdkToken;
	uint32_t channelId;
};

class TcpConnection;
class Player
{
public:
	Player();
	~Player();

	template <typename T, typename... Args>
	bool registerModule(Args&&... args)
	{
		IPlayerModule* m = new T(std::forward<Args>(args)...);
		auto it = m_playerModules.emplace(std::string(T::getName()), m);
		ASSERT(it.second, "register {} multiple times!", T::getName());
		return it.second;
	}

	template <typename T>
	T* getModule()
	{
		auto it = m_playerModules.find(std::string(T::getName()));
		if (it == m_playerModules.end())
		{
			ASSERT(false, "can't get module:{}!", T::getName());
			return nullptr;
		}
		return (T*)(it->second);
	}

	bool loadFromDB(const S2S::DBPlayerData& dbPlayerData);
	bool saveToDB(S2S::DBPlayerData& dbPlayerData);

	std::shared_ptr<TcpConnection> getConnection();
	void setConnection(std::shared_ptr<TcpConnection> conn);

	void setProfileId(const std::string& v) { m_profileId = v; }
	const std::string& getProfileId() const { return m_profileId; }

	void setSDKAccountInfo(const SDKAccountInfo& v) { m_sdkAccountInfo = v; }
	const SDKAccountInfo& getSDKAccountInfo() const { return m_sdkAccountInfo; }

public:

	std::shared_ptr<TcpConnection> m_conection;
	std::unordered_map<std::string, IPlayerModule*> m_playerModules;

	std::string m_profileId;
	SDKAccountInfo m_sdkAccountInfo;

	constexpr static std::string_view s_logCategory = "Player";
};


}
