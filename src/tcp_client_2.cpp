
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

		char k;
		std::cout << "enter 1 or 2:";
		std::cin >> k;


		std::array<char, 5> buf_tx;
		buf_tx[0] = k - '0';
		std::cout << "first byte=" << (int)buf_tx[0] << '\n';
		boost::system::error_code error;

		size_t len1 = socket.send( boost::asio::buffer(buf_tx,1) );
		std::cout << "TX " << len1 << " bytes\n";

		std::array<char, 128> buf_rx;
		size_t len2 = socket.receive( boost::asio::buffer(buf_rx) );
		std::cout << "RX " << len2 << " bytes\n";
		std::string rx_data;
		rx_data.resize(len2);
		std::copy( buf_rx.begin(), buf_rx.begin()+len2, rx_data.begin() );
		std::cout << "data " << rx_data << '\n';

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
