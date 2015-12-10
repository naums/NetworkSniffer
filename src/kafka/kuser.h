#ifndef NAUMS_KAFKA_PRODUCER
#define NAUMS_KAFKA_PRODUCER

#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include "../pcap/trace.h"

namespace network { namespace kafka {
        
    /**
     * \class kconsumer wrapper for the consumer - functionality for kafka
     **/
    class kconsumer
    {
        private:
            /// string identifying the host-ip + port of the kafka server
            const char* host;
            /// string identifying the topic on the server
            const char* topic_str;
            
            /// global Configuration for Kafka
            RdKafka::Conf *conf;
            /// topic Configuration for kafka
            RdKafka::Conf *tconf;
            /// the wrapped consumer-object of librdkafka++
            RdKafka::Consumer *consumer;
            /// topic-object of librdkafka++
            RdKafka::Topic *topic;
            
            /// partition number
            int32_t partition;
            /// offset for recv message
            int64_t start_offset;
            
            /// if true the consumer has been started, otherwise not (or has already been stopped)
            bool started;
                
            /**
             * \brief creates an empty kconsumer-object
             * \param host the host+ip + post
             * \param topic the topic string
             **/
            kconsumer ( const char* host, const char* topic );    
                
        public:
            /**
             * \brief creates and returns the kconsumer-object
             * \param host the host+ip + post
             * \param topic the topic string
             **/
            static kconsumer* create ( const char* host, const char* topic );
            /**
             * \brief deletes the consumer-object
             **/
            ~kconsumer ();
            
            /**
             * \brief will set the offset of messages to read
             * \param[in] offset the offset to be read next
             **/
            bool setStartOffset ( int32_t offset );
                        
            /**
             * \brief starts the consumer (makes it listen to the topic)
             * \return true if successful, false otherwise
             **/            
            bool start();
            /**
             * \brief stops the consumer; the consumer will not be able to listen to messages, until started again
             * \return true if successful, false otherwise
             **/
            bool stop ();
            
            /**
             * \brief set a new host-adress
             * \param host the new host-adress of the kafka-server
             **/
            bool setHost ( const char* host );
            /**
             * \brief returns the host-ip + port (string) of the kafka server
             * \note please do not free or overwrite the memory!
             * \return string with ip / URL + port
             **/
            const char* getHost ();
            
            /**
             * \brief sets the partition for kafka to read from
             **/
            bool setPartition ( int32_t part );
            /**
             * \brief returns the currently set partition
             **/
            int32_t getPartition ();
            
            /**
             * \brief sets a new topic-string
             * \param topic the new topic-string
             **/
            bool setTopic ( const char* topic );
            /**
             * \brief returns the topic-string
             * \note please do not free or overwrite the memory!
             * \return string with topic-name
             **/
            const char* getTopic ();
            
            /**
             * \brief recvs a message from the kafka server and returns the RdKafka::Message Object (ptr to the object)
             * \note don't forget to delete the object after usage
             **/
            RdKafka::Message* recv ( );
    };
    
    /**
     * \class kproducer wrapper for the consumer - functionality for kafka
     **/
    class kproducer
    {
        private:
            /// string identifying the host-ip + port of the kafka server
            const char* host;
            /// string identifying the topic on the server
            const char* topic_str;
            
            /// global Configuration for Kafka
            RdKafka::Conf *conf;
            /// topic Configuration for kafka
            RdKafka::Conf *tconf;
            /// the wrapped producer-object of librdkafka++
            RdKafka::Producer *producer;
            /// topic-object of librdkafka++
            RdKafka::Topic *topic;
            
            /// partition number
            int32_t partition;
            /// offset for recv message
            int64_t start_offset;
                
            /**
             * \brief creates an empty kproducer-object
             * \param host the host+ip + post
             * \param topic the topic string
             **/
            kproducer ( const char* host, const char* topic );  
                
        public:
            /**
             * \brief creates and returns the kproducer-object
             * \param host the host+ip + post
             * \param topic the topic string
             **/
            static kproducer* create ( const char* host, const char* topic );
            /**
             * \brief delete the producer-object
             **/
            ~kproducer ();
            
/**
             * \brief set a new host-adress
             * \param host the new host-adress of the kafka-server
             **/
            bool setHost ( const char* host );
            /**
             * \brief returns the host-ip + port (string) of the kafka server
             * \note please do not free or overwrite the memory!
             * \return string with ip / URL + port
             **/
            const char* getHost ();
            
            /**
             * \brief sets a new topic-string
             * \param topic the new topic-string
             **/
            bool setTopic ( const char* topic );
            /**
             * \brief returns the topic-string
             * \note please do not free or overwrite the memory!
             * \return string with topic-name
             **/
            const char* getTopic ();
            
            /**
             * \brief sends a message (given with string and size)
             * \param[in] str the string (Bytes) to be sent
             * \param[in] size explicitly given size of the string (in Bytes)
             * \return true if successful, false if not
             **/
            bool send ( const char* str, int size );
            /**
             * \brief sends a nullterminated message (given as string)
             * \param[in] str the string (Bytes) to be sent
             * \note the size of the message is determined by strlen, str must be null-terminated!
             * \return true if successful, false if not
             **/
            bool send ( const char* str );
            /**
             * \brief sends a message (given as std::string)
             * \param[in] str the string  to be sent
             * \return true if successful, false if not
             **/
            bool send ( std::string str );
    };
}}

#endif
