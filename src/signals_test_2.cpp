/**
\file signals_test_2.cpp
\brief using boost asio io_service

*/

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>

boost::asio::io_service io_service;
boost::asio::signal_set signals(io_service, SIGUSR1 );

// signal handler
void handler( const boost::system::error_code& error , int signal_number )
{
	static int c;
	std::cout << "handling signal " << signal_number << " c=" << c++ << " errorcode=" << error.message() << std::endl;

	signals.async_wait( handler );

	if( c==4 )
		;
//  exit(1);
}

int main( int argc , char** argv )
{
	std::cout << "start\n";
	// Construct a signal set registered for process termination.

	// Start an asynchronous wait for one of the signals to occur.
	signals.async_wait( handler );

	boost::asio::io_service::work work( io_service );
	io_service.run();
	std::cout << "exit\n";

}

