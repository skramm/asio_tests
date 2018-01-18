/**

http://www.boost.org/doc/libs/master/doc/html/boost_asio.html

Asynchronous send, no reception

*/

// http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/tutorial/tutdaytime4.html

//
// udp_client_1.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "common_header.hpp"

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Timer: Hello, world!" << std::endl;
}

void my_write_handler( const boost::system::error_code& ec, std::size_t bytes_transferred )
{
	std::cout << "Write handler: bt=" << bytes_transferred << "\n";
}

using boost::asio::ip::udp;

int main(int argc, char* argv[])
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

#if 0
//		boost::asio::io_context io;
		boost::asio::deadline_timer t( io_service, boost::posix_time::seconds(5) );
		t.async_wait(&print);
//		std::cout << "io_service.run()\n";
//		io_service.run();
#endif

		int iter(0);


		do

		{
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			std::string str_frame( "frame " + std::to_string(iter) + ": message=" + str + "\n" );

			boost::asio::const_buffers_1 my_buff = boost::asio::buffer( str );

			socket.async_send_to(
				my_buff,            // const ConstBufferSequence & buffers,
				receiver_endpoint,  // const endpoint_type & destination,
				my_write_handler    // WriteHandler && handler);
			);
			std::cout << "async_send_to(): done\n";
			io_service.run();
		}
		while(1);


/*
		do
		{
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			socket.send_to( boost::asio::buffer( str ), receiver_endpoint );

			boost::array<char, 128> recv_buf;
			udp::endpoint sender_endpoint;
			size_t len = socket.receive_from( boost::asio::buffer(recv_buf), sender_endpoint );

			std::cout.write( recv_buf.data(), len );
		}
		while( 1 );
*/
	}
	catch (std::exception& e)
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}

	return 0;
}


