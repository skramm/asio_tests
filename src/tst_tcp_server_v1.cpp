/**
tst_tcp_server_v1.cpp
*/
#include "tcp_server_v1.hpp"

//-----------------------------------------------------------------------------------
struct MyTcpServer: public TcpServer_v1
{
	MyTcpServer( boost::asio::io_service& io_service, int port ) : TcpServer_v1( io_service, port )
	{}

	std::vector<BYTE> getResponseData() const
	{
		std::vector<BYTE> out;
		std::cout << "first byte=" << (int)_rx_buff[0] << '\n';
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
		return out;
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
//		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

//-----------------------------------------------------------------------------------
