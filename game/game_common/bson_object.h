#pragma once


#include "common/log.hpp"
#include <stdint.h>
#include <variant>
#include <unordered_map>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>

#include <libbson-1.0/bson.h>


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

template <typename T>
struct DataTypeIdentity
{
};


#define REGISTER_TYPE(Type, Index)     inline constexpr BsonDataType dataTypeToId(DataTypeIdentity<Type>) noexcept { return Index; }

REGISTER_TYPE(bool, BsonDataType::BOOL)
REGISTER_TYPE(int8_t, BsonDataType::INT32)
REGISTER_TYPE(uint8_t, BsonDataType::INT32)
REGISTER_TYPE(int16_t, BsonDataType::INT32)
REGISTER_TYPE(uint16_t, BsonDataType::INT32)
REGISTER_TYPE(int32_t, BsonDataType::INT32)
REGISTER_TYPE(uint32_t, BsonDataType::INT32)
REGISTER_TYPE(int64_t, BsonDataType::INT64)
REGISTER_TYPE(uint64_t, BsonDataType::INT64)
REGISTER_TYPE(double, BsonDataType::DOUBLE)
//REGISTER_TYPE(double, BsonDataType::BIN)
//REGISTER_TYPE(double, BsonDataType::OID)

inline BsonDataType dataTypeToId(DataTypeIdentity<std::string>) noexcept { return BsonDataType::STRING; }



class BsonData
{
	using VariantDataT = std::variant<bool, int32_t, int64_t, double, std::string>;

public:
	BsonData(const std::string& key = "") :
			m_key(key), m_type(BsonDataType::UNKNOWN) {}
	~BsonData() = default;

	BsonData(const BsonData& that) :
			m_type(that.m_type), m_key(that.m_key), m_data(that.m_data) {}
	BsonData& operator=(BsonData const& that)
	{
		if (this != &that)
		{
			m_type = that.m_type;
			m_key = that.m_key;
			m_data = that.m_data;
		}

		return *this;
	}

	BsonData(BsonData&& that) :
			m_type(that.m_type), m_key(std::move(that.m_key)), m_data(std::move(that.m_data)) {}
	BsonData& operator=(BsonData&& that)
	{
		if (this != &that)
		{
			m_type = that.m_type;
			m_key = std::move(that.m_key);
			m_data = std::move(that.m_data);
		}

		return *this;
	}

	template<typename T>
	void setValue(const T& v)
	{
		using U = std::decay_t<T>;
		if constexpr (std::is_same_v<int8_t, U> || std::is_same_v<uint8_t, U> ||
				std::is_same_v<int16_t, U> || std::is_same_v<uint16_t, U> ||
				std::is_same_v<int32_t, U> || std::is_same_v<uint32_t, U>)
		{
			m_type = dataTypeToId(dataTypeToId<U>());
			m_data = (int32_t)v;
		}
		else
		{
			m_type = dataTypeToId(dataTypeToId<U>());
			m_data = (U)v;
		}
	}

	void setValue(const char* v)
	{
		m_type = BsonDataType::STRING;
		m_data = (std::string)v;
	}

	void setValue(const char* v, size_t len)
	{
		m_type = BsonDataType::BIN;
		m_data = std::string(v, len);
	}

	const bool& getValueBool() const
	{
		if (m_type != BsonDataType::BOOL)
		{
			return DEFAULT_BOOL;
		}

		return std::get<bool>(m_data);
	}

	const int32_t& getValueInt32() const
	{
		if (m_type != BsonDataType::INT32)
		{
			return DEFAULT_INT32;
		}

		return std::get<int32_t>(m_data);
	}

	const int64_t& getValueInt64()  const
	{
		if (m_type != BsonDataType::INT64)
		{
			return DEFAULT_INT64;
		}

		return std::get<int64_t>(m_data);
	}

	const double& getValueDouble() const
	{
		if (m_type != BsonDataType::DOUBLE)
		{
			return DEFAULT_DOUBLE;
		}

		return std::get<double>(m_data);
	}

	const std::string& getValueString()	const
	{
		if (m_type != BsonDataType::STRING)
		{
			return DEFAULT_STRING;
		}

		return std::get<std::string>(m_data);
	}

