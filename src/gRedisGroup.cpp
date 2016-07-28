/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <assert.h>

#include "gRedisGroup.h"

namespace gRedis
{

RedisGroup::RedisGroup()
{
}

RedisGroup::~RedisGroup()
{
	ClosePool();
}

bool RedisGroup::ConnectRedis(const RedisNode redisNode)
{
    if( (redisNode.mNode.mHost.empty())
        || (redisNode.mRole>SLAVE)
        || (redisNode.mPoolsize>MAX_REDIS_CONNPOOL_SIZE)) {
            return false;
    }

	RedisSlice* pRedisSlice = mMapRedisSlice[redisNode.mNode];
	if (NULL == pRedisSlice) {
		pRedisSlice = new RedisSlice();
		if (NULL == pRedisSlice) {
			return false;
		}
		mMapRedisSlice[redisNode.mNode] = pRedisSlice;
		if (MASTER==redisNode.mRole && mNodeMaster.empty()) {
			mNodeMaster = redisNode.mNode;
		}
		return pRedisSlice->ConnectRedisNodes(redisNode);
	}
	return false;
}

void RedisGroup::ClosePool()
{
	for (std::map<Node, RedisSlice*>::iterator pIter = mMapRedisSlice.begin();
			mMapRedisSlice.end() != pIter;
			++pIter)
	{
		if (NULL != pIter->second) {
			pIter->second->CloseConnPool();
			DELETE(pIter->second);
		}
	}
	mMapRedisSlice.clear();
}

void RedisGroup::KeepAlive()
{
	for (std::map<Node, RedisSlice*>::iterator pIter = mMapRedisSlice.begin();
			mMapRedisSlice.end() != pIter;
			++pIter)
	{
		if (NULL != pIter->second) {
			pIter->second->ConnPoolPing();
		}
	}
}

RedisStatus RedisGroup::GetRedisStatus(const Node node)
{
	RedisSlice* const pRedisSlice = GetRedisSlice(node);
	if (NULL != pRedisSlice) {
		return pRedisSlice->GetRedisStatus();
	}
    return REDISDB_UNCONN;
}

void RedisGroup::FreeConn(RedisConn* redisConn)
{
	RedisSlice* const pRedisSlice = GetRedisSlice(redisConn->getRedisNode().mNode);
	assert(NULL != pRedisSlice);
	if (NULL != pRedisSlice) {
		return pRedisSlice->FreeConn(redisConn);
	}
}

RedisConn* RedisGroup::GetConn(const Node node, const Role role)
{
    if (SLAVE!=role && MASTER!=role)
    {
        return NULL;
    }
	RedisSlice* const pRedisSlice = GetRedisSlice(node);
	if (NULL != pRedisSlice) {
		return pRedisSlice->GetConn();
	}
	return NULL;
}

RedisConn* RedisGroup::GetConn(const unsigned int slot, const Role role)
{
    if (SLAVE!=role && MASTER!=role)
    {
        return NULL;
    }
    if (1 == mMapRedisSlice.size()) {
    	mNodeMaster = mMapRedisSlice.begin()->first;
    	return mMapRedisSlice[mNodeMaster]->GetConn();
    } else if (1 < mMapRedisSlice.size()) {
		if (MASTER==role && !mNodeMaster.empty()) {
	    	return mMapRedisSlice[mNodeMaster]->GetConn();
		}
		std::map<Node, RedisSlice*>::iterator pIter = mMapRedisSlice.begin();
		unsigned int uiIndex = slot % mMapRedisSlice.size();
		while (0 < uiIndex) {
			--uiIndex; ++pIter;
		}
		if (NULL != pIter->second) {
			return pIter->second->GetConn();
		}
	}
	return NULL;
}

RedisSlice* RedisGroup::GetRedisSlice(const Node node)
{
	std::map<Node, RedisSlice*>::iterator pIter = mMapRedisSlice.find(node);
	if (mMapRedisSlice.end() != pIter) {
		mNodeMaster = node;
		return pIter->second;
	}
	return NULL;
}

}
