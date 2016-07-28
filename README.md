gRedis
======

C++ Redis client, support the data slice storage, connection pool, cluster, read/write separation.

**Features:**
* data slice storage
* support Redis master slave connection, Support read/write separation
* connection pool
* simultaneously connected multiple data slice groups  
* most Redis commands have been implemented
* multi thread safety
* suport linux and windows
* suport redis cluster

### Dependencies

gredis requires hiredis only

### Install

First step install libhiredis, on a Debian system you can use:

```bash
sudo apt-get install libhiredis-dev
```

on centos/redhat/fedora system you can use:
```bash
sudo yum install hiredis-devel
```

Then checkout the code and compile it
```bash
git clone https://github.com/HongXG/gredis.git
cd gredis
make
sudo make install
```

### Documentation
!<p>See [examples](https://github.com/HongXG/gredis/tree/master/examples) directory for some examples
<p>gRedis QQ Group: 130720409
