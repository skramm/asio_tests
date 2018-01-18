/**

Asynchronous send, synchronous reception: OK !

see:
 - http://www.boost.org/doc/libs/master/doc/html/boost_asio.html


*/


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "common_header.hpp"

using boost::asio::ip::udp;


void write_handler( const boost::system::error_code& ec, std::size_t bytes_transferred, udp::socket* socket )
{
	std::cout << "Write handler: bt=" << bytes_transferred << "\n";

	boost::array<char, 128> recv_buf;
	udp::endpoint sender_endpoint;
	size_t len = socket->receive_from( boost::asio::buffer(recv_buf), sender_endpoint );

	std::cout << "RX from " << sender_endpoint << " : ";
	std::cout.write( recv_buf.data(), len );
	std::cout << "\n";
}

using boost::asio::ip::udp;

//---------------------------------------------------------------------------------------------------
int main( int argc, char* argv[] )
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
			std::string str_frame( "frame " + std::to_string(iter) + ": message=" + str + "\n" );

			boost::asio::const_buffers_1 my_buff = boost::asio::buffer( str );

			socket.async_send_to(
				my_buff,
				receiver_endpoint,
				boost::bind(
					write_handler,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					&socket
				)
			);

			std::cout << "async_send_to(): done\n";
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


