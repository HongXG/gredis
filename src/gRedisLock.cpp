/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisLock.h"

namespace gRedis
{

RedisLock::RedisLock() {
#ifdef WIN32
    InitializeCriticalSection(&mSection);
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int ret = pthread_mutex_init(&mMutex, &attr);
    if(ret != 0 ){
        printf("pthread_mutex_init error %d \n\r",ret);
    }
#endif
};

RedisLock::~RedisLock() {
#ifdef WIN32
    DeleteCriticalSection(&mSection);
#else
    pthread_mutex_destroy(&mMutex);
#endif
}

void RedisLock::Enter() {
#ifdef WIN32
    EnterCriticalSection(&mSection);
#else
    pthread_mutex_lock(&mMutex);
#endif
}

void RedisLock::Leave() {
#ifdef WIN32
    LeaveCriticalSection(&mSection);
#else
    pthread_mutex_unlock(&mMutex);
#endif
};

RedisAutoLock::RedisAutoLock(RedisLock& lock):mlock(lock) {
    mlock.Enter();
};
RedisAutoLock::~RedisAutoLock(){
    mlock.Leave();
}

}
