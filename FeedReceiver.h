#ifndef FEEDRECEIVER_H
#define FEEDRECEIVER_H
#include <websocketpp/client.hpp>
#include <websocketpp/concurrency/none.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <string>
#include <iostream>

class FeedReceiver {
public:
    struct websocketppPolicy 
        : public websocketpp::config::asio_tls_client
    {
        typedef websocketpp::concurrency::none concurrency_type;
    
    };
    using client = typename websocketpp::client<websocketppPolicy>;
   // using websocketpp::lib::placeholders::_1;
   // using websocketpp::lib::placeholders::_2;
   // using websocketpp::lib::bind;
    using message_ptr = typename websocketpp::config::asio_client::message_type::ptr;

    virtual void on_open(client* c, websocketpp::connection_hdl hdl)= 0;
    virtual void on_fail(client* c, websocketpp::connection_hdl hdl)= 0;
    virtual void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg)= 0;
    virtual void on_close(client* c, websocketpp::connection_hdl hdl)= 0;    
    

};

#endif
