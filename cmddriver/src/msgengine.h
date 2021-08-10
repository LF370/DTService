#pragma once
#include<vector>
#include<iostream>
#include<string>
using namespace std;

// 消息引擎，负责消息代理连接，消息构建，消息发布
class IMsgEngine
{

public:
  
    // 初始化信息发布客户端
    virtual void  init() = 0;

    // 与代理进行连接
    virtual bool connect() = 0;

    // 与代理断开
    virtual bool disconnect() = 0;
    
    // 信息发布处理
    virtual bool publish() = 0;

    
    // 设定主题，注意一个泵一个主题
    void setTopic(string _topic) { m_topic = _topic;}
    string getTopic() const { return m_topic;}

protected:
    // 构建发布信息载体
    virtual string buildPayload() = 0;

protected:

    string  m_topic;       
    string m_server_address;
    string m_port;
    string m_user;
    string m_pwd;
    string m_cliendid;
    string m_presist_dir;
    string m_lastwords;

private:

};

