#include"msgpumpconf.h"


   
bool MsgPumpConfig::readConfig()
{
    TiXmlDocument doc;
    bool ret = doc.LoadFile("msgpumpconfig.xml");
    if( !ret )
    {           
        return false;
    }
    
        TiXmlDeclaration* pt = doc.ToDeclaration();

    TiXmlElement *root = doc.RootElement();
    if( !root )
    {
        return false;
    }
    
    
    TiXmlNode* broker = root->FirstChild(BROKER_NODE);                
        
    while( broker )
    {  
        for( TiXmlNode *child = broker->FirstChild();child; child = child->NextSibling() )
        {
            m_broker_set.insert( make_pair(child->Value(), child->ToElement()->GetText() ));                
        }
        
        broker = broker->ToElement()->NextSibling(BROKER_NODE);
    }

    // 获取轮询指令参数
    TiXmlNode* channel = root->FirstChild(LOOPQUERY_NODE);
    
    while( channel )
    {
        string topic_str;
        double rep_tm;
        string pre_topic_str ;
        for( TiXmlNode *child = channel->FirstChild();child; child = child->NextSibling() )
        {
            
            string tx_v = child->Value();                
            if( tx_v =="topic")
            {
                topic_str = string(LOOPQUERY_NODE) + "-" + child->ToElement()->GetText();
                m_loop_query_map.insert( make_pair(topic_str,0));
                pre_topic_str = topic_str;

            }
            if( tx_v =="reptm")
            {
                rep_tm = stod( child->ToElement()->GetText() );
                m_loop_query_map[pre_topic_str] = rep_tm;
            }              
            
        }        

        channel = channel->ToElement()->NextSibling(LOOPQUERY_NODE);
    }
    

    // 获取命令式查询
    channel = root->FirstChild(CMDQUERY_NODE);
    
    while( channel )
    {
        string topic_str;
        double rep_tm;
        string pre_topic_str ;
        for( TiXmlNode *child = channel->FirstChild();child; child = child->NextSibling() )
        {
            
            string tx_v = child->Value();                
            if( tx_v =="topic")
            {
                  topic_str = string(CMDQUERY_NODE) + "-" + child->ToElement()->GetText();
                m_cmd_query_map.insert( make_pair(topic_str,0));
                pre_topic_str = topic_str;

            }
            if( tx_v =="reptm")
            {
                rep_tm = stod( child->ToElement()->GetText() );
                m_cmd_query_map[pre_topic_str] = rep_tm;
            }              
            
        }        

        channel = channel->ToElement()->NextSibling(CMDQUERY_NODE);
    }
    

    return true;


}


