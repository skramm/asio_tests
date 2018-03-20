/**
tst_tcp_server_v2.cpp
*/
#include "tcp_server_v2.hpp"

//-----------------------------------------------------------------------------------
struct MyTcpServer: public TcpServer_v2
{
	MyTcpServer( boost::asio::io_service& io_service, int port ) : TcpServer_v2( io_service, port )
	{}

	std::pair<std::vector<BYTE>,bool> getResponseData() const
	{
		std::vector<BYTE> out;
//		std::cout << " received " << nb_bytes << " bytes, first one=" << (int)_rx_buff[0] << '\n';
		switch( _rx_buff[0] )
		{
			case 1:
			{
				std::string s( "abc" );
				out.resize( s.size() );
				std::copy( s.begin(), s.end(), out.begin() );
			}
			break;
			case 2:
			{
				std::string s( "def" );
				out.resize( s.size() );
				std::copy( s.begin(), s.end(), out.begin() );
			}
			break;
			default: std::cout << "client send invalid data\n";
		}
		return std::make_pair(out, true);
	}
};
//-----------------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	try
	{
		boost::asio::io_service io_service;
		MyTcpServer server( io_service, 12345 );
		server.allocate( 20 ); // nb of kB of buffer
		server.start();
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

//-----------------------------------------------------------------------------------
