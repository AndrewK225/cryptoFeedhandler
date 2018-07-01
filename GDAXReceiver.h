#ifndef GDAXReceiver_H
#define GDAXReceiver_H
#include <websocketpp/client.hpp>
#include <websocketpp/concurrency/none.hpp>
#include <websocketpp/config/asio_client.hpp>
#include "FeedReceiver.h"
#include <string>
    using websocketpp::lib::placeholders::_1;
        using websocketpp::lib::placeholders::_2;
            using websocketpp::lib::bind;


class GDAXReceiver : public FeedReceiver {
    std::string feed_url;
    GDAXReceiver(std::string url);

    void on_open(FeedReceiver::client* c, websocketpp::connection_hdl hdl) override;
    void on_fail(FeedReceiver::client* c, websocketpp::connection_hdl hdl) override;
    void on_message(FeedReceiver::client* c, websocketpp::connection_hdl hdl, message_ptr msg) override;
    void on_close(FeedReceiver::client* c, websocketpp::connection_hdl hdl) override;    
    websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls(FeedReceiver::client* c, websocketpp::connection_hdl hdl);
    void init();

};
#endif
