/**
udp_server_plati_1.cpp
*/


#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#define DO_SEND_ACK

using boost::asio::ip::udp;

//-----------------------------------------------------------------------------------
/// Generic udp server, may send acknowledge
class my_udp_server
{
	public:
		my_udp_server(
			boost::asio::io_service& io_service,
			int port_no,
			bool sendack=false
		)
			: _socket( io_service, udp::endpoint( udp::v4(), port_no ) ), _sendack(sendack)
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
#ifdef USE_ASYNC_SEND
		static void handler_tx(
			const boost::system::error_code& error, // Result of operation.
			std::size_t bytes_transferred           // Number of bytes sent.
		)
		{
			std::cout << "handler_tx(): bytes_transferred=" << bytes_transferred << "\n";
		}
#endif

	void handler_rx( const boost::system::error_code& error, std::size_t bytes_rx )
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
#ifdef USE_ASYNC_SEND
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
	}

	udp::socket   _socket;
	udp::endpoint _remote_endpoint;
	boost::array<char, 1024> _recv_buffer;
	bool _sendack;
};
//-----------------------------------------------------------------------------------
/// Server current status state
enum SERSTAT
{
	STAT_WAIT,              ///< initial waiting state
	STAT_GOT_ID,            ///< got agent id, waiting for request
	STAT_GOT_FULL_REQUEST,  /// got all data to complete request
};
//-----------------------------------------------------------------------------------
class PlatiServer : public my_udp_server
{
	public:
		PlatiServer( boost::asio::io_service& io_service, int port_no )
			:my_udp_server( io_service, port_no )
		{}

	private:
		SERSTAT _status;
};
//-----------------------------------------------------------------------------------
int main( int argc, const char** argv )
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

		PlatiServer server( io_service, 12345 );
		std::cout << "server created\n";

		io_service.run();
	}
	catch( std::exception& e )
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}
}
//-----------------------------------------------------------------------------------

