
#include"msgpump.h"
#include"redisengine.h"
// 信息泵
// 按照定时发布消息
extern class MsgPumpConfig g_msgpump_conf;

vector<IMsgEngine_PTR> MsgPump::m_msg_engines;


MsgPump::MsgPump( int _start_tm)
{
    m_start_tm = _start_tm;
    
    init();
}
void MsgPump::init()
{        
    EV_A = EV_DEFAULT;
    auto channel_map = g_msgpump_conf.getLoopquerypara();
    m_rep_watcher_map.resize( channel_map.size() );
    int i = 0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {
        auto& watcher_act = m_rep_watcher_map[i];
        
        watcher_act.data = reinterpret_cast<void*>(i);
        
        ev_timer_init( &watcher_act, pump_cb, m_start_tm, itr->second );
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

    //组装引擎    
    
    // 每个频道一个引擎
    m_msg_engines.resize( channel_map.size() );
    int i =0;
    for( auto itr = channel_map.begin(); itr != channel_map.end(); ++itr, i++ )
    {    
        m_msg_engines[i] = make_shared<RedisEngine>(RedisEngine());
        
        m_msg_engines[i]->setTopic( itr->first );                 
        
    }
    // 完成引擎连接
    for( int i = 0; i < m_msg_engines.size(); i++ )
    {
        auto& engine = m_msg_engines[i];
        if( !engine->connect() )
        {            
            cout<<"Assmbly MsgEngine failed!:"<< i << endl;                
        }
        else{            
            success++;
        }
    }
   
    return success;
    
}

void MsgPump::start()
{
    for( auto itr = m_rep_watcher_map.begin(); itr != m_rep_watcher_map.end(); ++itr )
    {
        auto& ev_tm = *itr;
        ev_timer_start( EV_A_ &ev_tm);
    }   
    
    ev_run(EV_A_ 0 );
}

void MsgPump::stop()
{
    for_each( m_msg_engines.begin(), m_msg_engines.end(),[]( IMsgEngine_PTR& ele){
        ele->disconnect();
    });        
}

void MsgPump::pump_cb(EV_P_ ev_timer* w, int revents)
{        
    int * ptopic_id = reinterpret_cast<int*>(w->data);
    long  topic_id  = reinterpret_cast<long>(ptopic_id);
    cout <<"topic no:"<<topic_id;
    m_msg_engines[topic_id]->publish();        
}    