	const std::string& getValueBin() const
	{
		if (m_type != BsonDataType::BIN)
		{
			return DEFAULT_STRING;
		}

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


class BsonBatchFindData
{
public:
	BsonBatchFindData(const std::string& key = "", size_t size = 0) :
			m_key(key) { m_data.resize(size); }
	~BsonBatchFindData() = default;

	BsonBatchFindData(const BsonBatchFindData& that) :
			m_key(that.m_key), m_data(that.m_data) {}
	BsonBatchFindData& operator=(BsonBatchFindData const& that)
	{
		if (this != &that)
		{
			m_key = that.m_key;
			m_data = that.m_data;
		}

		return *this;
	}

	BsonBatchFindData(BsonBatchFindData&& that) :
			m_key(std::move(that.m_key)), m_data(std::move(that.m_data)) {}
	BsonBatchFindData& operator=(BsonBatchFindData&& that)
	{
		if (this != &that)
		{
			m_key = std::move(that.m_key);
			m_data = std::move(that.m_data);
		}

		return *this;
	}

	template<typename T>
	void appendValue(const T& v)
	{
		BsonData data;
		data.setValue(v);
		m_data.emplace_back(std::move(data));
	}

	void appendValue(const char* v)
	{
		BsonData data;
		data.setValue(v);
		m_data.emplace_back(std::move(data));
	}

	void appendValue(BsonData&& data)
	{
		m_data.emplace_back(std::move(data));
	}

	template<typename T>
	void setValue(const T& v, size_t pos)
	{
		if (pos < 0 || pos >= m_data.size())
		{
			return;
		}

		m_data[pos].setValue(v);
	}

	BsonData* getValue(size_t pos)
	{
		if (pos < 0 || pos >= m_data.size())
		{
			return nullptr;
		}

		return &m_data[pos];
	}

	const std::string& getKey() const { return m_key; }
	const std::vector<BsonData>& getData() const { return m_data; }

private:

	std::string m_key;
	std::vector<BsonData> m_data;
};


class BsonObject;
using BsonObjectPtr = std::shared_ptr<BsonObject>;
class BsonObject
{
public:
	BsonObject(const std::string& key = "") :
			m_key(key) {}
	~BsonObject() = default;

	BsonObject(const BsonObject& that) { copy(that); }
	BsonObject& operator=(BsonObject const& that)
	{
		copy(that);
		return *this;
	}
	BsonObject(BsonObject&& that) { move(std::move(that)); }
	BsonObject& operator=(BsonObject&& that)
	{
		move(std::move(that));
		return *this;
	}

	void copy(const BsonObject& that)
	{
		if (this != &that)
		{
			clear();
			m_key = that.m_key;
			m_mapBsonData = that.m_mapBsonData;
		}
	}

	void move(BsonObject&& that)
	{
		if (this != &that)
		{
			clear();
			m_key = std::move(that.m_key);
			m_mapBsonData = std::move(that.m_mapBsonData);
		}
	}

	template<typename T>
	void appendValue(const std::string& strKey, const T& v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setValue(v);
			addBsonData(std::move(data));
		}
	}

	void appendValue(const std::string& strKey, const char* v)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setValue(v);
			addBsonData(std::move(data));
		}
	}

	void appendValue(const std::string& strKey, const char* v, size_t len)
	{
		if (getBsonData(strKey) == nullptr)
		{
			BsonData data(strKey);
			data.setValue(v, len);
			addBsonData(std::move(data));
		}
	}

	const bool& getValueBool(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_BOOL;
		}

		return pData->getValueBool();
	}

	const int32_t& getValueInt32(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_INT32;
		}

		return pData->getValueInt32();
	}

	const int64_t& getValueInt64(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_INT64;
		}

		return pData->getValueInt64();
	}

	const double& getValueDouble(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_DOUBLE;
		}

		return pData->getValueDouble();
	}

	const std::string& getValueString(const std::string& key)
	{
		auto pData = getBsonData(key);
		if (nullptr == pData)
		{
			return DEFAULT_STRING;
		}

		return pData->getValueString();
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

	const std::string& getKey() const { return m_key; }
	void setKey(const std::string& strKey) { m_key = strKey; }

	bool convertFromRawBson(const bson_t* rawBson);
	bool convertToRawBson(bson_t* rawBson);

private:

	bool addBsonData(BsonData&& prop)
	{
		std::string key = prop.getKey();
		return m_mapBsonData.insert(std::make_pair(key, std::move(prop))).second;
	}

private:

	std::string m_key;
	std::unordered_map<std::string, BsonData> m_mapBsonData;

	constexpr static std::string_view s_logCategory = "BsonObject";
};



}
