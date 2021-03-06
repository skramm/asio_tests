// from http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>


#include "boost/asio/ip/tcp.hpp"

using boost::asio::ip::tcp;

//-----------------------------------------------------------------------------------

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

 #if 0
class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
	public:
		typedef boost::shared_ptr<tcp_connection> pointer;

		static pointer create(boost::asio::io_service& io_service)
		{
			return pointer(new tcp_connection(io_service));
		}

		tcp::socket& socket()
		{
			return socket_;
		}

		void start()
		{
			std::cout << "tcp_connection::start\n";
			message_ = make_daytime_string();

			boost::asio::async_write( socket_, boost::asio::buffer(message_),
				boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}

	private:
	tcp_connection(boost::asio::io_service& io_service) : socket_(io_service)
	{}

	void handle_write(const boost::system::error_code& /*error*/,  size_t /*bytes_transferred*/)
	{}

	tcp::socket socket_;
	std::string message_;
};
#endif
//-----------------------------------------------------------------------------------
class tcp_server
{
	public:
		tcp_server(boost::asio::io_service& io_service, int port )
			: acceptor_( io_service, tcp::endpoint( tcp::v4(), port ) )
			,socket_(io_service)
		{
			start_accept();
		}

	private:
		void start_accept()
		{
			std::cout << __FUNCTION__ << "()\n";
//			tcp_connection::pointer new_connection = tcp_connection::create( acceptor_.get_io_service() );

			acceptor_.async_accept(
				socket_,
				boost::bind(
					&tcp_server::handle_accept,
					this,
//					new_connection,
					boost::asio::placeholders::error
				)
			);
		}

		void handle_accept( /*tcp_connection::pointer new_connection, */ const boost::system::error_code& error )
		{
			std::cout << __FUNCTION__ << "()\n";

			if (!error)
			{
				std::cout << "send ack\n";
//				new_connection->start();
			}

			start_accept();
		}

		tcp::acceptor acceptor_;
		tcp::socket socket_;

};
//-----------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	try
	{
		boost::asio::io_service io_service;
		tcp_server server( io_service, 12345 );
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
