/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_REPLY_H_
#define _GREDIS_REPLY_H_

#include <string>
#include <hiredis/hiredis.h>

#include "gRedisNode.h"

namespace gRedis
{

class RedisReply {
public:
	RedisReply(redisReply* const reply, const bool autoFree=true);
	virtual ~RedisReply();
	//拷贝构造函数
	RedisReply(const RedisReply& reply);

	bool empty() const;

	RedisReply& operator=(redisReply* const reply);

	operator redisReply*&();

	void Print();

    void FreeReply();
    static void FreeReply(redisReply*& reply);

    bool CheckReply();
    static bool CheckReply(const redisReply* const reply);

    /**
     * 判断是否需要切换到集群其它节点上去进行操作
     */
    static bool ExistCluster(const redisReply* reply, Node& node, unsigned int& slot);

private:
    bool mAutoFree;
    redisReply* mRedisReply;
};

}

#endif /* _GREDIS_REPLY_H_ */
