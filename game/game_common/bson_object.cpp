#include "game_common/bson_object.h"


namespace zq{


bool BsonObject::convertFromRawBson(const bson_t& rawBson)
{
	bson_iter_t iter_bson;

	if (!bson_iter_init(&iter_bson, &rawBson))
	{
		LOG_ERROR(s_logCategory, "Bson Iter Init Failed\n");
		return false;
	}

	while (bson_iter_next(&iter_bson))
	{
		bson_type_t type = bson_iter_type(&iter_bson);

		const char* key = bson_iter_key(&iter_bson);
		switch (type)
		{
			case BSON_TYPE_NULL:
			{
				appendString(key, "null");
			}
			break;
			case BSON_TYPE_INT32:
			{
				int iValue = bson_iter_int32(&iter_bson);
				appendInt32(key, iValue);
			}
			break;
			case BSON_TYPE_INT64:
			{
				long long llValue = bson_iter_int64(&iter_bson);
				appendInt64(key, llValue);
			}
			break;
			case BSON_TYPE_DOUBLE:
			{
				double fValuie = bson_iter_double(&iter_bson);
				appendDouble(key, fValuie);
			}
			break;
			case BSON_TYPE_UTF8:
			{
				uint32_t iLen;
				const char* pszBuff = bson_iter_utf8(&iter_bson, &iLen);
				if (NULL == pszBuff || iLen == 0)
				{
					appendString(key, "");
				}
				else
				{
					appendString(key, pszBuff);
				}
			}
			break;
			case BSON_TYPE_BOOL:
			{
				bool b = bson_iter_bool(&iter_bson);
				appendBool(key, b);
			}
			break;
			case BSON_TYPE_OID:
			{
				const bson_oid_t* oid = bson_iter_oid(&iter_bson);
				char szOid[25];
				bson_oid_to_string(oid, szOid);

				appendOid(key, szOid);
			}
			break;
			case BSON_TYPE_BINARY:
			{
				const uint8_t* binary = nullptr;
				bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
				uint32_t binary_len = 0;
				bson_iter_binary(&iter_bson, &subtype, &binary_len, &binary);
				if (nullptr == binary || binary_len == 0)
				{
					LOG_ERROR(s_logCategory,"Binary Error\n");
					return false;
				}

				appendBin(key, (const char*)binary, (int)binary_len);
			}
			break;

			default:
			{
				LOG_ERROR(s_logCategory, "UnSupprot ValueType:{}", (int)type);
				return false;
			}
			break;
		}
	}
	return true;
}

bool BsonObject::convertToRawBson(bson_t& rawBson)
{
	bool bRet = false;
	for (auto iter = m_mapBsonData.begin(); iter != m_mapBsonData.end(); iter++)
	{
		const BsonData& pBsonObj = iter->second;

		BsonDataType iType = pBsonObj.getType();
		switch (iType)
		{
			case BsonDataType::INT32:
			{
				bRet = bson_append_int32(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), pBsonObj.getInt32());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Int32 Falied\n");
					return false;
				}
			}
			break;
			case BsonDataType::INT64:
			{
				bRet = bson_append_int64(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), pBsonObj.getInt64());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Int64 Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::DOUBLE:
			{
				bRet = bson_append_double(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), pBsonObj.getDouble());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Double Faied\n");
					return false;
				}
			}
			break;
			case BsonDataType::BOOL:
			{
				bRet = bson_append_bool(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), pBsonObj.getBool());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Bool Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::STRING:
			{
				bRet = bson_append_utf8(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), pBsonObj.getString().c_str(), (int)pBsonObj.getString().length());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append uft8 Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::OID:
			{
				bson_oid_t stOid;
				bson_oid_init_from_string(&stOid, pBsonObj.getOid().c_str());

				bRet = bson_append_oid(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), &stOid);
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append OidObj Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::BIN:
			{
				bRet = bson_append_binary(&rawBson, pBsonObj.getKey().c_str(), (int)pBsonObj.getKey().length(), BSON_SUBTYPE_BINARY, (const uint8_t*)pBsonObj.getBin().data(), (uint32_t)pBsonObj.getBin().size());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append binary Failed\n");
					return false;
				}
			}
			break;
			default:
			{
				LOG_ERROR(s_logCategory, "UnSupported Data Type:{}\n", (int)iType);
				return false;
			}
			break;
		}
	}

	return true;
}

bool BsonObject::convertToProtoBson(S2S::ProtoBsonObj& protoBson)
{
	for (const auto& e : m_mapBsonData)
	{
		const BsonData& bsonData = e.second;
		int dataType = bsonData.getType();

		S2S::ProtoBsonData* protoData = protoBson.add_bson_data_list();
		protoData->set_data_type(dataType);
		protoData->set_key(bsonData.getKey());

		switch (dataType)
		{
			case BsonDataType::INT32:
			{
				protoData->set_var_int32(bsonData.getInt32());
			}
			break;
			case BsonDataType::INT64:
			{
				protoData->set_var_int64(bsonData.getInt64());
			}
			break;
			case BsonDataType::DOUBLE:
			{
				protoData->set_var_double(bsonData.getDouble());
			}
			break;
			case BsonDataType::BOOL:
			{
				protoData->set_var_bool(bsonData.getBool());
			}
			break;
			case BsonDataType::STRING:
			{
				protoData->set_var_string(bsonData.getString());
			}
			break;
			case BsonDataType::OID:
			{
				protoData->set_var_string(bsonData.getOid());
			}
			break;
			case BsonDataType::BIN:
			{
				protoData->set_var_bin(&bsonData.getBin(), bsonData.getBin().size());
			}
			break;
			default:
			{
				LOG_ERROR(s_logCategory, "UnSupport Type:{}\n", (int)dataType);
				return false;
			}
			break;
		}
	}
	return true;
}

bool BsonObject::convertFromProtoBson(const S2S::ProtoBsonObj& protoBson)
{
	for (int i = 0; i < (int)protoBson.bson_data_list_size(); i++)
	{
		const S2S::ProtoBsonData& protoBsonData = protoBson.bson_data_list(i);
		int dataType = protoBsonData.data_type();
		switch (dataType)
		{
			case BsonDataType::INT32:
			{
				appendInt32(protoBsonData.key(), protoBsonData.var_int32());
			}
			break;
			case BsonDataType::INT64:
			{
				appendInt64(protoBsonData.key(), protoBsonData.var_int64());
			}
			break;
			case BsonDataType::DOUBLE:
			{
				appendDouble(protoBsonData.key(), protoBsonData.var_double());
			}
			break;
			case BsonDataType::BOOL:
			{
				appendBool(protoBsonData.key(), protoBsonData.var_bool());
			}
			break;
			case BsonDataType::STRING:
			{
				appendString(protoBsonData.key(), protoBsonData.var_string());
			}
			break;
			case BsonDataType::OID:
			{
				appendOid(protoBsonData.key(), protoBsonData.var_string().c_str());
			}
			break;
			case BsonDataType::BIN:
			{
				appendBin(protoBsonData.key(), protoBsonData.var_bin().c_str(), protoBsonData.var_bin().size());
			}
			break;
			default:
			{
				LOG_ERROR(s_logCategory, "UnSupport Type:{}\n", (int)dataType);
				return false;
			}
			break;
		}
	}

	return true;
}

}