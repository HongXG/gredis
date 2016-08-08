/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisConn.h"
#include "gRedisReply.h"

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

void RedisConn::Init(const RedisNode redisNode)
{
    mRedisNode = redisNode;
}

bool RedisConn::auth()
{
    if (!mRedisNode.mPasswd.empty()) {
        RedisReply reply = command("AUTH %s", mRedisNode.mPasswd.c_str());
        return RedisReply::CheckReply(reply);
    }

    return true;
}

bool RedisConn::Connect()
{
    Close();
    mCtx = ConnectWithTimeout(mRedisNode);
    if (NULL!=mCtx && auth()) {
    	mConnStatus = true;
    	return true;
    } else {
        Close();
    	return false;
    }
}

void RedisConn::Close()
{
	mConnStatus = false;
	Close(mCtx);
}

bool RedisConn::Ping()
{
    RedisReply reply = command("PING");
    mConnStatus = !reply.empty();
    return mConnStatus;
}

void RedisConn::Close(redisContext*& context)
{
    if (NULL != context) {
        redisFree(context);
        context = NULL;
    }
}

redisContext * RedisConn::ConnectWithTimeout(const RedisNode redisNode)
{
    struct timeval timeoutVal;
    timeoutVal.tv_sec = redisNode.mTimeout;
    timeoutVal.tv_usec = 0;

    redisContext* context = redisConnectWithTimeout(redisNode.mNode.mHost.c_str(), redisNode.mNode.mPort, timeoutVal);
    if (NULL!=context && 0==context->err) {
    	return context;
    }
    Close(context);
    return context;
}

/**
 * 调用redisCommand，并自动释放RedisConn链接
 */
redisReply* RedisConn::command(const char *format, ...)
{
    va_list ap;
    va_start(ap,format);
    RedisReply reply(commandv(format, ap), false);
    va_end(ap);
    return reply;
}

redisReply* RedisConn::commandv(const char *format, va_list ap)
{
	redisReply* reply = NULL;
	if (NULL != mCtx)
	{
	    reply = static_cast<redisReply *>(redisvCommand(mCtx, format, ap));
	    if (NULL == reply) {
	    	Close();
	    }
	}
    return reply;
}

/**
 * 调用redisCommandArgv，并自动释放RedisConn链接
 */
redisReply* RedisConn::commandArgv(int argc, const char **argv, const size_t *argvlen)
{
	redisReply* reply = NULL;
	if (NULL != mCtx)
	{
	    reply = static_cast<redisReply *>(redisCommandArgv(mCtx, argc, argv, argvlen));
	    if (NULL == reply) {
	    	Close();
	    }
	}
    return reply;
}

}
