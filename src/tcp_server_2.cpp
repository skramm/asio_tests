// from http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html

#include <iostream>
#include <string>
#include <boost/asio.hpp>


#include "boost/asio/ip/tcp.hpp"

using boost::asio::ip::tcp;

//-----------------------------------------------------------------------------------
class tcp_server
{
	public:
		tcp_server( boost::asio::io_service& io_service, int port )
			:_acceptor( io_service, tcp::endpoint( tcp::v4(), port ) )
			,_socket(io_service)
		{
//			start_accept();
		}
		void allocate( int n )
		{
			_buff.resize( n*1000 );
		}

		void start()
		{
			while(1)
			{
				_socket.close();

//				std::cout << "listen...\n";
//				_acceptor.listen();

				std::cout << "accept...\n";
				_acceptor.accept( _socket );   // wait and listen

				std::cout << "Reading data\n";

				boost::system::error_code err;

				std::size_t nb_bytes =_socket.read_some( boost::asio::buffer(_buff), err );

				std::cout << "RX " << nb_bytes << " bytes, err=" << err.message() << '\n';
				std::cout << "data " << (int)_buff[0] << ", " << (int)_buff[1] << "\n";


				std::array<char, 128> buf2 = {5,6};
				size_t len1 = _socket.send( boost::asio::buffer(buf2) );
				std::cout << "TX " << len1 << " bytes\n";
//				_acceptor.close();

			}
		}

	private:
		std::vector<char> _buff;
		tcp::acceptor     _acceptor;
		tcp::socket       _socket;
};
//-----------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	try
	{
		boost::asio::io_service io_service;
		tcp_server server( io_service, 12345 );
		server.allocate( 20 ); // nb of kB of buffer
		server.start();
//		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
