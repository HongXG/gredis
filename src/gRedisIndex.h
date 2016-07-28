/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_INDEX_H_
#define _GREDIS_INDEX_H_

#include <string>
#include <stdint.h>
#include <hiredis/hiredis.h>

#include "gRedisSlot.h"
#include "gRedisComm.h"

typedef unsigned int (*HASHFUN)(const char*, const int);

namespace gRedis
{

class RedisIndex {
public:
	RedisIndex();
	RedisIndex(const std::string key);
	virtual ~RedisIndex();

    char *GetErrInfo() {return mStrerr;}
    void SetIOMaster();

protected:
    bool GetFlag() const;
    void SetFlag(const bool flag);
	void SetKey(const std::string key);
    void SetErrInfo(const redisReply* const reply);
    bool SetErrString(const char *info, int len);
    void SetErrMessage(const char* fmt, ...);
    // 当mFlag为false才能够设置成功
    bool SetRole(const Role role, const bool flag=false);

protected:
    std::string  mKey;
    char         mStrerr[5120];
    Role         mRole;
    bool         mFlag;
};

}

#endif /* _GREDIS_INDEX_H_ */
