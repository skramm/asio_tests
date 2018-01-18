/**

synchronous send and receive


http://www.boost.org/doc/libs/master/doc/html/boost_asio.html
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
//#include <boost/date_time/posix_time/posix_time.hpp>
#include "common_header.hpp"


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

		do
		{
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			socket.send_to( boost::asio::buffer( str ), receiver_endpoint );

			boost::array<char, 128> recv_buf;
			udp::endpoint sender_endpoint;
			size_t len = socket.receive_from( boost::asio::buffer(recv_buf), sender_endpoint );

			std::cout << "RX from " << sender_endpoint << " : ";
			std::cout.write( recv_buf.data(), len );
			std::cout << "\n";
		}
		while( 1 );
	}
	catch (std::exception& e)
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}

	return 0;
}


