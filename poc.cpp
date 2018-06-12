#include <websocketpp/client.hpp>
#include <websocketpp/concurrency/none.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <string>
#include <iostream>
struct websocketppPolicy
        : public websocketpp::config::asio_tls_client
    {
        typedef websocketpp::concurrency::none concurrency_type;
    
    };
typedef websocketpp::client<websocketppPolicy> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

// Handlers
void on_open(client* c, websocketpp::connection_hdl hdl) {
    std::string msg = "{"
                            "\"type\": \"subscribe\","
                            "\"product_ids\": [ \"ETH-USD\" ],"
                            "\"channels\": [" "\"level2\"" "]"
                        "}";
    c->send(hdl,msg,websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Sent Message: "+msg);
}

void on_fail(client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
}

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
   // c->get_alog().write(websocketpp::log::alevel::app, "Received Reply: "+msg->get_payload());
    //c->close(hdl,websocketpp::close::status::normal,"");
    std::cout << msg->get_payload() << std::endl;
}

void on_close(client* c, websocketpp::connection_hdl hdl) {
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Closed");
}

websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls(client* c, websocketpp::connection_hdl hdl) {
    std::cout << "in on _tls" << std::endl;
    websocketpp::lib::shared_ptr<boost::asio::ssl::context>
        context = websocketpp::lib::make_shared<
            boost::asio::ssl::context>(
            boost::asio::ssl::context::tlsv1);
    try {
        context -> set_options(
                boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::no_sslv3 |
                boost::asio::ssl::context::single_dh_use);
    } catch (std::exception& e) {
        std::cerr << "set tls_init_handler() faild to set contexct options: "<< e.what() <<std::endl;
    
    }
    return context;
}
int main(int argc, char* argv[]) {
    client c;
    
    std::string uri = "wss://ws-feed.gdax.com";
    
    try {
        // set logging policy if needed
        c.clear_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::connect |
                                websocketpp::log::alevel::disconnect);
        //c.set_error_channels(websocketpp::log::elevel::none);
        
        c.clear_error_channels(websocketpp::log::elevel::all);
        c.set_error_channels(
                websocketpp::log::elevel::info |
                websocketpp::log::elevel::warn |
                websocketpp::log::elevel::rerror |
                websocketpp::log::elevel::fatal);

        // Initialize ASIO
        c.init_asio();
        // Register our handlers
        c.set_tls_init_handler(bind(&on_tls,&c,::_1));
        c.set_open_handler(bind(&on_open,&c,::_1));
        c.set_fail_handler(bind(&on_fail,&c,::_1));
        c.set_message_handler(bind(&on_message,&c,::_1,::_2));
        c.set_close_handler(bind(&on_close,&c,::_1));
        // Create a connection to the given URI and queue it for connection once
        // the event loop starts
        websocketpp::lib::error_code ec;
        auto con = c.get_connection(uri, ec);
        c.connect(con);
        // Start the ASIO io_service run loop
        c.run();
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}


