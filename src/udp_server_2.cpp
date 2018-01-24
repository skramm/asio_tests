/**
 udp_server_2.cpp

 callback is outside of class

 */

#include <ctime>
#include <iostream>
#include <functional>
#include <string>

#include <array>

#include <boost/bind.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

typedef std::function< void( const boost::system::error_code&, std::size_t bytes_rx, int ) > CALLBACK_T;

//#define THIS_WORKS

//-----------------------------------------------------------------------------------
class udp_server
{
	public:
		udp_server( boost::asio::io_service& io_service, int port_no, bool sendack=false )
			: _socket( io_service, udp::endpoint( udp::v4(), port_no ) ), _sendack(sendack)
		{
		}

		void start_receive()
		{
			_socket.async_receive_from(
				boost::asio::buffer( _recv_buffer),
				_remote_endpoint,
				boost::bind(
					udp_server::_rx_handler,
//					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					42
				)
			);
		}

#ifndef THIS_WORKS
		void assignCallback( CALLBACK_T f )
		{
			_rx_handler = f;
		}
#endif

	protected:
		udp::socket              _socket;
		udp::endpoint            _remote_endpoint;

	private:
		std::array<char, 1024> _recv_buffer;
		bool                     _sendack;

#ifdef THIS_WORKS
		void _rx_handler( const boost::system::error_code&, std::size_t bytes_rx, int );
#else
		CALLBACK_T      _rx_handler;
#endif
};

//-----------------------------------------------------------------------------------
#if 1
class my_udp_server : public udp_server
{
	private:
		void my_rx_handler( const boost::system::error_code& error, std::size_t bytes_rx, int third )
		{
			std::cout << "my_rx_handler() !\n";
			std::cout << "sending ack\n";
			std::string str("ok");
			_socket.send_to(                // synchronous send acknowledge
				boost::asio::buffer(str),
				_remote_endpoint
			);
		}

	public:
		my_udp_server(boost::asio::io_service& io_service, int port_no )
			: udp_server( io_service, port_no, true )
		{
			assignCallback( CALLBACK_T(&my_udp_server::my_rx_handler) );
		}
};
#endif
//-----------------------------------------------------------------------------------
int
main( int argc, const char** argv )
{
/*	bool sendack(false);
	if( argc > 1 )
	{
		if( std::string(argv[1]) == "SENDACK" )
		sendack = true;
	}*/

	try
	{
		boost::asio::io_service io_service;
		std::cout << "io_service created\n";
/*
		my_udp_server server( io_service, 12345 );
		std::cout << "server created\n";

//		server.assignCallback( my_rx_handler );
		server.start_receive();
		std::cout << "server started\n";

		io_service.run();
*/
	}
	catch( std::exception& e )
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}
}
//-----------------------------------------------------------------------------------

