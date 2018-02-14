
//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: client <host>" << std::endl;
		return 1;
	}
	try
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query( argv[1], "12345" );
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		std::cout << "connecting socket...\n";
		boost::asio::connect(socket, endpoint_iterator);

		{
			std::cout << "loop start\n";

			std::array<char, 100000> buf1 = {1,2,3,4};
			boost::system::error_code error;

			size_t len1 = socket.send( boost::asio::buffer(buf1) );
			std::cout << "TX " << len1 << " bytes\n";

			std::array<char, 128> buf2 = {55,66};
			size_t len2 = socket.receive( boost::asio::buffer(buf2) );
			std::cout << "RX " << len2 << " bytes\n";
			std::cout << "data " << (int)buf2[0] << ", " << (int)buf2[1] << "\n";
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
