/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include <sstream>
#include "gRedisClient.h"

namespace gRedis
{

bool RedisClient::lindex( const Key& key, int64_t index, Value& value){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_string(value, "LINDEX %s %lld", key.c_str(), index);
}

bool RedisClient::linsert( const Key& key, const LMODEL mod, const string& pivot, const Value& value, int64_t& retval){
    static const char *lmodel[2]= {"BEFORE","AFTER"};
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(retval, "LINSERT %s %s %s %s", key.c_str(), lmodel[mod], pivot.c_str(), value.c_str());
}

bool RedisClient::llen( const Key& key, int64_t& retval){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_integer(retval, "LLEN %s", key.c_str());
}

bool RedisClient::lpop( const Key& key, string& value){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_string(value, "LPOP %s", key.c_str());
}

bool RedisClient::lpush( const Key& key, const Values& vValue, int64_t& length){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
	VDATA vCmdData;
    vCmdData.push_back("LPUSH");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vValue);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, length);
}

bool RedisClient::lrange( const Key& key, int64_t start, int64_t end, ArrayReply& array){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
	return command_array(array, "LRANGE %s %lld %lld", key.c_str(), start, end);
}

bool RedisClient::lrem(const Key& key, int count, const Value& value, int64_t num){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(num, "LREM %s %d %s", key.c_str(), count, value.c_str());
}

bool RedisClient::lset(const Key& key, int index, const Value& value){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_status("LSET %s %d %s", key.c_str(), index, value.c_str());
}

bool RedisClient::ltrim(const Key& key, int start, int end){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_status("LTRIM %s %d %d", key.c_str(), start, end);
}

bool RedisClient::rpop( const Key& key, string& value){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_string(value, "RPOP %s", key.c_str());
}

bool RedisClient::rpoplpush( const string& key_src, const string& key_dest, string& value){
    if ((0 == key_src.length()) || (0==key_dest.length())) {
        return false;
    }
    RedisIndex::SetKey(key_dest);
    RedisIndex::SetRole(MASTER);
    return command_string(value, "RPOPLPUSH %s %s", key_src.c_str(), key_dest.c_str());
}

bool RedisClient::rpush( const Key& key, const Values& vValue, int64_t& length){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
	VDATA vCmdData;
    vCmdData.push_back("RPUSH");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vValue);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, length);
}

bool RedisClient::rpushx(const Key& key, const Value& value, int64_t& length){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(length, "RPUSHX %s %s", key.c_str(), value.c_str());
}

}
