poc: poc.cpp
	g++ --std=c++11 poc.cpp -lboost_system -lpthread -lssl -lcrypto -I .
