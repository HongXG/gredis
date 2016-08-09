/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_DBSLICE_H_
#define _GREDIS_DBSLICE_H_

#include "gRedisComm.h"
#include "gRedisConn.h"
#include "gRedisLock.h"

namespace gRedis
{

enum RedisStatus {
    REDISDB_UNCONN,
    REDISDB_WORKING,
    REDISDB_DEAD
};

class RedisSlice {
public:
	RedisSlice();
	virtual ~RedisSlice();

    // 连到到一个REDIS服务节点
    bool Connect(const RedisNode redisNode);
	void Connect();

    RedisConn* GetConn();
    void FreeConn(RedisConn *redisConn);
    void CloseConn();
    void Ping();
    RedisStatus GetRedisStatus() const;

    Node GetNode() const;

private:
    std::list<RedisConn*> mRedisConnPool;
    RedisLock             mRedisLock;

    RedisStatus           mRedisStatus;        // redis DB status
    RedisNode             mRedisNode;
};

}

#endif /* _GREDIS_DBSLICE_H_ */
