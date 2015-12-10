#include "kuser.h"

#include <cstring>
#include <librdkafka/rdkafkacpp.h>

namespace network { namespace kafka {

    kproducer::kproducer ( const char* host, const char* topic )
    {
        this->host = host;
        this->topic_str = topic;
        
        this->partition = RdKafka::Topic::PARTITION_UA;
        
        // event callback
        /*ExampleEventCb ex_event_cb;
        conf->set("event_cb", &ex_event_cb, errstr);
        
        // delivery callback
        ExampleDeliveryReportCb ex_dr_cb;
        conf->set("dr_cb", &ex_dr_cb, errstr);*/
    }
    
    kproducer* kproducer::create ( const char* host, const char* topic )
    {
        kproducer* tmp = new kproducer ( host, topic );
        std::string errstr;
        
        tmp->conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        tmp->tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
        tmp->conf->set("metadata.broker.list", tmp->host, errstr);
        
        tmp->producer = RdKafka::Producer::create(tmp->conf, errstr);
        if (!tmp->producer) 
        {
            delete tmp;
            return NULL;
        }
        std::cout << ">> Created producer " << tmp->producer->name() << std::endl;
        
        tmp->topic = RdKafka::Topic::create(tmp->producer, tmp->topic_str, tmp->tconf, errstr);
        if (!tmp->topic)
        {
            delete tmp;
            return NULL;
        }
        std::cout << ">> Created topic " << tmp->topic->name() << std::endl;
        return tmp;
    }
    
    kproducer::~kproducer ()
    {
        while (this->producer->outq_len() > 0) 
        {
            std::cerr << "Waiting for " << producer->outq_len() << std::endl;
            producer->poll(1000);
        }

        delete topic;
        delete producer;
        RdKafka::wait_destroyed(5000);
    }
    
    bool kproducer::setHost ( const char* host )
    {
        this->host=host;
        std::string errstr;
        conf->set("metadata.broker.list", this->host, errstr);
        
        RdKafka::Producer *producer = RdKafka::Producer::create(this->conf, errstr);
        if (!producer)
            return NULL;
        
        this->setTopic ( this->topic_str );
        
        RdKafka::Producer* prod=this->producer;
        this->producer = producer;
        delete prod;
        
        return true;
    }
    
    const char* kproducer::getHost ()
    {
        return this->host;
    }
    
    bool kproducer::setTopic ( const char* topic )
    {
        std::string errstr;
        this->topic_str = topic;
        //this->topic = RdKafka::Topic::create(this->producer, topic, this->tconf, errstr);
        return true;
    }
    
    const char* kproducer::getTopic ()
    {
        return this->topic_str;
    }
    
    bool kproducer::send ( const char* str, int size )
    {
        RdKafka::ErrorCode resp = producer->produce(this->topic, 
                                        this->partition, 
                                        RdKafka::Producer::RK_MSG_COPY, 
                                        const_cast<char*>(str), size, NULL, NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
            std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
        else
            std::cerr << "% Produced message (" << size << " bytes)" << std::endl;
        producer->poll(0);
        
        return true;
    }
    
    bool kproducer::send ( const char* str )
    {
        RdKafka::ErrorCode resp = producer->produce(this->topic, 
                                        this->partition, 
                                        RdKafka::Producer::RK_MSG_COPY, 
                                        const_cast<char*>(str), strlen(str), NULL, NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
            std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
        else
            std::cerr << "% Produced message (" << strlen(str) << " bytes)" << std::endl;
        producer->poll(0);
        
        return true;
    }
    
    bool kproducer::send ( std::string str )
    {
        RdKafka::ErrorCode resp = producer->produce(this->topic, 
                                       this->partition, 
                                       RdKafka::Producer::RK_MSG_COPY, 
                                       const_cast<char *>(str.c_str()), 
                                       str.size(), NULL, NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
            std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
        else
            std::cerr << "% Produced message (" << str.size() << " bytes)" << std::endl;
        producer->poll(0);
        
        return true;
    }
}}
