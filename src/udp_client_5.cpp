/**
udp_client_5.cpp

Synchronous send, asynchronous reception: OK !

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
void timer_print( const boost::system::error_code& /*e*/)
{
	std::cout << "Timer:no response from server\n";
}
void timer_print_2( const boost::system::error_code& /*e*/, udp::socket* socket )
{
	std::cout << "Timer:no response from server\n";
	socket->cancel();
}

//---------------------------------------------------------------------------------------------------
void
read_handler( const boost::system::error_code& /*error*/, std::size_t bytes_rx )
{
	std::cout << "read_handler() bt=" << bytes_rx << "\n";
}


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
		udp::endpoint endpoint = *resolver.resolve(query);

		std::cout << "endpoint: " << endpoint << "\n";
		udp::socket socket( io_service );
		socket.open( udp::v4() );

		int iter(0);
		do
		{
			io_service.reset();
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			std::string str_frame( "frame " + std::to_string(iter++) + ": message=" + str );

			socket.send_to(
				boost::asio::buffer( str_frame ),
				endpoint
			);
			std::cout << "-data is sent\n";
			boost::array<char, 128> recv_buf;
			socket.async_receive_from( boost::asio::buffer(recv_buf), endpoint, read_handler );

			std::cout << "starting timer\n";
			boost::asio::deadline_timer timer( io_service, boost::posix_time::seconds(2));
#if 1
			timer.async_wait( &timer_print );
#else
			timer.async_wait(
					boost::bind(
						&timer_print_2,
						&socket
					);
			);
#endif
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


