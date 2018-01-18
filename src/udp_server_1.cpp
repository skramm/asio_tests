
/*
http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/tutorial/tutdaytime6.html
http://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/tutorial/tutdaytime6/src.html
*/
//
// udp_server_1.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

//std::string prog( "-server: " );

class my_udp_server
{
	public:
		my_udp_server( boost::asio::io_service& io_service, int port_no )
			: _socket( io_service, udp::endpoint( udp::v4(), port_no ) )
		{
			start_receive();
		}

	private:
		void start_receive()
		{
			_socket.async_receive_from(
				boost::asio::buffer( _recv_buffer),
				_remote_endpoint,
				boost::bind(
					&my_udp_server::handler_rx,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
		}

		static void handler_tx(
			const boost::system::error_code& error, // Result of operation.
			std::size_t bytes_transferred           // Number of bytes sent.
		)
		{
			std::cout << "handler_tx(): bytes_transferred=" << bytes_transferred << "\n";
		}

	void handler_rx( const boost::system::error_code& error, std::size_t bytes_rx )
	{
		std::cout << "handle_receive(), bt=" << bytes_rx << "\n";
		if( !error || error == boost::asio::error::message_size )
		{
			std::cout << "bt=" << bytes_rx << " bytes\n";

			std::cout << "data:*";
			std::cout.write( _recv_buffer.data(), bytes_rx );
			std::cout << "*";

			std::string str( "ok\n" );
#if 0
			_socket.async_send_to(
				boost::asio::buffer(str),
				_remote_endpoint,
				handler_tx
			);
#else
			_socket.send_to(
				boost::asio::buffer(str),
				_remote_endpoint
			);
#endif
			start_receive();
		}
		else
			std::cout << "error on receive\n";
	}

//	void handle_send(boost::shared_ptr<std::string> /*message*/, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/ )
//	{
//		std::cout << prog << "handle_send()\n";
//	}
//
	udp::socket   _socket;
	udp::endpoint _remote_endpoint;
	boost::array<char, 1024> _recv_buffer;
};

int main()
{
	try
	{
		boost::asio::io_service io_service;
		std::cout << "io_service created\n";

		my_udp_server server( io_service, 12345 );
		std::cout << "server created\n";

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}

return 0;
}
