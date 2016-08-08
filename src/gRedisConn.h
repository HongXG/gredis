/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_CONN_H_
#define _GREDIS_CONN_H_

#include <string>
#include <list>
#include <vector>

#include <hiredis/hiredis.h>

#include "gRedisNode.h"

using namespace std;

namespace gRedis
{

class RedisConn
{
public:
    RedisConn();
    virtual ~RedisConn();

    void Init(const RedisNode redisNode);

    bool Connect();
    void Close();
    bool Ping();

    RedisNode     getRedisNode() const  { return mRedisNode; }
    bool          getConnstatus() const { return mConnStatus; }

public:
    /**
     * 调用redisCommand
     */
    redisReply* command(const char *format, ...);
    redisReply* commandv(const char *format, va_list ap);
    /**
     * 调用redisCommandArgv
     */
    redisReply* commandArgv(int argc, const char **argv, const size_t *argvlen);

private:
    bool auth();
    static void Close(redisContext*& context);
    static redisContext* ConnectWithTimeout(const RedisNode redisNode);

private:
    // redis connector context
    redisContext* mCtx;
    RedisNode     mRedisNode;     // redis node info
    bool          mConnStatus;    // redis connection status
};

}

#endif /* _GREDIS_CONN_H_ */
