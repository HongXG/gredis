/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisClient.h"

namespace gRedis
{

bool RedisClient::sadd(   const Key& key, const VALUES& vValue, int64_t& count){
    VDATA vCmdData;
    vCmdData.push_back("SADD");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vValue);
    RedisIndex::SetRole(MASTER);
    return commandargv_integer(vCmdData, count);
}

bool RedisClient::scard(   const Key& key, int64_t& count){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetRole(SLAVE);
    return command_integer(count, "SCARD %s", key.c_str());
}

/*
bool RedisClient::sdiff(const DBIArray& vdbi,     const KEYS& vkey, VALUES& sValue){
	size_t size = vkey.size();
    if (0 == size) {
        return false;
    }
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    DBIArray::const_iterator iter_dbi = vdbi.begin();
    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key, ++iter_dbi, ++i) {
        const string &key = *iter_key;
        const RedisIndex &redisIndex = *iter_dbi;
        if (!smembers(redisIndex, key, setData[i])) {
        	DELETEARRAY(setData);
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1) {
        endpos = set_difference( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    DELETEARRAY(setData);
    return true;
}

bool RedisClient::sdiffstore(const KEY& destinationkey,  const DBIArray& vdbi, const KEYS& vkey, int64_t& count){
    VALUES sValue;
    if (!sdiff(vdbi, vkey, sValue)) {
       return false;
    }
    return sadd(redisIndex, destinationkey, sValue, count);
}

bool RedisClient::sinter(const DBIArray& vdbi, const KEYS& vkey, VALUES& sValue){
    size_t size = vkey.size();
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    DBIArray::const_iterator iter_dbi = vdbi.begin();
    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key, ++iter_dbi, ++i) {
        const string &key = *iter_key;
        const RedisIndex &redisIndex = *iter_dbi;
        if (!smembers(redisIndex, key, setData[i])) {
        	DELETEARRAY(setData);
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1){
        endpos = set_intersection( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    DELETEARRAY(setData);

    return true;
}

bool RedisClient::sinterstore(const RedisIndex& redisIndex, const KEY& destinationkey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count){
    VALUES sValue;
    if (!sinter(vdbi, vkey, sValue)) {
        return false;
    }
    return sadd(redisIndex, destinationkey, sValue, count);
}
*/

bool RedisClient::sismember(const Key& key,   const Value& member){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    return command_bool("SISMEMBER %s %s", key.c_str(), member.c_str());
}

bool RedisClient::smembers(const Key& key, VALUES& vValue){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_list(vValue, "SMEMBERS %s", key.c_str());
}

bool RedisClient::smove(const Key& srckey, const Key& deskey,  const Value& member){
    if (0==srckey.length()) {
        return false;
    }
    RedisIndex::SetKey(deskey);
    RedisIndex::SetRole(MASTER);
    return command_bool("SMOVE %s", srckey.c_str(), deskey.c_str(), member.c_str());
}

bool RedisClient::spop(const Key& key, Value& member){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_string(member, "SPOP %s", key.c_str());
}

bool RedisClient::srandmember(const Key& key, VALUES& members, int count){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    if (0==count) {
        return command_list(members, "SRANDMEMBER %s", key.c_str());
    }
    return command_list(members, "SRANDMEMBER %s %d", key.c_str(), count);
}

bool RedisClient::srem(const Key& key, const VALUES& vmembers, int64_t& count){
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    VDATA vCmdData;
    vCmdData.push_back("SREM");
    vCmdData.push_back(key);
    RedisComm::addParam(vCmdData, vmembers);
    return commandargv_integer(vCmdData, count);
}
/*

bool RedisClient::sunion(const DBIArray& vdbi,     const KEYS& vkey, VALUES& sValue){
    size_t size = vkey.size();
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    DBIArray::const_iterator iter_dbi = vdbi.begin();
    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key, ++iter_dbi, ++i) {
        const string &key = *iter_key;
        const RedisIndex &redisIndex = *iter_dbi;
        if (!smembers(redisIndex, key, setData[i])) {
        	DELETEARRAY(setData);
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1) {
            endpos = set_union( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    DELETEARRAY(setData);
    return true;
}

bool RedisClient::sunionstore(const Key& deskey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count){
    VALUES sValue;
    if (!redisIndex.mFlag) { SetRole(MASTER, true); }
    if (!sunion(vdbi, vkey, sValue)) {
        return false;
    }
    return sadd(redisIndex, deskey, sValue, count);
}
*/

}
