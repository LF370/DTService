#pragma once
#include"msgengine.h"
#include<sw/redis++/redis++.h>
#include "msgpumpconf.h"
using namespace std;
using namespace sw::redis;

// 消息引擎，负责消息代理连接，消息构建，消息发布
class RedisEngine: public IMsgEngine
{

public:
    //MsgEngine( string _server_address, string _port, string _user, string _pwd );
    RedisEngine();
    ~RedisEngine();

    // 初始化信息发布客户端
    void init();

    // 与代理进行连接
    bool connect();
    // 与代理断开
    bool disconnect();
    
    // 信息发布处理
    bool publish();
    
private:
    // 构建发布信息载体
    string buildPayload();


private:
    // 引擎核心机
    std::shared_ptr<Redis> m_redis_client;    
    
    

};
