
//http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/reference/basic_datagram_socket/async_send_to/overload1.html


#include <iostream>
#include <boost/asio.hpp>


void my_write_handler( const boost::system::error_code& ec, std::size_t bytes_transferred )
{
	std::cout << "Write handler: bt=" << bytes_transferred << "\n";
}

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), "localhost", "12345" );
	udp::endpoint receiver_endpoint = *resolver.resolve(query);

	std::cout << "endpoint: " << receiver_endpoint << "\n";
	udp::socket socket( io_service );
	socket.open( udp::v4() );

	std::string str( "hi there, I'm the client\n" );

	boost::asio::const_buffers_1 my_buff = boost::asio::buffer( str );

	socket.async_send_to(
		my_buff,            // const ConstBufferSequence & buffers,
		receiver_endpoint,  // const endpoint_type & destination,
		my_write_handler    // WriteHandler && handler);
	);
	std::cout << "async_send_to(): done\n";
	io_service.run();
}



