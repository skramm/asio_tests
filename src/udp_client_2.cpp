/**

http://www.boost.org/doc/libs/master/doc/html/boost_asio.html

Asynchronous send, no reception

*/

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "common_header.hpp"

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Timer: Hello, world!" << std::endl;
}

void my_write_handler( const boost::system::error_code& ec, std::size_t bytes_transferred )
{
	std::cout << "Write handler: bt=" << bytes_transferred << "\n";
}

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


		int iter(0);
		do
		{
			std::string str;
			std::cout << "enter string: ";
			std::cin >> str;
			std::string str_frame( "frame " + std::to_string(iter) + ": message=" + str );

			boost::asio::const_buffers_1 my_buff = boost::asio::buffer( str );

			socket.async_send_to(
				my_buff,            // const ConstBufferSequence & buffers,
				receiver_endpoint,  // const endpoint_type & destination,
				my_write_handler    // WriteHandler && handler);
			);
			std::cout << "async_send_to(): done\n";
			io_service.run();
		}
		while(1);
	}
	catch (std::exception& e)
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}

	return 0;
}


