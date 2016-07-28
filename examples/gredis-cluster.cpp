
 /** \example demo_cluster.cpp
 * This is an example of how to use the gRedis.
 * <br>Connect to a redis cluster which contains three redis node
 * <br>More details about this example.
 */

#include <stdio.h>

#include "../src/gRedisClient.h"

using namespace gRedis;

RedisNode arrayRedisNode[]=
{
	RedisNode(Node("10.8.0.1", 6379),  "", 1, 5, MASTER, 0),
    RedisNode(Node("10.8.0.1", 26379), "", 1, 5, MASTER, 1),
    RedisNode(Node("10.8.0.1", 46379), "", 1, 5, MASTER, 2),
	RedisNode(Node("10.8.0.1", 6380),  "", 1, 5, SLAVE, 0),
    RedisNode(Node("10.8.0.1", 26380), "", 1, 5, SLAVE, 1),
    RedisNode(Node("10.8.0.1", 46380), "", 1, 5, SLAVE, 2)
};

int main(int /*argc*/, char **/*argv*/) {

    RedisClient redisClient;
    if (!redisClient.ConnectRedisGroup(arrayRedisNode, sizeof(arrayRedisNode)/sizeof(RedisNode))) {
        printf("error ConnectRedisGroup \r\n");
        return EXIT_FAILURE;
    }

    const char *key = "test";
    const char *value = "test value";

    for (unsigned int uiIndex=0; uiIndex<1000; ++uiIndex) {
    	ostringstream os;
    	os << key << "-" << (uiIndex%10);
    	printf("====== Index:%d Key:%s ======\n", uiIndex, os.str().c_str());
        bool bRet = redisClient.set(os.str(), value);
        if (bRet){
               printf("set success \r\n");
         } else {
            printf("set error [%s] \r\n", redisClient.GetErrInfo());
         }

        string strValue;
        bRet = redisClient.get(os.str(), strValue);
        if (bRet) {
            printf("get success %s \r\n", strValue.c_str());
        } else {
            printf("get error [%s] \r\n", redisClient.GetErrInfo());
        }

        bRet = redisClient.exists(os.str());
        if (bRet) {
            printf("exists success \r\n");
        } else {
            printf("exists error [%s] \r\n", redisClient.GetErrInfo());
        }

        sleep(1);
    }

    return 0;
}


