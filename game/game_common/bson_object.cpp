#include "game_common/bson_object.h"


namespace zq{


bool BsonObject::convertFromRawBson(const bson_t* rawBson)
{
	if (NULL == rawBson)
	{
		return false;
	}

	bson_iter_t iter_bson;

	if (!bson_iter_init(&iter_bson, rawBson))
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
				appendValue(key, "null");
			}
			break;
			case BSON_TYPE_INT32:
			{
				int iValue = bson_iter_int32(&iter_bson);
				appendValue(key, iValue);
			}
			break;
			case BSON_TYPE_INT64:
			{
				long long llValue = bson_iter_int64(&iter_bson);
				appendValue(key, llValue);
			}
			break;
			case BSON_TYPE_DOUBLE:
			{
				double fValuie = bson_iter_double(&iter_bson);
				appendValue(key, fValuie);
			}
			break;
			case BSON_TYPE_UTF8:
			{
				uint32_t iLen;
				const char* pszBuff = bson_iter_utf8(&iter_bson, &iLen);
				if (NULL == pszBuff || iLen == 0)
				{
					appendValue(key, "");
				}
				else
				{
					appendValue(key, pszBuff);
				}
			}
			break;
			case BSON_TYPE_BOOL:
			{
				bool b = bson_iter_bool(&iter_bson);
				appendValue(key, b);
			}
			break;
			case BSON_TYPE_OID:
			{
				const bson_oid_t* oid = bson_iter_oid(&iter_bson);
				char szOid[25];
				bson_oid_to_string(oid, szOid);

				appendValue(key, szOid);
			}
			break;
			case BSON_TYPE_BINARY:
			{
				const uint8_t* binary = NULL;
				bson_subtype_t subtype = BSON_SUBTYPE_BINARY;
				uint32_t binary_len = 0;
				bson_iter_binary(&iter_bson, &subtype, &binary_len, &binary);
				if (NULL == binary || binary_len == 0)
				{
					LOG_ERROR(s_logCategory,"Binary Error\n");
					return false;
				}

				appendValue(key, (const char*)binary, (int)binary_len);
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

bool BsonObject::convertToRawBson(bson_t* rawBson)
{
	bool bRet = false;
	for (auto iter = m_mapBsonData.begin(); iter != m_mapBsonData.end(); iter++)
	{
		BsonData* pBsonObj = &iter->second;

		BsonDataType iType = pBsonObj->getType();
		switch (iType)
		{
			case BsonDataType::INT32:
			{
				bRet = bson_append_int32(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), pBsonObj->getValueInt32());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Int32 Falied\n");
					return false;
				}
			}
			break;
			case BsonDataType::INT64:
			{
				bRet = bson_append_int64(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), pBsonObj->getValueInt64());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Int64 Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::DOUBLE:
			{
				bRet = bson_append_double(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), pBsonObj->getValueDouble());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Double Faied\n");
					return false;
				}
			}
			break;
			case BsonDataType::BOOL:
			{
				bRet = bson_append_bool(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), pBsonObj->getValueBool());
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append Bool Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::STRING:
			{
				bRet = bson_append_utf8(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), pBsonObj->getValueString().c_str(), pBsonObj->getValueString().length());
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
				bson_oid_init_from_string(&stOid, pBsonObj->getValueString().c_str());

				bRet = bson_append_oid(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), &stOid);
				if (!bRet)
				{
					LOG_ERROR(s_logCategory, "Append OidObj Failed\n");
					return false;
				}
			}
			break;
			case BsonDataType::BIN:
			{
				bRet = bson_append_binary(rawBson, pBsonObj->getKey().c_str(), pBsonObj->getKey().length(), BSON_SUBTYPE_BINARY, (const uint8_t*)pBsonObj->getValueString().c_str(), (uint32_t)pBsonObj->getValueString().size());
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


}