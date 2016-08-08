/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisSlice.h"

#include "gRedisLock.h"

namespace gRedis
{

RedisSlice::RedisSlice()
{
    mRedisStatus = REDISDB_UNCONN;
}

RedisSlice::~RedisSlice()
{

}

bool RedisSlice::ConnectRedisNodes(const RedisNode redisNode)
{
	mRedisNode.clear();

    if ((redisNode.mNode.empty())
        || (redisNode.mPoolsize > MAX_REDIS_CONNPOOL_SIZE)) {
        return false;
    }

    try {
    	mRedisNode = redisNode;

		GREDISLOCK(mRedisLock);
		for (unsigned int i = 0; i < redisNode.mPoolsize; ++i) {
			RedisConn *pRedisconn = new RedisConn;
			if (NULL == pRedisconn) {
				continue;
			}

			pRedisconn->Init(redisNode);
			if (pRedisconn->Connect()) {
				mRedisConnPool.push_back(pRedisconn);
				mRedisStatus = REDISDB_WORKING;
			} else {
				DELETE(pRedisconn);
			}
		}

		return true;

    } catch (...) {
        return false;
    }

    return false;
}

RedisConn* RedisSlice::GetConn()
{
    RedisConn *pRedisConn = NULL;
    GREDISLOCK(mRedisLock);
    if (!mRedisConnPool.empty()) {
        pRedisConn = mRedisConnPool.front();
        mRedisConnPool.pop_front();
    } else {
    	mRedisStatus = REDISDB_DEAD;
    }
    return pRedisConn;
}

void RedisSlice::FreeConn(RedisConn *redisConn)
{
    if (NULL != redisConn) {
    	GREDISLOCK(mRedisLock);
    	mRedisConnPool.push_back(redisConn);
    }
}

void RedisSlice::CloseConn()
{
	GREDISLOCK(mRedisLock);
	for (std::list<RedisConn*>::iterator pIter = mRedisConnPool.begin();
			mRedisConnPool.end() != pIter;
			++pIter)
	{
		RedisConn*& pRedisConn = *pIter;
		if (NULL != pRedisConn)
		{
			pRedisConn->Close();
			DELETE(pRedisConn);
		}
	}
	mRedisConnPool.clear();
    mRedisStatus = REDISDB_DEAD;
}

void RedisSlice::Ping()
{
	GREDISLOCK(mRedisLock);
	for (std::list<RedisConn*>::iterator pIter = mRedisConnPool.begin();
			mRedisConnPool.end() != pIter;
			++pIter)
	{
		RedisConn*& pRedisConn = *pIter;
		if (NULL!=pRedisConn && !pRedisConn->Ping())
		{
			pRedisConn->Connect();
		}
	}
}

RedisStatus RedisSlice::GetRedisStatus() const
{
    return mRedisStatus;
}

Node RedisSlice::GetNode() const
{
	return mRedisNode.mNode;
}

}
