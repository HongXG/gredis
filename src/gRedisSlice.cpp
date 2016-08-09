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

bool RedisSlice::Connect(const RedisNode redisNode)
{
	mRedisNode.clear();

    if ((redisNode.mNode.empty())
        || (redisNode.mPoolsize > MAX_REDIS_CONNPOOL_SIZE)) {
        return false;
    }

    try {
    	mRedisNode = redisNode;

		GREDISLOCK(mRedisLock);
		for (unsigned int i = 0; i < mRedisNode.mPoolsize; ++i) {
			RedisConn *pRedisconn = new RedisConn;
			if (NULL == pRedisconn) {
		        return false;
			}

			pRedisconn->Init(mRedisNode);
			if (pRedisconn->Connect()) {
				mRedisStatus = REDISDB_WORKING;
			}
			mRedisConnPool.push_back(pRedisconn);
		}

		return true;

    } catch (...) {
        return false;
    }
}

void RedisSlice::Connect()
{
	GREDISLOCK(mRedisLock);
	RedisStatus redisStatus = REDISDB_DEAD;
	for (std::list<RedisConn*>::iterator pIter = mRedisConnPool.begin();
			mRedisConnPool.end() != pIter;
			++pIter)
	{
		RedisConn*& pRedisConn = *pIter;
		if (NULL != pRedisConn)
		{
			if (pRedisConn->getConnstatus() || pRedisConn->Connect())
			{
				redisStatus = REDISDB_WORKING;
			}
		}
	}
	mRedisStatus = redisStatus;
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

void RedisSlice::FreeConn(RedisConn* redisConn)
{
    if (NULL != redisConn) {
    	GREDISLOCK(mRedisLock);
    	mRedisConnPool.push_back(redisConn);
    }
}

void RedisSlice::CloseConn()
{
	GREDISLOCK(mRedisLock);
    mRedisStatus = REDISDB_DEAD;
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
}

void RedisSlice::Ping()
{
	GREDISLOCK(mRedisLock);
	RedisStatus redisStatus = REDISDB_DEAD;
	for (std::list<RedisConn*>::iterator pIter = mRedisConnPool.begin();
			mRedisConnPool.end() != pIter;
			++pIter)
	{
		RedisConn*& pRedisConn = *pIter;
		if (NULL != pRedisConn)
		{
			if (pRedisConn->Ping() || pRedisConn->Connect())
			{
				redisStatus = REDISDB_WORKING;
			}
		}
	}
	mRedisStatus = redisStatus;
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
