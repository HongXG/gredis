/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <sstream>
#include "gRedisClient.h"

namespace gRedis
{

void RedisClient::quit(){
	Release();
}


bool RedisClient::echo(const string& str, Value& value)
{
	if (0==str.length()) {
        return false;
    }
    RedisIndex::SetKey(str);
    RedisIndex::SetRole(MASTER);
    return command_string(value, "echo %s", str.c_str());
}

}
