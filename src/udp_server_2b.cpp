/**
udp_server_2b.cpp

 callback is outside of class

 WITHOUT std::function
 */

#include <ctime>
#include <iostream>
#include <functional>
#include <string>

#include <array>

#include <boost/bind.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::udp;


//typedef void (*callback_func)( const boost::system::error_code&, std::size_t bytes_rx, int );

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
					&udp_server::_rx_handler,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred,
					42
				)
			);
		}

	protected:
/*		void assignCallback( callback_func f )
		{
			_rx_handler = f;
		}
*/
	protected:
		udp::socket              _socket;
		udp::endpoint            _remote_endpoint;

	private:
		std::array<char, 1024> _recv_buffer;
		bool                   _sendack;
//		callback_func          _rx_handler;

		virtual std::string GetResponse() const = 0;

	void
	_rx_handler( const boost::system::error_code&, std::size_t bytes_rx, int )
	{
		std::string str = GetResponse();
		std::cout << "sending ack:" << str << "\n";
		_socket.send_to(                // synchronous send acknowledge
			boost::asio::buffer(str),
			_remote_endpoint
		);
		start_receive();
	}
};

//-----------------------------------------------------------------------------------
class my_udp_server : public udp_server
{

	private:
		std::string GetResponse() const
		{
			return "my_udp_server!";
		}

//		callback_func _real_rx_handler;

/*		void _rx_handler( const boost::system::error_code& error, std::size_t bytes_rx, int third )
		{
			std::cout << "my_rx_handler() !\n";
			std::cout << "sending ack\n";
			std::string str("ok");
			_socket.send_to(                // synchronous send acknowledge
				boost::asio::buffer(str),
				_remote_endpoint
			);
		}*/

	public:
		my_udp_server(boost::asio::io_service& io_service, int port_no ):
			udp_server( io_service, port_no, true )
		{
		}
};

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

		my_udp_server server( io_service, 12345 );
		std::cout << "server created\n";

//		server.assignCallback( my_rx_handler );
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

