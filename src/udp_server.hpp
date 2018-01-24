/**
\file udp_server.hpp
\brief holds definition of class udp_server
S. Kramm - 2017/01
*/
#ifndef HG_UDP_SERVER_HPP
#define HG_UDP_SERVER_HPP

#include <array>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

typedef unsigned char BYTE;
//-----------------------------------------------------------------------------------
/// A udp server, templated by size of buffer
/**
Callback: upon each reception of data, a callback is called/

The user of this class needs to provide a function GetResponse()
that will return the data thats needs to be send back.

*/
template<size_t BUF_SIZE>
class udp_server
{
	public:
		typedef std::array<BYTE, BUF_SIZE> Buffer_t;

		udp_server( boost::asio::io_service& io_service, int port_no ) //, bool sendack=false )
			: _socket( io_service, boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(), port_no ) ) //, _sendack(sendack)
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

	private:
		boost::asio::ip::udp::socket    _socket;
		boost::asio::ip::udp::endpoint _remote_endpoint;

	protected:
		Buffer_t _recv_buffer;
//		bool                   _sendack;

		virtual std::vector<BYTE> GetResponse( const Buffer_t& buffer ) const = 0;

/// Callback function. Inherited classes can access the received data and need to provide a GetResponse() function
	void _rx_handler( const boost::system::error_code&, std::size_t bytes_rx, int )
	{
		std::vector<BYTE> v = GetResponse( _recv_buffer );
		std::cout << "sending ack:" << std::string(v.begin(),v.end()) << "\n";
		_socket.send_to(                // synchronous send acknowledge
			boost::asio::buffer( v ),
			_remote_endpoint
		);
		start_receive();
	}
};
//-----------------------------------------------------------------------------------

#endif // HG_UDP_SERVER_HPP
