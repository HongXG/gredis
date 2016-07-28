/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_NODE_H_
#define _GREDIS_NODE_H_

#include <string>

#include "gRedisComm.h"

namespace gRedis
{

class Node
{
public:
	Node();
	Node(const std::string host, const unsigned int port);
	virtual ~Node();

	bool empty() const;
	void clear();

	bool operator==(const Node& node) const;
	bool operator<(const Node& node) const;
	bool operator>(const Node& node) const;

public:
    std::string  mHost;            // redis host
    unsigned int mPort;            // redis sever port
};

class RedisNode {
public:
	RedisNode();
	RedisNode(
			const Node         node,
			const std::string  passwd,
			const unsigned int poolsize,
			const unsigned int timeout,
			const Role         role=SLAVE,
			const GroupID      groupID=0);
	virtual ~RedisNode();

	operator const Node&();

	bool operator==(const RedisNode& redisNode) const;

	bool empty() const;
	void clear();

public:
	GroupID      mGroupID;
	Node         mNode;
    std::string  mPasswd;          // redis server password
    unsigned int mPoolsize;        // connect pool size for each redis DB
    unsigned int mTimeout;         // connect timeout second
    Role         mRole;            // redis role
};

}

#endif /* _GREDIS_NODE_H_ */
