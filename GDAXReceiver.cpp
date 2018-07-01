#include "FeedReceiver.h"
#include "GDAXReceiver.h"
#include <websocketpp/client.hpp>
#include <websocketpp/concurrency/none.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <string>
#include <iostream>

GDAXReceiver::GDAXReceiver(std::string url) 
    : feed_url(url)
{
}

void GDAXReceiver::on_open(FeedReceiver::client* c, websocketpp::connection_hdl hdl) {
    std::string msg = "{"
                            "\"type\": \"subscribe\","
                            "\"product_ids\": [\"ETH-USD\"],"
                            "\"channels\": [" "\"level2\"" "]"
                       "}";
    c->send(hdl, msg, websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+msg);
}


void GDAXReceiver::on_fail(FeedReceiver::client* c, websocketpp::connection_hdl hdl) {    
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void GDAXReceiver::on_message(FeedReceiver::client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << msg->get_payload() << std::endl;

}

void GDAXReceiver::on_close(FeedReceiver::client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app,"Connection Closed");
}

websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls(FeedReceiver::client* c, websocketpp::connection_hdl hdl) {
    websocketpp::lib::shared_ptr<boost::asio::ssl::context>
        context = websocketpp::lib::make_shared<boost::asio::ssl::context>(
                boost::asio::ssl::context::tlsv1);
    try {
        context -> set_options(
                boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::no_sslv3 |
                boost::asio::ssl::context::single_dh_use);
    
    } catch(std::exception& e) {
        std::cerr << "tls_init_handler() failed" << e.what() << std::endl;
    
    }

    return context;
}

void GDAXReceiver::init() {
    FeedReceiver::client c;

    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;

    std::string uri = feed_url;
    try {
        c.clear_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::connect |
                                websocketpp::log::alevel::disconnect);

        c.clear_error_channels(websocketpp::log::elevel::all);
        c.set_error_channels(
                websocketpp::log::elevel::info |
                websocketpp::log::elevel::warn |
                websocketpp::log::elevel::rerror |
                websocketpp::log::elevel::fatal);

        c.init_asio();
        c.set_tls_init_handler(bind(&GDAXReceiver::on_tls,&c,::_1));
        //c.set_open_handler(bind(&GDAXReceiver::on_open,&c,::_1));
        //c.set_fail_handler(bind(&GDAXReceiver::on_fail,&c,::_1));
        //c.set_message_handler(bind(&GDAXReceiver::on_message,&c,::_1,::_2));
        //c.set_close_handler(bind(&GDAXReceiver::on_close,&c,::_1));
        
        websocketpp::lib::error_code ec;
        auto con = c.get_connection(uri,ec);
        c.connect(con);
        c.run();    
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    
    }


}

