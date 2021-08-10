
/*
    消息泵，按照设定的时序完成信息的发布
*/
#pragma once
#include<vector>
#include<iostream>
#include<memory>
#include<ev.h>
#include"msgpumpconf.h"
#include"msgengine.h"


using namespace std;


typedef shared_ptr<IMsgEngine> IMsgEngine_PTR;

// 信息泵
// 按照定时发布消息
class MsgPump
{
public:
    MsgPump( int _start_tm = 0 );
     
        
protected:
    void init();    

public:
    // 装配好消息引擎
    int assemblyMsgEngine( );
    
    // 组装好的引擎启动
    void start();
    
    // 停机
    void stop();
    
protected:

    // 定时回调函数
    static void pump_cb(EV_P_ ev_timer* w, int revents);
    
    protected:
    EV_P;
    
    // 监听器
    // key:topic
    // value:监听者    
    vector<ev_timer> m_rep_watcher_map;   

    int m_start_tm;
    int m_repeate_tm;

    private:

    // 引擎集
    // 每个频道一个引擎
    static std::vector<IMsgEngine_PTR> m_msg_engines;


    
    

};
