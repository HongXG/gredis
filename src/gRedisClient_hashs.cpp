/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <sstream>
#include <stdlib.h>
#include "gRedisClient.h"
#include "gRedisPool.h"
#include "gRedisReply.h"

namespace gRedis
{

bool RedisClient::hdel(const Key& key, const string& field, int64_t& count) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(count, "HDEL %s %s", key.c_str(), field.c_str());
}

bool RedisClient::hdel(const Key& key, const Keys& vfiled, int64_t& count) {
    RedisIndex::SetKey(key);
    VDATA vCmdData;
    vCmdData.push_back("HDEL");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vfiled);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, count);
}

bool RedisClient::hexist( const Key& key, const string& field){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_bool("HEXISTS %s %s", key.c_str(), field.c_str());
}

bool RedisClient::hget(const Key& key, const string& field, string& value) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_string(value, "HGET %s %s", key.c_str(), field.c_str());
}

bool  RedisClient::hgetall(const Key& key, ArrayReply& array){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_array(array, "HGETALL %s", key.c_str());
}

bool RedisClient::hincrby(const Key& key, const string& field, int64_t increment, int64_t& num ) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(num, "HINCRBY %s %s %lld", key.c_str(),field.c_str(), increment);
}

bool RedisClient::hincrbyfloat(const Key& key, const string& field, float increment, float& value ) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    bool bRet = false;

    RedisReply reply = command("HINCRBYFLOAT %s %s %f", key.c_str(), field.c_str(), increment);
    if (reply.CheckReply()) {
    	redisReply*& pReply = reply;
        value = atof(pReply->str);
        bRet = true;
    }

    return bRet;
}

bool RedisClient::hkeys(const Key& key, Keys& keys){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_list(keys, "HKEYS %s", key.c_str());
}

bool RedisClient::hlen(const Key& key, int64_t& count){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_integer(count, "HLEN %s", key.c_str());
}

bool RedisClient::hmget(const Key& key, const Keys& field, ArrayReply& array){
    RedisIndex::SetKey(key);
    VDATA vCmdData;
    vCmdData.push_back("HMGET");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, field);
    RedisIndex::SetRole(SLAVE);
    return commandargv_array(vCmdData, array);
}

bool RedisClient::hmset(const Key& key, const VDATA& vData){
    RedisIndex::SetKey(key);
    VDATA vCmdData;
    vCmdData.push_back("HMSET");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vData);
    RedisIndex::SetRole(MASTER);
    return commandargv_status(vCmdData);
}

bool RedisClient::hset(const Key& key, const string& field, const Value& value, int64_t& retval){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(retval, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool RedisClient::hsetnx(const Key& key, const string& field, const Value& value){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("HSETNX %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool RedisClient::hvals(const Key& key, Values& values) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_list(values, "HVALS %s", key.c_str());
}

}
