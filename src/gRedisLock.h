/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */


#ifndef _GREDIS_LOCK_H_
#define _GREDIS_LOCK_H_

#ifdef WIN32
#include <Windows.h>
#else
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#endif

namespace gRedis
{

class RedisLock {
public:
    RedisLock();
    virtual ~RedisLock();
    void Enter();
    void Leave();

private:
#ifdef WIN32
    CRITICAL_SECTION    mSection;
#else
    pthread_mutex_t     mMutex;
#endif
};

class RedisAutoLock {
public:
    RedisAutoLock(RedisLock& lock);
    virtual ~RedisAutoLock();
private:
    RedisLock& mlock;
};

}

#define GREDISLOCK(T) gRedis::RedisAutoLock lock(T)

#endif /* _GREDIS_LOCK_H_ */

