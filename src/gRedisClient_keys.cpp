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

bool  RedisClient::del(   const string& key) {
    RedisIndex::SetKey(key);
    if (0==key.length()) {
        return false;
    }

    RedisIndex::SetRole(MASTER);
    return command_bool("DEL %s", key.c_str());
}

/*
bool  RedisClient::del(const DBIArray& vdbi,    const KEYS &  vkey, int64_t& count) {
    count = 0;
    if (vdbi.size()!=vkey.size()) {
        return false;
    }
    DBIArray::const_iterator iter_dbi = vdbi.begin();
    KEYS::const_iterator iter_key     = vkey.begin();
    for(;iter_key!=vkey.end();++iter_key, ++iter_dbi) {
        const RedisIndex &redisIndex = (*iter_dbi);
        const string &key     = (*iter_key);
        if (del(redisIndex, key)) {
            count++;
        }
    }
    return true;
}
*/

bool RedisClient::exists(const string& key) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("EXISTS %s", key.c_str());
}

bool RedisClient::expire(const Key& key, unsigned int second) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    int64_t ret = -1;
    if (!command_integer(ret, "EXPIRE %s %u", key.c_str(), second)) {
        return false;
    }

    if (1==ret) {
        return true;
    } else {
        SetErrMessage("expire return %ld ", ret);
        return false;
    }
}

bool RedisClient::expireat(const Key& key, unsigned int timestamp) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("EXPIREAT %s %u", key.c_str(), timestamp);
}

bool RedisClient::persist(const string& key) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("PERSIST %s %u", key.c_str());
}

bool RedisClient::pexpire(const Key& key, unsigned int milliseconds) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    return command_bool("PEXPIRE %s %u", key.c_str(), milliseconds);
}

bool RedisClient::pexpireat(const Key& key, unsigned int millisecondstimestamp) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("PEXPIREAT %s %u", key.c_str(), millisecondstimestamp);
}

bool RedisClient::pttl(const Key& key, int64_t &milliseconds) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(milliseconds, "PTTL %s", key.c_str());
}

bool RedisClient::ttl(const Key& key, int64_t &seconds) {
    if (0==key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_integer(seconds, "TTL %s", key.c_str());
}

bool RedisClient::randomkey(Key& key){
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_string(key, "RANDOMKEY");
}




bool RedisClient::sort(const Key& key, ArrayReply& array, const char* by,
						LIMIT *limit /*= NULL*/, bool alpha /*= false*/, const FILEDS* get /*= NULL*/,
						const SORTODER order /*= ASC*/, const char* destination )
{
    static const char *sort_order[3] = { "ASC", "DESC" };
    if (0 == key.length()) {
        return false;
    }
    RedisIndex::SetKey(key);
       

    VDATA vCmdData;
    vCmdData.push_back("sort");
    vCmdData.push_back(key);
    if (NULL != by) {
        vCmdData.push_back("by");
        vCmdData.push_back(by);
    }

    if (NULL != limit) {
        vCmdData.push_back("LIMIT");
        vCmdData.push_back(toString(limit->offset));
        vCmdData.push_back(toString(limit->count));
    }
    if (alpha) {
        vCmdData.push_back("ALPHA");
    }

    if (NULL != get) {
        for (FILEDS::const_iterator iter = get->begin(); iter != get->end(); ++iter) {
            vCmdData.push_back("get");
            vCmdData.push_back(*iter);
        }
    }

    vCmdData.push_back(sort_order[order]);
    if (destination) {
        vCmdData.push_back(destination);
    }
    RedisIndex::SetRole(MASTER);
    return commandargv_array(vCmdData, array);
}

}
