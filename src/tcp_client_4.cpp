#include <iostream>
#include <string>
#include <boost/asio.hpp>

//-----------------------------------------------------------------------------------
struct Client_TCP
{
		Client_TCP() : _socket( _io_service ) {}

		void connect( std::string host, int port )
		{
			boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::address::from_string(host), port);
			boost::system::error_code ec;
			_socket.connect( endpoint );
		}

		boost::asio::io_service      _io_service;
		boost::asio::ip::tcp::socket _socket;
};

//-----------------------------------------------------------------------------------
int main()
{
	std::cout << "start\n";
	Client_TCP sender;
	sender.connect( "127.0.0.1", 12345 );
	std::cout << "Connected !\n";
}
//-----------------------------------------------------------------------------------
