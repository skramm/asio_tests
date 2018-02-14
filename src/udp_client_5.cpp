/**
udp_client_5.cpp

Synchronous send, asynchronous reception and timeout: OK !

see:
 - http://www.boost.org/doc/libs/master/doc/html/boost_asio.html


*/

#include <iostream>
//#include <boost/array.hpp>
#include <array>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "common_header.hpp"

using boost::asio::ip::udp;

#define LOG std::cout << __FUNCTION__ << ": "

//---------------------------------------------------------------------------------------------------
/// callback for async timer
/**
The funny looking switch structure is because this handler gets called even when timer has been canceled.
*/
void timer_handler( const boost::system::error_code& error, udp::socket* socket )
{
	switch( error.value() )
	{
		case boost::system::errc::operation_canceled: // do nothing
		break;
		case 0:
		{
			LOG << "no response from server\n";
			socket->cancel();  // cancel remaing async operations
		}
		break;
		default: // all other values
			LOG << "unexpected error, message=" << error.message() << "\n";
	}
}
//---------------------------------------------------------------------------------------------------
/// callback for async reception
void
read_handler( const boost::system::error_code& error, std::size_t bytes_rx, boost::asio::deadline_timer* timer )
{
	switch( error.value() )
	{
		case boost::system::errc::operation_canceled: // do nothing
		break;
		case 0:
		{
			LOG << "bt=" << bytes_rx << "\n";
			timer->cancel(); // cancel timer as we have received the acknowledge
		}
		break;

		default: // all other values
			LOG << "unexpected error, message=" << error.message() << "\n";
	}
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

			std::cout << "starting timer\n";
			boost::asio::deadline_timer timer( io_service, boost::posix_time::seconds(2));
			timer.async_wait( boost::bind( timer_handler, boost::asio::placeholders::error, &socket ) );

			std::array<char, 128> recv_buf;
			socket.async_receive_from(
				boost::asio::buffer(recv_buf),
				endpoint,
				boost::bind(
					read_handler,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					&timer
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
}
//---------------------------------------------------------------------------------------------------


