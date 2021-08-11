#pragma once
#include"msglisten.h"
#include<sw/redis++/redis++.h>
#include "msgdriverconf.h"
using namespace std;
using namespace sw::redis;

class MsgPump;
// 消息引擎，负责消息代理连接，消息构建，消息发布
class RedisListener: public IMsgListener
{

public:
    
    RedisListener(MsgPump*);
    ~RedisListener();

    // 初始化信息发布客户端
    void init();

    // 与代理进行连接
    bool connect();
    // 与代理断开
    bool disconnect();
    
  
    void listening();
    
private:
    // 根据具体命令发起后续动作
    void action( string _ask_topic );


private:
    // 引擎核心机
    std::shared_ptr<Redis> m_redis_client;    

    std::shared_ptr<Subscriber> m_subscriber;

    MsgPump* m_msg_pump;
    
    

};
