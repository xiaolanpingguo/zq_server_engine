#pragma once


#include "game_server/world_server/player/i_player_module.h"


namespace zq {


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

	bool loadFromDB(const DB::DBPlayer& playerDBData);
	bool saveToDB(DB::DBPlayer& playerDBData);

public:

	std::unordered_map<std::string, IPlayerModule*> m_playerModules;

	constexpr static std::string_view s_logCategory = "Player";
};


}
