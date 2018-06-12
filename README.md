# cryptoFeedhandler

poc.cpp is the proof of concept just to connect and stream data.

Proposed design:
1) Reorganize the logic in poc.cpp into an abstract FeedReceiver class.  FeedReceivers for specific exchanges (in this case, GDAX) will inherit from this. Ex. GDAXFeedReceiver.

2) The GDAXFeedReceiver class will connect, subscribe to a specific instrument, parse the incoming data, and insert into a private circular queue.

3) Create an OrderBook class (friend of GDAXFeedReceiver) that will pop the data from the queue and insert into orderbook.

4) Can have two threads, one that does the stuff in (2) and one that does the stuff in (3). Can the queue be single producer,single consumer lock-free?


gdax api reference:
https://docs.gdax.com/#websocket-feed
