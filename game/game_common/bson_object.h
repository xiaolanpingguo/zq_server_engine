#pragma once


#include "common/log.hpp"
#include <stdint.h>
#include <variant>
#include <unordered_map>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>

#include "protocol/s2s/db_mongo_proxy.pb.h"
#include <bson/bson.h>


namespace zq {


constexpr static bool DEFAULT_BOOL = false;
constexpr static int32_t DEFAULT_INT32 = 0;
constexpr static int64_t DEFAULT_INT64 = 0;
constexpr static double DEFAULT_DOUBLE = 0.0;
const static std::string DEFAULT_STRING = "";

enum BsonDataType : int
{
	UNKNOWN, 
	BOOL,
	INT32, 
	INT64, 
	DOUBLE, 
	STRING, 
	BIN,
	OID,
};


class BsonData
{
	using VariantDataT = std::variant<bool, int32_t, int64_t, double, std::string>;

public:
	BsonData(const std::string& key = "") :
			m_key(key), m_type(BsonDataType::UNKNOWN) {}
	~BsonData() = default;

	void setBool(bool v)
	{
		m_type = BsonDataType::BOOL;
		m_data = v;
	}

	void setInt32(int32_t v)
	{
		m_type = BsonDataType::INT32;
		m_data = v;
	}

	void setInt64(int64_t v)
	{
		m_type = BsonDataType::INT64;
		m_data = v;
	}

	void setDouble(double v)
	{
		m_type = BsonDataType::DOUBLE;
		m_data = v;
	}

	void setString(const std::string& v )
	{
		m_type = BsonDataType::STRING;
		m_data = v;
	}

	void setString(const char* v)
	{
		m_type = BsonDataType::STRING;
		m_data = (std::string)v;
	}

	void setBin(const char* v, size_t len)
	{
		m_type = BsonDataType::BIN;
		m_data = std::string(v, len);
	}

	void setOid(const char* v)
	{
		m_type = BsonDataType::OID;
		m_data = std::string(v);
	}

	const bool& getBool() const
	{
		return std::get<bool>(m_data);
	}

	const int32_t& getInt32() const
	{
		return std::get<int32_t>(m_data);
	}

	const int64_t& getInt64()  const
	{
		return std::get<int64_t>(m_data);
	}

	const double& getDouble() const
	{
		return std::get<double>(m_data);
	}

	const std::string& getString()	const
	{
		return std::get<std::string>(m_data);
	}

	const std::string& getBin() const
	{
		return std::get<std::string>(m_data);
	}

	const std::string& getOid() const
	{
		return std::get<std::string>(m_data);
	}

	std::string debugPrint()
	{
		std::ostringstream ss;
		ss << "---------" << std::endl;
		ss << "type: " << (int)m_type << std::endl;
		ss << "key: " << m_key << std::endl;
		std::visit([&ss](auto&& arg)
		{
			ss << "data: " << arg << std::endl;
		}, m_data);
		ss << "---------" << std::endl;

		return ss.str();
	}

	BsonDataType getType() const { return m_type; }
	const std::string& getKey() const { return m_key; }

private:

	BsonDataType m_type;
	std::string m_key;
	VariantDataT m_data;
};


class BsonObject;
using BsonObjectPtr = std::shared_ptr<BsonObject>;
using BsonObjectUniquePtr = std::unique_ptr<BsonObject>;
class BsonObject
{
public:
	BsonObject(const std::string& key = "") :
			m_key(key) {}
	~BsonObject() = default;

	void appendBool(const std::string& strKey, bool v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setBool(v);
			addBsonData(data);
		}
	}

	void appendInt32(const std::string& strKey, int32_t v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setInt32(v);
			addBsonData(data);
		}
	}

	void appendInt64(const std::string& strKey, int64_t v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setInt64(v);
			addBsonData(data);
		}
	}

	void appendDouble(const std::string& strKey, double v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setDouble(v);
			addBsonData(data);
		}
	}

	void appendString(const std::string& strKey, const std::string& v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setString(v);
			addBsonData(data);
		}
	}

	void appendString(const std::string& strKey, const char* v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setString(v);
			addBsonData(data);
		}
	}

	void appendBin(const std::string& strKey, const char* v, size_t len)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setBin(v, len);
			addBsonData(data);
		}
	}

	void appendOid(const std::string& strKey, const char* v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setOid(v);
			addBsonData(data);
		}
	}

	const bool& getBool(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_BOOL;
		}

		return pData->getBool();
	}

	const int32_t& getInt32(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_INT32;
		}

		return pData->getInt32();
	}

	const int64_t& getInt64(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_INT64;
		}

		return pData->getInt64();
	}

	const double& getDouble(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_DOUBLE;
		}

		return pData->getDouble();
	}

	const std::string& getString(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_STRING;
		}

		return pData->getString();
	}

	const std::string& getBin(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_STRING;
		}

		return pData->getBin();
	}

	BsonData* getBsonData(const std::string& strKey)
	{
		auto iter = m_mapBsonData.find(strKey);
		if (iter != m_mapBsonData.end())
		{
			return &iter->second;
		}

		return nullptr;
	}

	std::string debugPrint()
	{
		std::ostringstream ss;
		for (auto& ele : m_mapBsonData)
		{
			ss << ele.second.debugPrint() << std::endl;
		}

		return ss.str();
	}

	void clear()
	{
		m_key.clear();
		m_mapBsonData.clear();
	}

	bool empty()
	{
		return m_mapBsonData.empty();
	}

	const std::string& getKey() const { return m_key; }
	void setKey(const std::string& strKey) { m_key = strKey; }
	const std::unordered_map<std::string, BsonData>& getAllBsonData() const { return m_mapBsonData; }

	bool convertFromRawBson(const bson_t& rawBson);
	bool convertToRawBson(bson_t& rawBson);
	bool convertToProtoBson(S2S::ProtoBsonObj& protoBson);
	bool convertFromProtoBson(const S2S::ProtoBsonObj& protoBson);

private:

	bool addBsonData(const BsonData& data)
	{
		std::string key = data.getKey();
		return m_mapBsonData.insert(std::make_pair(key, data)).second;
	}

private:

	std::string m_key;
	std::unordered_map<std::string, BsonData> m_mapBsonData;

	constexpr static std::string_view s_logCategory = "BsonObject";
};


}
