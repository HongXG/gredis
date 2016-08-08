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

bool RedisClient::zadd(const Key& key,   const Values& vValues, int64_t& count){
    VDATA vCmdData;
    vCmdData.push_back("ZADD");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vValues);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, count);
}

bool RedisClient::zscrad(   const Key& key, int64_t& count){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_integer(count, "ZSCRAD %s", key.c_str());
}

bool RedisClient::zincrby(const Key& key, const double &increment, const string& member, string& value ) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_string(value, "ZINCRBY %s %f %s", key.c_str(), increment, member.c_str());
}

bool RedisClient::zrange(const Key& key, int start, int end, Values& vValues, bool withscore) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    if (withscore) {
        return command_list(vValues, "ZRANGE %s %d %d %s", key.c_str(), start, end, "WITHSCORES");
    }
    return command_list(vValues, "ZRANGE %s %d %d", key.c_str(), start, end);
}

bool RedisClient::zrank(const Key& key, const string& member, int64_t &rank) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(rank, "ZRANK %s %s", key.c_str(), member.c_str());
}

bool RedisClient::zrem(      const Key& key, const Values& vmembers, int64_t &count) {
    VDATA vCmdData;
    vCmdData.push_back("ZREM");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vmembers);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, count);
}

bool RedisClient::zremrangebyrank(const Key& key, int start, int stop, int64_t& count) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(count, "ZREMRANGEBYRANK %s %d %d", key.c_str(), start, stop);
}

bool RedisClient::zrevrange(const Key& key, int start, int end, Values& vValues, bool withscore) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    if (withscore) {
        return command_list(vValues, "ZREVRANGE %s %d %d %s", key.c_str(), start, end, "WITHSCORES");
    }
    return command_list(vValues, "ZREVRANGE %s %d %d", key.c_str(), start, end);
}

 bool RedisClient::zrevrank(const Key& key, const string &member, int64_t& rank){
     if (0==key.length()) {
         return false;
     }
     RedisIndex::SetKey(key);
     RedisIndex::SetRole(SLAVE);
     return command_integer(rank, "ZREVRANK %s %s", key.c_str(), member.c_str());
 }

 bool RedisClient::zscore(const Key& key, const string &member, string& score ){
     if (0==key.length()) {
         return false;
     }
     RedisIndex::SetKey(key);
     RedisIndex::SetRole(SLAVE);
     return command_string(score, "ZSCORE %s %s", key.c_str(), member.c_str());
 }

}
