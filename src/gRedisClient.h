/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016-2016, HongXG <1277206558@qq.com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _GREDIS_CLIENT_H_
#define _GREDIS_CLIENT_H_

#include "gRedisCmd.h"

using namespace std;

namespace gRedis
{

class RedisClient : public RedisCmd
{
public:
	RedisClient();
    virtual ~RedisClient();

public:

    //              Connection
    /* AUTH        */  /* nonsupport */
    /* ECHO        */  bool echo(const std::string& str, Value &value);
    /* PING        */  /* nonsupport */
    /* QUIT        */  void quit();
    /* SELECT      */  /* nonsupport */

    //                 Commands operating on std::string values
    /* APPEND      */  bool append( const Key& key,  const Value& value);
    /* BITCOUNT    */  bool bitcount(const Key& key, int& count, int start=0, int end=0);
    /* BITOP       */  //bool bitop(  const std::string& destkey, const BITOP operation, const KEYS& keys, int& lenght);
    /* BITPOS      */  bool bitpos( const Key& key, int bit, int64_t& pos, int start=0, int end=0);
    /* DECR        */  bool decr(   const Key& key, int64_t& result);
    /* DECRBY      */  bool decrby( const Key& key, int by, int64_t& result);
    /* GET         */  bool get(    const Key& key, std::string& value);
    /* GETBIT      */  bool getbit( const Key& key, int& offset, int& bit);
    /* GETRANGE    */  bool getrange(const Key& key, int start, int end, std::string& out);
    /* GETSET      */  bool getset( const Key& key, const Value& newValue, Value& oldValue);
    /* INCR        */  bool incr(   const Key& key, int64_t& result);
    /* INCRBY      */  bool incrby( const Key& key, int by, int64_t& result);
    /* INCRBYFLOAT */  
    /* MGET        */  //bool mget(const DBIArray& dbi,    const KEYS &  keys, ReplyData& vDdata);
    /* MSET        */  //bool mset(const DBIArray& dbi,    const VDATA& data);
    /* MSETNX      */  
    /* PSETEX      */  bool psetex( const Key& key,  int milliseconds, const Value& value);
    /* SET         */  bool set(    const Key& key,  const Value& value);
    /* SET         */  bool set(    const Key& key, const char *value, int len, int second=0);
    /* SETBIT      */  bool setbit( const Key& key,  int offset, int64_t newbitValue, int64_t oldbitValue);
    /* SETEX       */  bool setex(  const Key& key,  int seconds, const Value& value);
    /* SETNX       */  bool setnx(  const Key& key,  const std::string& value);
    /* SETRANGE    */  bool setrange(const Key& key,  int offset, const Value& value, int& length);
    /* STRLEN      */  bool strlen( const Key& key, int& length);


    /* DEL          */  bool del(   const Key& key);
                        //bool del(const DBIArray& dbi,      const KEYS &  vkey, int64_t& count);
    /* DUMP         */
    /* EXISTS       */  bool exists(const Key& key);
    /* EXPIRE       */  bool expire(const Key& key, unsigned int second);
    /* EXPIREAT     */  bool expireat(const Key& key, unsigned int timestamp);
    /* KEYS         */  
    /* MIGRATE      */  
    /* MOVE         */  
    /* OBJECT       */  
    /* PERSIST      */  bool persist(const Key& key);
    /* PEXPIRE      */  bool pexpire(const Key& key, unsigned int milliseconds);
    /* PEXPIREAT    */  bool pexpireat(const Key& key, unsigned int millisecondstimestamp);
    /* PTTL         */  bool pttl(const Key& key,  int64_t &milliseconds);
    /* RANDOMKEY    */  bool randomkey( Key& key);
    /* RENAME       */  
    /* RENAMENX     */  
    /* RESTORE      */  
    /* SCAN         */  

    
    /* SORT         */  bool sort(const Key& key, ArrayReply& array, const char* by = NULL,
    								LIMIT *limit = NULL, bool alpha = false, const FILEDS* get = NULL,
									const SORTODER order = ASC, const char* destination = NULL);

    /* TTL          */  bool ttl(const Key& key, int64_t& seconds);
    /* TYPE         */  


    /* HDEL         */  bool hdel(   const Key& key, const std::string& field, int64_t& num);
                        bool hdel(   const Key& key, const KEYS& vfiled, int64_t& num);
    /* HEXISTS      */  bool hexist( const Key& key, const std::string& field);
    /* HGET         */  bool hget(   const Key& key, const std::string& field, Value& value);
    /* HGETALL      */  bool hgetall(const Key& key, ArrayReply& array);
    /* HINCRBY      */  bool hincrby(const Key& key, const std::string& field, int64_t increment ,int64_t& value);
    /* HINCRBYFLOAT */  bool hincrbyfloat( const Key& key, const std::string& field, const float increment, float& value);
    /* HKEYS        */  bool hkeys(  const Key& key, KEYS& keys);
    /* HLEN         */  bool hlen(   const Key& key, int64_t& count);
    /* HMGET        */  bool hmget(  const Key& key, const KEYS& field, ArrayReply& array);
    /* HMSET        */  bool hmset(  const Key& key, const VDATA& vData);
    /* HSCAN        */                                      
    /* HSET         */  bool hset(   const Key& key, const std::string& field, const Value& value, int64_t& retval);
    /* HSETNX       */  bool hsetnx( const Key& key, const std::string& field, const Value& value);
    /* HVALS        */  bool hvals(  const Key& key, VALUES& values);

