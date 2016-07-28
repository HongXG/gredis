gRedis
======

C++ Redis client, support the data slice storage, connection pool, cluster, read/write separation.

gRedis 是一个C++开发的redis客户端，是对hiredis的C++封装，提供易用的redis命令操作接口.

***功能与特点：***
* 支持数据多节点分布存储，可自定义分片规则;
* 支持同时连接到每个分片的主从节点，支持主从读写分离;
* 支持对每个存储节点建立连接池;
* 支持同时连接多个数据分片集群;
* 提供简单易用的C++接口封装，已实现大部分REDIS命令;
* 只依赖hiredis库;
* 多线程安全
* 支持Reids集群
 

### Dependencies

gredis 依赖 hiredis ,  在使用gRedis前需要安装hiredis库

### Install

第一步 安装libhiredis
 在Debian系统上:
```bash
sudo apt-get install libhiredis-dev
```

gRedis源码安装
```bash
git clone https://github.com/HongXG/gredis.git
cd gredis
make
sudo make install
```

### 相关文档
!<p>使用示例 [examples](https://github.com/HongXG/gredis/tree/master/examples) directory for some examples
<p>gRedis开源社区QQ群: 130720409
