/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdarg.h>

#include "gRedisComm.h"

namespace gRedis
{

RedisComm::RedisComm() {
}

RedisComm::~RedisComm() {
}

void RedisComm::addParam(VDATA& vDes, const VDATA& vSrc)
{
    for (VDATA::const_iterator iter=vSrc.begin(); iter!=vSrc.end();++iter)
    {
        vDes.push_back(*iter);
    }
}

/**
 * 格式化命令
 */
bool RedisComm::formatCommand(std::string& target, const char *format, ...)
{
    va_list ap;
    va_start(ap,format);
    const bool result = formatCommand(target, format, ap);
    va_end(ap);
    return result;
}

/**
 * 格式化命令
 */
bool RedisComm::formatCommand(std::string& target, const char *format, va_list ap)
{
    char value[USHRT_MAX] = {'\0'};
    if (0 < vsnprintf(value, sizeof(value)-1, format, ap)) {
    	target = value;
    }
    return !target.empty();
}

}
