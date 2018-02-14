/**
thread_1.cpp
*/

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <mutex>

static std::mutex mut;

void call_from_thread()
{
	std::cout << "Thread start" << std::endl;
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(4));
	timer.wait();
	std::cout << "thread end\n";

}

void print(const boost::system::error_code& /*e*/)
{
	std::cout << "Timer end!" << std::endl;
}

int main()
{
	std::cout << "- Launch thread\n";
	std::thread t1(call_from_thread);

	std::cout << "- start loop\n";
	boost::asio::io_service io;
	for( int i=0; i<5; i++ )
	{
		io.reset();
		std::cout << "- start timer " << i << "\n";
		boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
		t.async_wait(&print);
		io.run();
	}


	std::cout << "wait for join()\n";


// Join the thread with the main thread
	t1.join();

}


