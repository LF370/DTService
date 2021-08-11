
#include"msgpump.h"
#include"redisengine.h"
#include "redislisten.h"
#include <thread>
// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;

vector<IMsgEngine_PTR> MsgPump::m_loopmsg_engines;
vector<IMsgEngine_PTR> MsgPump::m_askmsg_engines;


MsgPump::MsgPump( int _start_tm)
{
    m_start_tm = _start_tm;
    
    init();
}
void MsgPump::init()
{        
    EV_A = EV_DEFAULT;
    auto channel_map = g_msgpump_conf.getLoopquerypara();
    m_rep_watchers.resize( channel_map.size() );
    int i = 0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {
        auto& watcher_act = m_rep_watchers[i];
        
        watcher_act.data = reinterpret_cast<void*>(i);
        
        ev_timer_init( &watcher_act, pump_cb, m_start_tm, itr->second );
    } 

    // channel_map = g_msgpump_conf.getCmdquerypara();
    // m_signal_watchers.resize( channel_map.size() );
    // int i = 0;
    // for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    // {
    //     auto& signal_act = m_signal_watchers[i];
        
    //     signal_act.data = reinterpret_cast<void*>(i);
        
    //     ev_signal_init( &signal_act, signal_cb,SIGUSR1 );
    // } 
    
    // 构建监听器
    m_askmsg_listener = make_shared<RedisListener>(RedisListener(this));
    
    if( !m_askmsg_listener->connect() )
    {
        cout <<"查询指令监听器连接失败"<<endl;        
    }


}

    // 装配好消息引擎
int MsgPump::assemblyMsgEngine( )
{
    int success = 0;
    // 通过配置文件获取服务地址，IP 密码等
    // 以及配套的主题
    // 这里先简单设定固定值   
    auto channel_map = g_msgpump_conf.getLoopquerypara();      

    //组装轮询引擎    
    
    // 每个频道一个引擎
    m_loopmsg_engines.resize( channel_map.size() );
    int i =0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {    
        m_loopmsg_engines[i] = make_shared<RedisEngine>(RedisEngine());
        
        m_loopmsg_engines[i]->setTopic( itr->first );                 
        
    }

     channel_map = g_msgpump_conf.getCmdquerypara();     

    //组装ASK引擎    
    
    // 每个频道一个引擎
    m_askmsg_engines.resize( channel_map.size() );
    i =0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {    
        m_askmsg_engines[i] = make_shared<RedisEngine>(RedisEngine());
        
        m_askmsg_engines[i]->setTopic( itr->first );                 
        
    }

    // 完成引擎链接
    for_each( m_loopmsg_engines.begin(), m_loopmsg_engines.end(),[&success]( IMsgEngine_PTR ele ){
        success+= ele->connect()? 1:0;
    });

    for_each( m_askmsg_engines.begin(), m_askmsg_engines.end(),[&success]( IMsgEngine_PTR ele ){
        success+= ele->connect()? 1:0;
    });

    

    // // 完成引擎连接
    // for( int i = 0; i < m_msg_engines.size(); i++ )
    // {
    //     auto& engine = m_msg_engines[i];
    //     if( !engine->connect() )
    //     {            
    //         cout<<"Assmbly MsgEngine failed!:"<< i << endl;                
    //     }
    //     else{            
    //         success++;
    //     }
    // }

    //     // 完成引擎连接
    // for( int i = 0; i < m_msg_engines2.size(); i++ )
    // {
    //     auto& engine = m_msg_engines[i];
    //     if( !engine->connect() )
    //     {            
    //         cout<<"Assmbly MsgEngine failed!:"<< i << endl;                
    //     }
    //     else{            
    //         success++;
    //     }
    // }
   
    return success;
    
}
void MsgPump::receiveAskSignal( string _ask_topic )
{
    auto itr_engine = find_if( m_askmsg_engines.begin(), m_askmsg_engines.end(), [_ask_topic](IMsgEngine_PTR e){
        return e->getTopic() == _ask_topic;
    });
    if( itr_engine != m_askmsg_engines.end() )
    {
        auto engine = *itr_engine;
        engine->publish();

    }
    else{
        cout <<"无法识别的命令："<< _ask_topic << endl;
    }


}
void MsgPump::start()
{
    // 轮询监听启动
    for( auto itr = m_rep_watchers.begin(); itr != m_rep_watchers.end(); ++itr )
    {
        auto& ev_tm = *itr;
        ev_timer_start( EV_A_ &ev_tm);
    }   

    // // 信号监听启动
    // for( auto itr = m_signal_watchers.begin(); itr != m_signal_watchers.end(); ++itr )
    // {
    //     auto& ev_tm = *itr;
    //     ev_signal_start( EV_A_ &ev_tm );
    // }
    

    thread t(bind(&RedisListener::listening, dynamic_cast<RedisListener*>(m_askmsg_listener.get() )) );
    t.detach();

    
    ev_run(EV_A_ 0 );
}

void MsgPump::stop()
{
    for_each( m_loopmsg_engines.begin(), m_loopmsg_engines.end(),[]( IMsgEngine_PTR& ele){
        ele->disconnect();
    });        
}

void MsgPump::pump_cb(EV_P_ ev_timer* w, int revents)
{        
    int * ptopic_id = reinterpret_cast<int*>(w->data);
    long  topic_id  = reinterpret_cast<long>(ptopic_id);
    cout <<"topic no:"<<topic_id;
    m_loopmsg_engines[topic_id]->publish();        
} 

// void MsgPump::signal_cb( EV_P_ ev_signal * w, int revents )
// {
//     int * ptopic_id = reinterpret_cast<int*>(w->data);
//     long  topic_id  = reinterpret_cast<long>(ptopic_id);
//     cout <<"topic no:"<<topic_id;
//     m_askmsg_engines[topic_id]->publish();        
// }
