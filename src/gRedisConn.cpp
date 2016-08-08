/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisConn.h"

#include <string.h>

namespace gRedis
{

RedisConn::RedisConn()
{
    mCtx = NULL;
    mConnStatus = false;
}

RedisConn::~RedisConn()
{

}

redisContext * RedisConn::ConnectWithTimeout()
{
    struct timeval timeoutVal;
    timeoutVal.tv_sec = mRedisNode.mTimeout;
    timeoutVal.tv_usec = 0;

    redisContext *ctx = NULL;
    ctx = redisConnectWithTimeout(mRedisNode.mNode.mHost.c_str(), mRedisNode.mNode.mPort, timeoutVal);
    if (NULL == ctx || ctx->err) {
        if (NULL != ctx) {
            redisFree(ctx);
            ctx = NULL;
        } else {

        }
    }

    return ctx;
}

bool RedisConn::auth()
{
    bool bRet = false;
    if (0 == mRedisNode.mPasswd.length()) {
        bRet = true;
    } else {
        redisReply *reply = static_cast<redisReply *>(redisCommand(mCtx, "AUTH %s", mRedisNode.mPasswd.c_str()));
        if ((NULL == reply) || (strcasecmp(reply->str, "OK") != 0)) {
            bRet = false;
        } else {
            bRet = true;
        }
        freeReplyObject(reply);
    }

    return bRet;
}

bool RedisConn::Connect()
{
    bool bRet = false;
    if (NULL != mCtx) {
        redisFree(mCtx);
        mCtx = NULL;
    }

    mCtx = ConnectWithTimeout();
    if (NULL==mCtx) {
        bRet = false;
    } else {
        bRet = auth();
        mConnStatus = bRet;
    }

    return bRet;
}

bool RedisConn::Ping()
{
    redisReply *reply = static_cast<redisReply *>(redisCommand(mCtx, "PING"));
    bool bRet = (NULL != reply);
    mConnStatus = bRet;
    if(bRet)
    {
        freeReplyObject(reply);
    }
    return bRet;
}

void RedisConn::Init(const RedisNode redisNode)
{
    mRedisNode = redisNode;
}

}
