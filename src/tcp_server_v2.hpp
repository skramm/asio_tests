/**
\file tcp_server_v2.hpp

attemp of async server

taken from
http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html
and adapted
*/


//#include <boost/asio/ip/tcp.hpp>





#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

typedef unsigned char BYTE;

//#define LOG std::cout << __FUNCTION__ << ": "
#define LOG std::cout << __PRETTY_FUNCTION__ << ": "

//-----------------------------------------------------------------------------------
class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
	public:
		typedef boost::shared_ptr<TcpConnection> p_conn;

		static p_conn create(boost::asio::io_service& io_service)
		{
			return p_conn(new TcpConnection(io_service));
		}

		tcp::socket& socket()
		{
			return socket_;
		}

		void start()
		{
			LOG << "begin\n";
			message_ = "aaaaa";
			boost::asio::async_write(
				socket_,
				boost::asio::buffer(message_),
				boost::bind(
					&TcpConnection::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
			LOG << "end\n";
		}

	private:
		TcpConnection(boost::asio::io_service& io_service) : socket_(io_service)
		{}

		void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
		{
			LOG << "\n";
		}

		tcp::socket socket_;
		std::string message_;
};

//-----------------------------------------------------------------------------------
class TcpServer_v2
{
	public:
		TcpServer_v2( boost::asio::io_service& io_service, int port )
			: acceptor_( io_service, tcp::endpoint( tcp::v4(), port ) )
		{}

/// allocates \c n kB
		void allocate( int n )
		{
			_rx_buff.resize( n*1000 );
		}

		virtual std::pair<std::vector<BYTE>,bool> getResponseData() const = 0;

		void start()
		{
			TcpConnection::p_conn new_connection = TcpConnection::create(acceptor_.get_io_service());

			LOG << "begin\n";
			acceptor_.async_accept(
				new_connection->socket(),
				boost::bind(
					&TcpServer_v2::handle_accept,
					this,
					new_connection,
					boost::asio::placeholders::error
				)
			);
			LOG << "end\n";
		}

	private:

/// The function handle_accept() is called when the asynchronous accept operation initiated by start() finishes.
/// It services the client request, and then calls start() to initiate the next accept operation.
		void handle_accept( TcpConnection::p_conn new_connection, const boost::system::error_code& error )
		{
			std::cout << "\n";
			if (!error)
			{
				new_connection->start();
			}
			start();
		}

		tcp::acceptor acceptor_;

	protected:
		std::vector<char> _rx_buff;

};
//-----------------------------------------------------------------------------------
