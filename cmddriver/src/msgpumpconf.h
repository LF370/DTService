/*
    通过外部xml 文件：msgpumpconfig.xml 获取
    消息泵，消息引擎的配置参数
    消息引擎参数：
    服务器地址
    端口号
    用户名和密码
    消息泵参数：
    所有主题+定时
*/
#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<tinyxml.h>
using namespace std;

#define BROKER_NODE "BROKER"
#define LOOPQUERY_NODE "LOOPQUERY"
#define CMDQUERY_NODE "CMDASK"

class MsgPumpConfig
{
public:
     const map<string,string>& getBrokerConf() const { return m_broker_set; }
     const map<string,double>& getLoopquerypara() const  { return m_loop_query_map; }
     const map<string,double>& getCmdquerypara() const  { return m_cmd_query_map; }


public:
    bool readConfig();   


protected:    
    // 基本登录信息
    map<string,string> m_broker_set;
   

    // 信息频道
    // 轮询参数表
    // key: topic
    // value:repeate tm
    map<string,double> m_loop_query_map;

    // 查询参数表
    map<string,double> m_cmd_query_map;


};


