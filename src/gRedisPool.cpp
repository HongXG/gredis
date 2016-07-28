/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <hiredis/hiredis.h>
#include <time.h>

#include "gRedisPool.h"
#include "gRedisSlot.h"

namespace gRedis
{

RedisPool::RedisPool() {
}

RedisPool::~RedisPool() {

}

void RedisPool::Keepalive() {
	for (std::map<GroupID, RedisGroup>::iterator pIter = mMapRedisGroup.begin();
			mMapRedisGroup.end() != pIter;
			++pIter) {
        pIter->second.KeepAlive();
	}
}

bool RedisPool::ConnectRedisDB(const RedisNode redisNode){
    return mMapRedisGroup[redisNode.mGroupID].ConnectRedis(redisNode);
}

void RedisPool::Release() {
	for (std::map<GroupID, RedisGroup>::iterator pIter = mMapRedisGroup.begin();
			mMapRedisGroup.end() != pIter;
			++pIter) {
		pIter->second.ClosePool();
	}
	mMapRedisGroup.clear();
	RedisSlot::Clear();
}

RedisConn* RedisPool::GetConnection(const Node node, const unsigned int slot){
	const GroupID tempGroupID = RedisSlot::GetSlotGroup(slot);
	std::map<GroupID, RedisGroup>::iterator pIter = mMapRedisGroup.find(tempGroupID);
	if (mMapRedisGroup.end() != pIter) {
        RedisSlice* const redisDBSlice = pIter->second.GetRedisSlice(node);
        if (NULL != redisDBSlice) {
        	if (node == redisDBSlice->GetNode()) {
        		RedisConn* const pRedisConn = redisDBSlice->GetConn();
                if (NULL != pRedisConn) {
                	return pRedisConn;
                }
        	}
        }
	} else {
		for (std::map<GroupID, RedisGroup>::iterator pIter = mMapRedisGroup.begin();
				mMapRedisGroup.end() != pIter;
				++pIter)
		{
	        RedisGroup& redisGroup = pIter->second;
	        RedisSlice* const redisDBSlice = redisGroup.GetRedisSlice(node);
	        if (NULL != redisDBSlice) {
	        	if (node == redisDBSlice->GetNode()) {
	        		RedisConn* const pRedisConn = redisDBSlice->GetConn();
	                if (NULL != pRedisConn) {
	                	RedisSlot::SetSlotGroup(slot, pIter->first);
	                	return pRedisConn;
	                }
	        	}
	        }
		}
	}

    return NULL;
}

RedisConn* RedisPool::GetConnection(const Key& key, const Role role/*=MASTER*/)
{
	if (0 >= mMapRedisGroup.size()) {
		return NULL;
	}

	Role roleTmp = role;
	GroupID groupID = mMapRedisGroup.begin()->first;
	const unsigned int uiSlot = RedisSlot::keyHashSlot(key.c_str(), key.length());
	if (1 < mMapRedisGroup.size())
	{
		roleTmp = MASTER;
		const GroupID tempGroupID = RedisSlot::GetSlotGroup(uiSlot);
		std::map<GroupID, RedisGroup>::iterator pIter = mMapRedisGroup.find(tempGroupID);
		if (mMapRedisGroup.end() != pIter) {
			groupID = pIter->first;
		}
	}
    return mMapRedisGroup[groupID].GetConn(uiSlot, roleTmp);
}

void RedisPool::FreeConnection(RedisConn* redisConn){
    if (NULL != redisConn) {
    	mMapRedisGroup[redisConn->getRedisNode().mGroupID].FreeConn(redisConn);
    }
}

}
