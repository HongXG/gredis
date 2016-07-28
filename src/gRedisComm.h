/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_COMM_H_
#define _GREDIS_COMM_H_

#include <vector>
#include <sstream>
#include <stdio.h>
#include <limits.h>

#define MAX_GROUPID (UINT_MAX-1)
#define ERROR_GROUPID (UINT_MAX)

typedef unsigned int GroupID;

typedef std::string             Key;
typedef std::string             Value;

typedef std::vector<std::string>     VDATA;

#define MAX_REDIS_CONNPOOL_SIZE     128      // 每个DB最大连接数

#define GET_CONNECT_ERROR       "get connection error"
#define CONNECT_CLOSED_ERROR    "redis connection be closed"

#ifdef WIN32
    #define   strcasecmp   stricmp
    #define   strncasecmp  strnicmp
#endif

/**
 * 主备角色定义
 */
enum Role {
    MASTER = 0,
    SLAVE
};

/**
 * 内存回收
 */
#define DELETE(point)      { if(NULL!=point){delete point; point=NULL;} }
#define DELETEARRAY(point) { if(NULL!=point){delete[] point; point=NULL;} }

/**
 * 通用对象转字符串
 */
template<class Type>
std::string toString(const Type& val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

namespace gRedis
{

class RedisComm {
public:
	RedisComm();
	virtual ~RedisComm();

    static void addParam(VDATA& vDes, const VDATA& vSrc);
    static bool formatCommand(std::string& target, const char *format, ...);
    static bool formatCommand(std::string& target, const char *format, va_list ap);
};

}

#endif /* _GREDIS_COMM_H_ */
