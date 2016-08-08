/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#include <assert.h>

#include "gRedisCmd.h"
#include "gRedisReply.h"

namespace gRedis
{

RedisCmd::RedisCmd() {
}

RedisCmd::~RedisCmd() {
    Release();
}

bool RedisCmd::ConnectRedisGroup( const RedisNode* arrayRedisNode, const unsigned int arraySize) {
    /**
     * 判断IP地址和端口是否重复，重复则返回false
     */
    for (unsigned int uiIndexOut=0; uiIndexOut<arraySize; ++uiIndexOut) {
    	unsigned int uiCount = (MASTER==arrayRedisNode[uiIndexOut].mRole)?1:0;
        for (unsigned int uiIndexIn=uiIndexOut+1; uiIndexIn<arraySize; ++uiIndexIn) {
        	if (arrayRedisNode[uiIndexOut].mNode==arrayRedisNode[uiIndexIn].mNode) {
        		return false;
        	} else if (arrayRedisNode[uiIndexOut].mGroupID==arrayRedisNode[uiIndexIn].mGroupID) {
        		if (MASTER == arrayRedisNode[uiIndexIn].mRole) {
        			++uiCount;
        		}
        	}
        }
        if (1 < uiCount) {
        	return false;
        }
        RedisPool::Connect(arrayRedisNode[uiIndexOut]);
    }

    return true;
}

bool RedisCmd::command_bool(const char *cmd, ...) {
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);

    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        if (REDIS_REPLY_STATUS==pReply->type || 1==pReply->integer) {
            return true;
        }
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::command_status(const char* cmd, ...) {
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);

    if (RedisReply::CheckReply(reply)) {
    	return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::command_integer(int64_t &retval, const char* cmd, ...) {
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);
    if (RedisReply::CheckReply(reply)) {
        retval = ((redisReply*&)reply)->integer;
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::command_string(string &data, const char* cmd, ...) {
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        data.assign(pReply->str, pReply->len);
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::command_list(Values &vValue, const char* cmd, ...) {
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        for (size_t uiIndex = 0; uiIndex<pReply->elements; ++uiIndex) {
            vValue.push_back(string(pReply->element[uiIndex]->str, pReply->element[uiIndex]->len));
        }
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::command_array( ArrayReply& array,  const char* cmd, ...){
    va_list args;
    va_start(args, cmd);
    RedisReply reply = commandv(cmd, args);
    va_end(args);
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        for (size_t uiIndex=0; uiIndex<pReply->elements; ++uiIndex) {
            DataItem item;
            item.type = pReply->element[uiIndex]->type;
            item.str.assign(pReply->element[uiIndex]->str, pReply->element[uiIndex]->len);
            array.push_back(item);
        }
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::commandargv_bool(const VDATA& vData) {
    vector<const char *> argv( vData.size() );
    vector<size_t> argvlen( vData.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vData.begin(); i != vData.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    RedisReply reply = commandArgv(argv.size(), &(argv[0]), &(argvlen[0]));
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
    	if (1 == pReply->integer) {
        	return true;
    	}
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::commandargv_status(const VDATA& vData) {
    vector<const char *> argv( vData.size() );
    vector<size_t> argvlen( vData.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vData.begin(); i != vData.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    RedisReply reply = commandArgv(argv.size(), &(argv[0]), &(argvlen[0]));
    if (RedisReply::CheckReply(reply)) {
    	return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::commandargv_array(const VDATA& vDataIn, ArrayReply& array){
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vDataIn.begin(); i != vDataIn.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    RedisReply reply = commandArgv(argv.size(), &(argv[0]), &(argvlen[0]));
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        for (size_t uiIndex=0; uiIndex<pReply->elements; ++uiIndex) {
            DataItem item;
            item.type = pReply->element[uiIndex]->type;
            item.str.assign(pReply->element[uiIndex]->str, pReply->element[uiIndex]->len);
            array.push_back(item);
        }
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::commandargv_array(const VDATA& vDataIn, Values& array){
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vDataIn.begin(); i != vDataIn.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    RedisReply reply = commandArgv(argv.size(), &(argv[0]), &(argvlen[0]));
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        for (size_t uiIndex=0; uiIndex<pReply->elements; ++uiIndex) {
            string str(pReply->element[uiIndex]->str, pReply->element[uiIndex]->len);
            array.push_back(str);
        }
        return true;
    }

    SetErrInfo(reply);
    return false;
}

bool RedisCmd::commandargv_integer(const VDATA& vDataIn, int64_t& retval){
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator iter = vDataIn.begin(); iter != vDataIn.end(); ++iter, ++j ) {
        argv[j] = iter->c_str(), argvlen[j] = iter->size();
    }

    RedisReply reply = commandArgv(argv.size(), &(argv[0]), &(argvlen[0]));
    if (RedisReply::CheckReply(reply)) {
    	redisReply*& pReply = reply;
        retval = pReply->integer;
        return true;
    }

    SetErrInfo(reply);
    return false;
}

redisReply* RedisCmd::command(const char *format, ...)
{
    va_list ap;
    va_start(ap,format);
    RedisReply reply(commandv(format, ap), false);
    va_end(ap);
    return reply;
}

redisReply* RedisCmd::commandv(const char *format, va_list ap)
{
    RedisConn *pRedisConn = RedisPool::GetConnection(RedisIndex::mKey, RedisIndex::mRole);
    if (NULL == pRedisConn) {
        SetErrString(GET_CONNECT_ERROR, ::strlen(GET_CONNECT_ERROR));
        return NULL;
    }

    RedisReply reply(pRedisConn->commandv(format, ap), false);
    RedisPool::FreeConnection(pRedisConn);
    if (!RedisReply::CheckReply(reply)) {
    	Node node;
    	unsigned int slot = 0;
    	if (RedisReply::ExistCluster(reply, node, slot)) {
			RedisConn* const pRedisConn = RedisPool::GetConnection(node, slot);
			if (NULL != pRedisConn) {
				// 将Slot与GroupID的匹配信息进行记录
				RedisSlot::SetSlotGroup(slot, pRedisConn->getRedisNode().mGroupID);
			    reply = pRedisConn->commandv(format, ap);
			    RedisPool::FreeConnection(pRedisConn);
			}
    	}
    }

    return reply;
}

redisReply* RedisCmd::commandArgv(int argc, const char **argv, const size_t *argvlen)
{
    RedisConn *pRedisConn = RedisPool::GetConnection(RedisIndex::mKey, RedisIndex::mRole);
    if (NULL == pRedisConn) {
        SetErrString(GET_CONNECT_ERROR, ::strlen(GET_CONNECT_ERROR));
        return NULL;
    }
    RedisReply reply(pRedisConn->commandArgv(argc, argv, argvlen), false);
    RedisPool::FreeConnection(pRedisConn);
    if (!RedisReply::CheckReply(reply)) {
    	Node node;
    	unsigned int slot = 0;
    	if (RedisReply::ExistCluster(reply, node, slot)) {
			RedisConn* const pRedisConn = RedisPool::GetConnection(node, slot);
			if (NULL != pRedisConn) {
				// 将Slot与GroupID的匹配信息进行记录
				RedisSlot::SetSlotGroup(slot, pRedisConn->getRedisNode().mGroupID);
			    reply = pRedisConn->commandArgv(argc, argv, argvlen);
			    RedisPool::FreeConnection(pRedisConn);
			}
    	}
    }

    return reply;
}

}
