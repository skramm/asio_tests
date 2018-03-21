/**
\file tcp_server_v2b.hpp

attemp of async server

taken from
http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html
and adapted

moved socket from TcpConnection to TcpServer
*/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

typedef unsigned char BYTE;

#ifdef DEBUG_MODE
	#define SERVER_BEGIN if(1) std::cout << "* start " <<  __PRETTY_FUNCTION__ << '\n'
	#define SERVER_END   if(1) std::cout << "* end "   <<  __PRETTY_FUNCTION__ << '\n'
	#define SERVER_LOG   if(1) std::cout << __PRETTY_FUNCTION__ << ": "
#else
	#define SERVER_BEGIN if(0) std::cout << "* start " <<  __PRETTY_FUNCTION__ << '\n'
	#define SERVER_END   if(0) std::cout << "* end "   <<  __PRETTY_FUNCTION__ << '\n'
	#define SERVER_LOG   if(0) std::cout << __PRETTY_FUNCTION__ << ": "
#endif

//-----------------------------------------------------------------------------------
class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
	public:
		typedef boost::shared_ptr<TcpConnection> PtrConn;

		static PtrConn createConn(boost::asio::io_service& io_service)
		{
			return PtrConn( new TcpConnection(io_service) );
		}

		void sendMessage( tcp::socket& socket, const std::vector<BYTE>& message )
		{
			SERVER_BEGIN;
//			std::string message_ = "aaaaa";
			boost::asio::async_write(
				socket,
				boost::asio::buffer( message ),
				boost::bind(
					&TcpConnection::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
			SERVER_END;
		}

	private:
		TcpConnection(boost::asio::io_service& io_service)//: socket_(io_service)
		{}

		void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
		{
			SERVER_LOG << "data has been written\n";
		}
};

//-----------------------------------------------------------------------------------
class TcpServer_v2
{
	public:
		TcpServer_v2( boost::asio::io_service& io_service, int port )
			: acceptor_( io_service, tcp::endpoint( tcp::v4(), port ) )
			, _socket(io_service)
		{}

/// allocates \c n kB
		void allocate( int n )
		{
			_rx_buff.resize( n*1000 );
		}

		virtual std::pair<std::vector<BYTE>,bool> getResponseData() const = 0;

		void start()
		{
			TcpConnection::PtrConn new_connection = TcpConnection::createConn( acceptor_.get_io_service() );

			SERVER_BEGIN;
			acceptor_.async_accept(
				_socket,
				boost::bind(
					&TcpServer_v2::handle_accept,
					this,
					new_connection,
					boost::asio::placeholders::error
				)
			);
			SERVER_END;
		}

	private:

/// The function handle_accept() is called when the asynchronous accept operation initiated by start() finishes.
/// It services the client request, and then calls start() to initiate the next accept operation.
		void handle_accept( TcpConnection::PtrConn new_connection, const boost::system::error_code& error )
		{
			SERVER_BEGIN;
			if( !error )
			{
				boost::system::error_code sock_ec;
				_rx_nb_bytes = _socket.read_some( boost::asio::buffer( _rx_buff ), sock_ec );
				if( !sock_ec )
				{
					std::cout << " read " << _rx_nb_bytes <<  " bytes\n";
					auto resp_data = getResponseData();
					if( resp_data.second )
						new_connection->sendMessage( _socket, resp_data.first );
				}
				else
				{
					std::cerr << "Error in handle_accept(): socket.read_some(): " << sock_ec.message() << "\n";
				}
				_socket.close();
			}
			else
			{
				std::cerr << "Error in handle_accept(): " << error.message() << ", restart server\n";
			}
			start();
			SERVER_END;
		}

		tcp::acceptor acceptor_;
		tcp::socket       _socket;

	protected:
		std::vector<BYTE> _rx_buff;    ///< received data is stored here (\warning nb of bytes read is NOT size of vector)
		size_t            _rx_nb_bytes; ///< nb of bytes read


};
//-----------------------------------------------------------------------------------
