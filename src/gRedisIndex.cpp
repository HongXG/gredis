/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisIndex.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace gRedis
{

RedisIndex::RedisIndex() {
    memset(mStrerr, 0, sizeof(mStrerr));
    mRole = MASTER;
    mFlag = false;
}

RedisIndex::RedisIndex(const std::string key) {
	mKey = key;
    memset(mStrerr, 0, sizeof(mStrerr));
    mRole = MASTER;
    mFlag = false;
}

RedisIndex::~RedisIndex() {
}

bool RedisIndex::GetFlag() const
{
	return mFlag;
}

void RedisIndex::SetFlag(const bool flag)
{
	mFlag = flag;
}

void RedisIndex::SetKey(const std::string key)
{
	mKey = key;
}

// 当mFlag为false才能够设置成功
bool RedisIndex::SetRole(const Role role, const bool flag/*=false*/)
{
	if (!mFlag) {
		mRole = role;
	    mFlag = flag;
	    return true;
	}
	return false;
}

void RedisIndex::SetIOMaster()
{
	mRole = MASTER;
    mFlag = true;
}

void RedisIndex::SetErrInfo(const redisReply* const reply) {
    if (NULL == reply){
        SetErrString(CONNECT_CLOSED_ERROR, ::strlen(CONNECT_CLOSED_ERROR));
    } else {
        SetErrString(reply->str, reply->len);
    }
}

bool RedisIndex::SetErrString(const char *info, int /*len*/) {
    if (NULL == info) {
        return false;
    }
    memset(mStrerr, 0, sizeof(mStrerr));
    strncpy(mStrerr, info, sizeof(mStrerr)-1);
    return true;
}

void RedisIndex::SetErrMessage(const char* fmt, ...)
{
    char szBuf[128] = { 0 };
    va_list va;
    va_start(va, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, va);
    va_end(va);
    SetErrString(szBuf, ::strlen(szBuf));
}

}
