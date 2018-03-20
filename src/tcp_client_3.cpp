#include <iostream>
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string("127.0.0.1"), 12345
    );
    std::cout <<"endpoint ok\n";
    socket.connect(endpoint);
    std::cout << "connected\n";
}




