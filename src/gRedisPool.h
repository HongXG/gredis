/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_POOL_H_
#define _GREDIS_POOL_H_

#include <hiredis/hiredis.h>
#include <string.h>
#include <string>
#include <list>
#include <map>

#include "gRedisGroup.h"
#include "gRedisLock.h"
#include "gRedisNode.h"
#include "gRedisSlot.h"
#include "gRedisComm.h"

namespace gRedis
{

class RedisPool : protected RedisSlot
{
public:
    RedisPool();
    virtual ~RedisPool();

    unsigned int getMemberSize(const GroupID groupID);
    bool Connect(const RedisNode redisNode);
    
    RedisConn* GetConnection(const Node node, const unsigned int slot);
    RedisConn* GetConnection(const Key& key, const Role role=MASTER);
    RedisConn* GetConnection(const GroupID groupID);
    void FreeConnection(RedisConn* redisConn);
    
    void Keepalive();
    void Release();

private:
    RedisLock                     mRedisLock;
    std::map<GroupID, RedisGroup> mMapRedisGroup;
};

}

#endif
