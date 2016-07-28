/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_CMD_H_
#define _GREDIS_CMD_H_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <hiredis/hiredis.h>

#include "gRedisComm.h"
#include "gRedisConn.h"
#include "gRedisPool.h"
#include "gRedisIndex.h"

typedef std::vector<Key>        KEYS;
typedef KEYS                    FILEDS;
typedef std::vector<Value>      VALUES;

typedef std::set<std::string>        SETDATA;

typedef struct _DATA_ITEM_{
    int         type;
    std::string str;

    _DATA_ITEM_ & operator=(const _DATA_ITEM_ &data) {
        type = data.type;
        str  = data.str;
        return *this;
    }
}DataItem;
typedef std::vector<DataItem>            ReplyData;
typedef ReplyData                        ArrayReply;
typedef std::map<std::string, double>    ZSETDATA;
//typedef std::vector<RedisIndex>          DBIArray;


typedef enum _BIT_OP_{
    AND = 0,
    OR  = 1,
    XOR = 2,
    NOT = 3
}BITOP;

typedef enum _LIST_MODEL_{
    BEFORE = 0,
    AFTER  = 1
}LMODEL;


typedef enum _SORT_ORDER_{
    ASC = 0,
    DESC = 1
}SORTODER;

typedef struct _SORT_LIMIT_
{
	int offset;
	int count;
}LIMIT;

namespace gRedis
{

class RedisCmd : public RedisPool, public RedisIndex
{
public:
	RedisCmd();
	virtual ~RedisCmd();

    bool ConnectRedisGroup(const RedisNode* arrayRedisNode, const unsigned int arraySize);

protected:
    bool command_bool(                      const char* cmd, ...);
    bool command_status(                    const char* cmd, ...);
    bool command_integer(int64_t &intval,   const char* cmd, ...);
    bool command_string( std::string &data, const char* cmd, ...);
    bool command_list(   VALUES &vValue,    const char* cmd, ...);
    bool command_array(  ArrayReply& array, const char* cmd, ...);

protected:
    bool commandargv_bool(  const VDATA& vData);
    bool commandargv_status(const VDATA& vData);
    bool commandargv_array( const VDATA& vDataIn, ArrayReply& array);
    bool commandargv_array( const VDATA& vDataIn, VALUES& array);
    bool commandargv_integer(const VDATA& vDataIn, int64_t& retval);

protected:
    redisReply* command(const char *format, ...);
    redisReply* commandv(const char *format, va_list ap);
    redisReply* commandArgv(int argc, const char **argv, const size_t *argvlen);

    /**
     * 调用redisCommand，并自动释放RedisConn链接
     */
    redisReply* commandv(RedisConn* const pRedisConn, const char *format, va_list ap);
    /**
     * 调用redisCommandArgv，并自动释放RedisConn链接
     */
    redisReply* commandArgv(RedisConn* const pRedisConn, int argc, const char **argv, const size_t *argvlen);
};

}

#endif /* _GREDIS_CMD_H_ */
