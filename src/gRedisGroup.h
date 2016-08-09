/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_CACHE_H_
#define _GREDIS_CACHE_H_

#include <map>
#include <vector>

#include "gRedisSlice.h"

namespace gRedis
{

class RedisGroup {
public:
	RedisGroup();
	virtual ~RedisGroup();

	bool ConnectRedis(const RedisNode redisNode);

    RedisConn* GetConn(const Node node);
	RedisConn* GetConn(const unsigned int slot, const Role role);
    void FreeConn(RedisConn* redisConn);
    void ClosePool();
    void KeepAlive();
    RedisStatus GetRedisStatus(const Node node);
    RedisSlice* GetRedisSlice(const Node node);

private:
    Node  mNodeMaster;
    std::map<Node, RedisSlice*>           mMapRedisSlice;
};

}

#endif /* _GREDIS_CACHE_H_ */
