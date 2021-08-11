#pragma once
#include<vector>
#include<iostream>
#include<string>
using namespace std;

// 消息引擎，负责消息代理连接，消息构建，消息发布
class IMsgListener
{

public:
  
    // 初始化信息发布客户端
    virtual void  init() = 0;

    // 与代理进行连接
    virtual bool connect() = 0;

    // 与代理断开
    virtual bool disconnect() = 0;

  //  virtual void listening() = 0;
   
protected:
   // 根据具体命令发起后续动作
    void action( string _ask_topic );

protected:

    
    string m_server_address;
    string m_port;
    string m_user;
    string m_pwd;
    

private:

};

