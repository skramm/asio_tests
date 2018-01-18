/**
udp_client_4.cpp

Asynchronous send and reception: WIP

see:
 - http://www.boost.org/doc/libs/master/doc/html/boost_asio.html


*/


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "common_header.hpp"

using boost::asio::ip::udp;

//---------------------------------------------------------------------------------------------------
void
read_handler( const boost::system::error_code& /*error*/, std::size_t bytes_rx )
{
	std::cout << "read_handler() bt=" << bytes_rx << "\n";
}
//---------------------------------------------------------------------------------------------------
void
write_handler( const boost::system::error_code& ec, std::size_t bytes_tx, udp::socket* socket )
{
	std::cout << "write_handler(): bt=" << bytes_tx << "\n";

	boost::array<char, 128> recv_buf;
	udp::endpoint sender_endpoint;
	socket->async_receive_from( boost::asio::buffer(recv_buf), sender_endpoint, read_handler );
}

using boost::asio::ip::udp;

//---------------------------------------------------------------------------------------------------
int
main( int argc, char* argv[] )
{
	std::cout << GetBoostVersion();
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), argv[1], "12345" );
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		std::cout << "endpoint: " << receiver_endpoint << "\n";
		udp::socket socket( io_service );
		socket.open( udp::v4() );

		int iter(0);
		do
		{
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			std::string str_frame( "frame " + std::to_string(iter++) + ": message=" + str + "\n" );

			socket.async_send_to(
				boost::asio::buffer( str_frame ),
				receiver_endpoint,
				boost::bind(
					write_handler,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					&socket
				)
			);
			io_service.run();
		}
		while(1);
	}
	catch (std::exception& e)
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------


