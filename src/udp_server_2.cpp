/**
 udp_server_2.cpp

 callback is outside of class

 */

#include <ctime>
#include <iostream>
#include <functional>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#define DO_SEND_ACK

using boost::asio::ip::udp;

//-----------------------------------------------------------------------------------
class my_udp_server
{
	public:
		my_udp_server( boost::asio::io_service& io_service, int port_no, bool sendack=false )
			: _socket( io_service, udp::endpoint( udp::v4(), port_no ) ), _sendack(sendack)
		{
//			start_receive();
		}

		void start_receive()
		{
			_socket.async_receive_from(
				boost::asio::buffer( _recv_buffer),
				_remote_endpoint,
#if 0
				boost::bind(
					my_udp_server::_rx_handler,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
#else
				my_udp_server::_rx_handler
#endif
			);
		}
	private:

/*	void handler_rx( const boost::system::error_code& error, std::size_t bytes_rx )
	{
		static int iter(1);
		std::cout << "rx handler(), call " << iter++ << "\n";
		if( !error || error == boost::asio::error::message_size )
		{
			std::cout << "bt=" << bytes_rx << " bytes, data=*";
			std::cout.write( _recv_buffer.data(), bytes_rx );
			std::cout << "*\n";

			if( _sendack )
			{
				std::string str( "ok\n" );
#if 0
				_socket.async_send_to(          // asynchronous send acknowledge
					boost::asio::buffer(str),
					_remote_endpoint,
					handler_tx
				);
#else
				std::cout << "sending ack\n";
				_socket.send_to(                // synchronous send acknowledge
					boost::asio::buffer(str),
					_remote_endpoint
				);
#endif
			}
			start_receive();
		}
		else
			std::cout << "error on receive\n";
	}*/

	udp::socket   _socket;
	udp::endpoint _remote_endpoint;
	boost::array<char, 1024> _recv_buffer;
	bool _sendack;
	std::function< void(const boost::system::error_code&, std::size_t bytes_rx ) > _rx_handler;
};

//-----------------------------------------------------------------------------------
void
my_rx_handler( const boost::system::error_code&, std::size_t bytes_rx )
{
	std::cout << "my_rx_handler()\n";
}

int main( int argc, const char** argv )
{
	bool sendack(false);
	if( argc > 1 )
	{
		if( std::string(argv[1]) == "SENDACK" )
		sendack = true;
	}

	try
	{
		boost::asio::io_service io_service;
		std::cout << "io_service created, " << (sendack?"sending ack": "no ack send") << "\n";

		my_udp_server server( io_service, 12345, sendack );
		std::cout << "server created\n";

		server.start_receive();
		std::cout << "server started\n";

		io_service.run();
	}
	catch( std::exception& e )
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}
}
//-----------------------------------------------------------------------------------

