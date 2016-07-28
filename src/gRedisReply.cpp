/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisReply.h"

#include <string.h>
#include <stdlib.h>

namespace gRedis
{

RedisReply::RedisReply(redisReply* const reply, const bool autoFree/*=true*/)
	: mAutoFree(autoFree)
	, mRedisReply(reply){
}

RedisReply::~RedisReply() {
	if (mAutoFree) {
		FreeReply(mRedisReply);
	}
}

//拷贝构造函数
RedisReply::RedisReply(const RedisReply& reply)
{
	mAutoFree = reply.mAutoFree;
	mRedisReply = reply.mRedisReply;
}

RedisReply& RedisReply::operator=(redisReply* const reply)
{
	FreeReply(mRedisReply);
	mRedisReply = reply;
	return *this;
}

RedisReply::operator redisReply*&()
{
	return mRedisReply;
}

void RedisReply::FreeReply()
{
	FreeReply(mRedisReply);
}

void RedisReply::FreeReply(redisReply*& reply) {
    if (NULL != reply) {
        freeReplyObject(reply);
        reply = NULL;
    }
}

bool RedisReply::CheckReply(const redisReply* const reply) {
    if(NULL==reply) {
        return false;
    }
    switch(reply->type){
    case REDIS_REPLY_STRING:{
            return true;
        }
    case REDIS_REPLY_ARRAY:{
        	return true;
        }
    case REDIS_REPLY_INTEGER:{
            return true;
        }
    case REDIS_REPLY_NIL:{
            return false;
        }
    case REDIS_REPLY_STATUS:{
            return (strcasecmp(reply->str,"OK") == 0)?true:false;
        }
    case REDIS_REPLY_ERROR:{
            return false;
        }
    default:{
            return false;
        }
    }

    return false;
}

/**
 * 判断是否需要切换到集群其它节点上去进行操作
 */
bool RedisReply::ExistCluster(const redisReply* reply, Node& node, unsigned int& slot) {
    if (!RedisReply::CheckReply(reply) && NULL!=reply) {
    	if (REDIS_REPLY_ERROR==reply->type && (0==strncasecmp(reply->str,"MOVED",5) || 0==strncasecmp(reply->str,"ASK",3))) {
    		char szBuffer[512] = {'\0'};
    		memmove(szBuffer, reply->str, sizeof(szBuffer)-1);

			char *p = szBuffer, *s = NULL;
			s = strchr(p, ' ');      /* MOVED[S]3999 127.0.0.1:6381 */
			if (NULL == s) return false;
			p = strchr(s+1,' ');    /* MOVED[S]3999[P]127.0.0.1:6381 */
			if (NULL == p) return false;
			*p = '\0';
			slot = atoi(s+1);
			if (0 >= slot) return false;
			s = strrchr(p+1,':');    /* MOVED 3999[P]127.0.0.1[S]6381 */
			if (NULL == s) return false;
			*s = '\0';
			node.mHost = (p+1);
			if (7 > node.mHost.length()) return false;
			node.mPort = atoi(s+1);
			if (0 >= node.mPort) return false;

			return true;
    	}
    }
    return false;
}

}
