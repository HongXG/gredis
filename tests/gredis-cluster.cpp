#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtest/gtest.h>

#include "../src/gRedisClient.h"

using namespace gRedis;

RedisClient redisClient;

class CTest : public testing::Test
{
public:
	CTest()
	{
	}
	virtual ~CTest()
	{
	}
};

TEST(CTest, test_zadd)
{
    std::string strkey="test:sorted:key";
    VALUES vVal;
    int64_t retVal=0;
    int64_t scores = 168;
    vVal.push_back(toString(scores));
    vVal.push_back("sorted value hello gredis");
    EXPECT_EQ(true, redisClient.zadd(strkey, vVal, retVal));
}

TEST(CTest, test_set)
{
    EXPECT_EQ(true, redisClient.set("test", "123456789"));
}

TEST(CTest, test_append)
{
    EXPECT_EQ(true, redisClient.append("test", "0"));
}

TEST(CTest, test_decr)
{
	int64_t res = 0;
    EXPECT_EQ(true, redisClient.decr("test", res));
}

TEST(CTest, test_decrby)
{
	int64_t res = 0;
    EXPECT_EQ(true, redisClient.decrby("test", 11, res));
}

TEST(CTest, test_incr)
{
	int64_t res = 0;
    EXPECT_EQ(true, redisClient.incr("test", res));
}

TEST(CTest, test_incrby)
{
	int64_t res = 0;
    EXPECT_EQ(true, redisClient.incrby("test", 11, res));
}

TEST(CTest, test_get)
{
	string strData;
    EXPECT_EQ(true, redisClient.get("test", strData));
}

TEST(CTest, test_getrange)
{
	string strData;
    EXPECT_EQ(true, redisClient.getrange("test", 2, 6, strData));
}

TEST(CTest, test_exists)
{
    EXPECT_EQ(true, redisClient.exists("test"));
}

TEST(CTest, test_del)
{
    EXPECT_EQ(true, redisClient.del("test"));
}

TEST(CTest, test_hset)
{
    int64_t count = 0;
    EXPECT_EQ(true, redisClient.hset("hashtest", "filed1", "filed1_values", count));
}

TEST(CTest, test_lpush)
{
    VALUES vVal;
    vVal.push_back(toString(time(NULL)));
    int64_t count = 0;
    EXPECT_EQ(true, redisClient.lpush("list_test", vVal, count));
}

TEST(CTest, test_llen)
{
    int64_t count = 0;
    EXPECT_EQ(true, redisClient.llen("list_test", count));
}

TEST(CTest, test_lrange)
{
	ArrayReply Reply;
    EXPECT_EQ(true, redisClient.lrange("list_test", 0, -1, Reply));
}

TEST(CTest, test_lpop)
{
	string strVal;
    EXPECT_EQ(true, redisClient.lpop("list_test", strVal));
}

TEST(CTest, test_rpush)
{
    VALUES vVal;
    vVal.push_back(toString(time(NULL)));
    int64_t count = 0;
    EXPECT_EQ(true, redisClient.rpush("list_test", vVal, count));
}

TEST(CTest, test_rpop)
{
	string strVal;
    EXPECT_EQ(true, redisClient.rpop("list_test", strVal));
}

int main(int argc, char **argv)
{
	RedisNode arrayRedisNode[]=
	{
		RedisNode(Node("127.0.0.1", 6379), "", 1, 5, MASTER, 0),
		RedisNode(Node("127.0.0.1", 6380), "", 1, 5, SLAVE,  0),
	    RedisNode(Node("127.0.0.1", 6579), "", 1, 5, MASTER, 1),
	    RedisNode(Node("127.0.0.1", 6580), "", 1, 5, SLAVE,  1),
	    RedisNode(Node("127.0.0.1", 6779), "", 1, 5, MASTER, 2),
	    RedisNode(Node("127.0.0.1", 6780), "", 1, 5, SLAVE,  2)
	};

    if (!redisClient.ConnectRedisGroup(arrayRedisNode, sizeof(arrayRedisNode)/sizeof(RedisNode))) {
        printf("error ConnectRedisGroup \r\n");
        return EXIT_FAILURE;
    }

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}


