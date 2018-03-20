/**
\file tcp_server_v1.hpp
*/

#include <boost/asio.hpp>


#include "boost/asio/ip/tcp.hpp"


using boost::asio::ip::tcp;
typedef unsigned char BYTE;
//-----------------------------------------------------------------------------------
/// A class implementing a synchronous TCP server
class TcpServer_v1
{
	public:
		TcpServer_v1( boost::asio::io_service& io_service, int port )
			:_acceptor( io_service, tcp::endpoint( tcp::v4(), port ) )
			,_socket(io_service)
		{
		}
/// allocates \c n kB
		void allocate( int n )
		{
			_rx_buff.resize( n*1000 );
		}

		virtual std::pair<std::vector<BYTE>,bool> getResponseData() const = 0;
		void start()
		{
			assert( _rx_buff.size() );
			boost::system::error_code err;
			do
			{
				_socket.close();
				std::cout << "accept...\n";
				_acceptor.accept( _socket );

				std::cout << "got something!\n";

				std::size_t nb_bytes =_socket.read_some( boost::asio::buffer(_rx_buff), err );

				std::cout << "RX " << nb_bytes << " bytes, err=" << err.message() << '\n';
				if( !err )
				{
					auto resp = getResponseData();

					size_t len_tx = _socket.send( boost::asio::buffer(resp.first) );
					std::cout << "TX: " << len_tx << " bytes\n";
				}
				else
					std::cout << "Error, quiting\n";
			}
			while( !err );
		}

	protected:
		std::vector<char> _rx_buff;

	private:
		tcp::acceptor _acceptor;
		tcp::socket   _socket;
};

