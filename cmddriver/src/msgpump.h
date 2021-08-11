
/*
    消息泵，按照设定的时序完成信息的发布
*/
#pragma once
#include<vector>
#include<iostream>
#include<memory>
#include<ev.h>
#include"msgdriverconf.h"
#include"msgengine.h"
#include "msglisten.h"




using namespace std;

typedef shared_ptr<IMsgListener> IMsgListener_PTR;

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

    // 捕获来自ask 命令监听器的指令
    void receiveAskSignal( string _ask_topic );
    
protected:

    // 定时回调函数
    static void pump_cb(EV_P_ ev_timer* w, int revents);

    //static void signal_cb( EV_P_ ev_signal* w, int revents );
    
    protected:
    EV_P;
    
    // 轮询 监听器
    // key:topic
    // value:监听者    
    vector<ev_timer> m_rep_watchers;   

    // 命令信号监听
    //vector<ev_signal> m_signal_watchers;



    int m_start_tm;
    int m_repeate_tm;

    private:

    // 引擎集
    // 每个频道一个引擎
    // 轮询专用
    static std::vector<IMsgEngine_PTR> m_loopmsg_engines;

    // ASK 指令专用
    static std::vector<IMsgEngine_PTR> m_askmsg_engines;

    
    // ASK命令监听器
    IMsgListener_PTR m_askmsg_listener;


    
    

};