    /* BLPOP        */  
    /* BRPOP        */  
    /* BRPOPLPUSH   */  
    /* LINDEX       */  bool lindex(   const Key& key, int64_t index, Value& value);
    /* LINSERT      */  bool linsert( const Key& key, LMODEL mod, const std::string& pivot, const Value& value, int64_t& retval);
    /* LLEN         */  bool llen(    const Key& key, int64_t& len);
    /* LPOP         */  bool lpop(    const Key& key, Value& value);
    /* LPUSH        */  bool lpush(   const Key& key, const VALUES& vValue, int64_t& length);
    /* LPUSHX       */  bool lpushx(  const Key& key, const Value& value, int64_t& length);
    /* LRANGE       */  bool lrange(  const Key& key, int64_t start, int64_t end, ArrayReply& array);
    /* LREM         */  bool lrem(    const Key& key,  int count, const Value& value, int64_t num);
    /* LSET         */  bool lset(    const Key& key,  int index, const Value& value);
    /* LTRIM        */  bool ltrim(   const Key& key,  int start, int end);
    /* RPOP         */  bool rpop(    const Key& key, Value& value);
    /* RPOPLPUSH    */  bool rpoplpush(const std::string& key_src, const std::string& key_dest, Value& value);
    /* RPUSH        */  bool rpush(   const Key& key, const VALUES& vValue, int64_t& length);
    /* RPUSHX       */  bool rpushx(  const Key& key, const Value& value, int64_t& length);



    /* SADD         */  bool sadd(       const Key& key, const VALUES& vValue, int64_t& count);
    /* SCARD        */  bool scard(const Key& key, int64_t& count);
    /* SDIFF        */  //bool sdiff(const DBIArray& dbi,       const KEYS& vKkey, VALUES& vValue);
    /* SDIFFSTORE   */  //bool sdiffstore( const KEY& destinationkey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);
    /* SINTER       */  //bool sinter(const DBIArray& dbi,      const KEYS& vkey, VALUES& vValue);
    /* SINTERSTORE  */  //bool sinterstore(const KEY& destinationkey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);
    /* SISMEMBER    */  bool sismember(  const Key& key,   const Value& member);
    /* SMEMBERS     */  bool smembers(    const Key& key,  VALUES& vValue);
    /* SMOVE        */  bool smove(      const std::string& srckey, const Key& deskey,  const Value& member);
    /* SPOP         */  bool spop(       const Key& key, Value& member);
    /* SRANDMEMBER  */  bool srandmember(const Key& key, VALUES& vmember, int num=0);
    /* SREM         */  bool srem(       const Key& key, const VALUES& vmembers, int64_t& count);
    /* SSCAN        */  
    /* SUNION       */  //bool sunion(const DBIArray& dbi,      const KEYS& vkey, VALUES& vValue);
    /* SUNIONSTORE  */  //bool sunionstore(const KEY& deskey, const DBIArray& vdbi, const KEYS& vkey, int64_t& count);

    /* ZADD             */  bool zadd(   const std::string& deskey,   const VALUES& vValues, int64_t& count);
    /* ZCARD            */  bool zscrad( const Key& key, int64_t& num);
    /* ZCOUNT           */
    /* ZINCRBY          */  bool zincrby(const Key& key, const double &increment, const std::string& member, Value& value );
    /* ZINTERSTORE      */  
    /* ZRANGE           */  bool zrange( const Key& key, int start, int end, VALUES& vValues, bool withscore=false);
    /* ZRANGEBYSCORE    */  
    /* ZRANK            */  bool zrank(  const Key& key, const std::string& member, int64_t &rank);
    /* ZREM             */  bool zrem(   const Key& key, const VALUES& vmembers, int64_t &num);
    /* ZREMRANGEBYRANK  */  bool zremrangebyrank( const Key& key, int start, int stop, int64_t& num);
    /* ZREMRANGEBYSCORE */  
    /* ZREVRANGE        */  bool zrevrange( const Key& key, int start, int end, VALUES& vValues, bool withscore=false);
    /* ZREVRANGEBYSCORE */  
    /* ZREVRANK         */  bool zrevrank( const Key& key, const std::string &member, int64_t& rank);
    /* ZSCAN            */  
    /* ZSCORE           */  bool zscore( const Key& key, const std::string &member, std::string& score);
    /* ZUNIONSTORE      */  

    /* PSUBSCRIBE   */
    /* PUBLISH      */
    /* PUBSUB       */
    /* PUNSUBSCRIBE */
    /* SUBSCRIBE    */
    /* UNSUBSCRIBE  */


    /* DISCARD  */
    /* EXEC     */
    /* MULTI    */
    /* UNWATCH  */
    /* WATCH    */
};

}

#endif
