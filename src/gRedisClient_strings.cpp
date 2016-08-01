/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <hiredis/hiredis.h>
#include <sstream>
#include "gRedisClient.h"

namespace gRedis
{

bool RedisClient::psetex( const Key& key,  int milliseconds, const Value& value) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_bool("PSETEX %s %d %s", key.c_str(), milliseconds, value.c_str());
}

bool RedisClient::append( const Key& key,  const Value& value) {
    VDATA vCmdData;
    vCmdData.push_back("APPEND");
    vCmdData.push_back(key);
    vCmdData.push_back(value);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_status(vCmdData);
}

bool RedisClient::set( const Key& key,  const Value& value) {
    VDATA vCmdData;
    vCmdData.push_back("SET");
    vCmdData.push_back(key);
    vCmdData.push_back(value);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_status(vCmdData);
}

bool RedisClient::set(const Key& key, const char *value, int len, int second) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    if (0==second) {
        return command_bool("set %s %b", key.c_str(), value, len);
    } else {
        return command_bool("set %s %b EX %d", key.c_str(), value, len, second);
    }
}

bool RedisClient::setbit(const Key& key,  int offset, int64_t newbitValue, int64_t oldbitValue) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(oldbitValue, "SETBIT %s %d %lld", key.c_str(), offset, newbitValue);
}

bool RedisClient::get(const Key& key,  string& value) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_string(value, "GET %s", key.c_str());
}

bool RedisClient::getbit(const Key& key, int& offset, int& bit ) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    int64_t intval = 0;
    bool bRet = command_integer(intval, "GETBIT %s %d", key.c_str(), offset);
    bit = (int)intval;
    return bRet;
}

bool RedisClient::getrange(const Key& key,  int start, int end, string& out) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    return command_string(out, "GETRANGE %s %d %d", key.c_str(), start, end);
}

bool RedisClient::getset(const Key& key,  const string& newValue, string& oldValue) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_string(oldValue, "GETSET %s %s", key.c_str(), newValue.c_str());
}

/*
bool RedisClient::mget(const DBIArray &vdbi,   const KEYS &  keys, ReplyData& vDdata) {
    bool bRet = false;
    size_t n = vdbi.size();
    if (n!=keys.size()) {
        return bRet;
    }
    
    DataItem item;
    DBIArray::const_iterator iter_dbi = vdbi.begin();
    KEYS::const_iterator iter_key = keys.begin();
    for (;iter_key!=keys.end();++iter_key, ++iter_dbi) {
        const RedisIndex& redisIndex = *iter_dbi;
        RedisIndex::SetRole(SLAVE);
        const string &key = *iter_key;
        if (key.length()>0) {
            bool ret = command_string(*iter_dbi, item.str, "GET %s", key.c_str());
            if (!ret) {
                item.type = REDIS_REPLY_NIL;
                item.str  = "";
            } else {
                item.type = REDIS_REPLY_STRING;
                bRet = true;
            }
            vDdata.push_back(item);
        }
    }
    return bRet;
}

bool RedisClient::mset(const DBIArray& vdbi, const VDATA& vData) {
    DBIArray::const_iterator iter_dbi = vdbi.begin();
    VDATA::const_iterator iter_data = vData.begin();
    for (; iter_data != vData.end(); iter_dbi++) {
        const string &key = (*iter_data++);
        const string &value = (*iter_data++);
        const RedisIndex& redisIndex = *iter_dbi;
        RedisIndex::SetRole(SLAVE);
        command_status("SET %s %s", key.c_str(), value.c_str());
    }
    return true;
}
*/

bool RedisClient::setex( const Key& key,  int seconds, const Value& value) {
    VDATA vCmdData;

    vCmdData.push_back("SETEX");
    vCmdData.push_back(key);
    vCmdData.push_back(toString(seconds));
    vCmdData.push_back(value);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_bool(vCmdData);
}

bool RedisClient::setnx(const Key& key,  const Value& value) {
    VDATA vCmdData;
    vCmdData.push_back("SETNX");
    vCmdData.push_back(key);
    vCmdData.push_back(value);

    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return commandargv_bool(vCmdData);
}

bool RedisClient::setrange(const Key& key,  int offset, const Value& value, int& length) {
    int64_t intval = 0;
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    bool bRet = command_integer(intval, "setrange %s %d %s", key.c_str(), offset, value.c_str());
    length = (int)intval;
    return bRet;
}

bool RedisClient::strlen(const Key& key, int& length) {
    int64_t intval = 0;
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    bool bRet = command_integer(intval, "STRLEN %s", key.c_str());
    length = (int)intval;
    return bRet;
}

bool RedisClient::incr(const Key& key, int64_t& result) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(result, "INCR %s", key.c_str());
}

bool RedisClient::incrby(const Key& key, int by, int64_t& result) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(result, "INCRBY %s %d", key.c_str(), by);
}

bool RedisClient::bitcount(const Key& key, int& count, int start, int end) {
    int64_t intval = 0;
    bool bRet = false;
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    if ( (start!=0)||(end!=0) ) {
        bRet = command_integer(intval, "bitcount %s %d %d", key.c_str(), start, end);
    } else {
        bRet =  command_integer(intval, "bitcount %s", key.c_str());
    }
    count = (int)intval;
    return bRet;
}

//// 这个实现有问题
//bool RedisClient::bitop(const string& destkey, const BITOP operation, const KEYS& keys, int& lenght) {
//    static const char *op_cmd[4]= {"AND","OR","XOR","NOT"};
//    VDATA vCmdData;
//    int64_t intval = 0;
//    vCmdData.push_back("bitop");
//    vCmdData.push_back(op_cmd[operation]);
//    vCmdData.push_back(destkey);
//    addparam(vCmdData, keys);
//    RedisIndex::SetRole(MASTER);
//    bool bRet = commandargv_integer(vCmdData, intval);
//    lenght = (int)intval;
//    return bRet;
//}

bool RedisClient::bitpos(const Key& key, int bit, int64_t& pos, int start, int end) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(SLAVE);
    if ( (start!=0)||(end!=0) ) {
        return command_integer(pos, "BITPOS %s %d %d %d", key.c_str(), bit, start, end);
    } 
    return command_integer(pos, "BITPOS %s %d", key.c_str(), bit);
}

bool RedisClient::decr(const Key& key, int64_t& result) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(result,"decr %s", key.c_str());
}

bool RedisClient::decrby(const Key& key, int by, int64_t& result) {
    RedisIndex::SetKey(key);
    RedisIndex::SetRole(MASTER);
    return command_integer(result, "decrby %s %d", key.c_str(), by);
}

}
