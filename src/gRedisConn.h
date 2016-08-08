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
    bool ReConnect();
    bool Ping();

    redisContext* getCtx() const        { return mCtx; }
    RedisNode     getRedisNode() const  { return mRedisNode; }
    bool          getConnstatus() const { return mConnStatus; }

private:
    bool auth();
    redisContext* ConnectWithTimeout();

private:
    // redis connector context
    redisContext* mCtx;
    RedisNode     mRedisNode;     // redis node info
    bool          mConnStatus;    // redis connection status
};

}

#endif /* _GREDIS_CONN_H_ */
