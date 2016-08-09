/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include "gRedisNode.h"

namespace gRedis
{

Node::Node()
{
	clear();
}

Node::Node(const std::string host, const unsigned int port)
	: mHost(host),
	  mPort(port)
{

}

Node::~Node()
{

}

bool Node::empty() const
{
	return 0==mPort && mHost.empty();
}

void Node::clear()
{
	mHost.clear();
	mPort = 0;
}

bool Node::operator==(const Node& node) const
{
	return mHost==node.mHost && mPort==node.mPort;
}

bool Node::operator<(const Node& node) const
{
	return mHost<node.mHost || (mHost==node.mHost && mPort<node.mPort);
}

bool Node::operator>(const Node& node) const
{
	return mHost>node.mHost || (mHost==node.mHost && mPort>node.mPort);
}

RedisNode::RedisNode()
{
	clear();
}

RedisNode::RedisNode(
		const Node         node,
		const std::string  passwd,
		const unsigned int poolsize,
		const unsigned int timeout,
		const Role         role/*=SLAVE*/,
		const GroupID      groupID/*=0*/)
	: mGroupID(groupID),
	  mNode(node),
	  mPasswd(passwd),
	  mPoolsize(poolsize),
	  mTimeout(timeout),
	  mRole(role)
{
}

RedisNode::~RedisNode() {
}

RedisNode::operator const Node&() {
	return mNode;
}

bool RedisNode::operator==(const RedisNode& redisNode) const
{
	if (mGroupID==redisNode.mGroupID
			&& mNode==redisNode.mNode
			&& mPasswd==redisNode.mPasswd
			&& mPoolsize==redisNode.mPoolsize
			&& mTimeout==redisNode.mTimeout
			&& mRole==redisNode.mRole)
	{
		return true;
	}
	return false;
}

bool RedisNode::empty() const
{
	return RedisNode()==*this;
}

void RedisNode::clear()
{
	mGroupID = 0;
	mNode.clear();
	mPasswd.clear();
    mPoolsize = 0;
    mTimeout = 0;
    mRole = SLAVE;
}

void RedisNode::Print() const
{
	printf("mGroupID:%u  mNode(%s:%d)  mPasswd:%s  mPoolsize:%u  mTimeout:%u  mRole:%d\n",
			mGroupID, mNode.mHost.c_str(), mNode.mPort, mPasswd.c_str(), mPoolsize, mTimeout, mRole);
}

}
