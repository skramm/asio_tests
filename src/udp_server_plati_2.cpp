/**
udp_server_plati_2.cpp
*/



#include "udp_server.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <chrono>

#define BUFFER_SIZE 2048

using boost::asio::ip::udp;


//-----------------------------------------------------------------------------------
/// The different types of events that can happen during the communication process in the platinum project
enum EventType
{
	ET_LOST_CLIENT
};
//-----------------------------------------------------------------------------------
/// Holds an "event", i.e. something that happened. Stored in class PlatiServer so that it can be logged
class Event
{
	public:
		Event( EventType et ): _et(et)
		{
			_time_point = std::chrono::system_clock::now();
		}

	private:
		EventType _et;
		std::chrono::system_clock::time_point _time_point;
};

//-----------------------------------------------------------------------------------
/// holds the 3D mesh data
struct PlatinumData_3D
{
};
/// holds the graph of spheres
struct PlatinumData_graph
{
};

//-----------------------------------------------------------------------------------
/// Server current status state
enum SERSTAT
{
	STAT_WAIT               ///< initial waiting state
	,STAT_GOT_ID            ///< got agent id, waiting for request
	,STAT_GOT_FULL_REQUEST  ///< got all data to complete request
};

//-----------------------------------------------------------------------------------
class PlatiServer : public udp_server<BUFFER_SIZE>
{
	public:
		PlatiServer( boost::asio::io_service& io_service, int port_no )
			:udp_server( io_service, port_no )
		{}

		/// implementation of root class virtual function
		std::vector<BYTE> GetResponse( const udp_server<BUFFER_SIZE>::Buffer_t& buffer ) const
		{
			switch( _status )
			{
				case STAT_WAIT:
					return std::vector<BYTE>{0x01};
				break;
				default:
					assert(0);
			}
		}

	private:
		PlatinumData_3D    _data3d;
		PlatinumData_graph _dataGraph;
		SERSTAT _status;
		std::vector<Event> _vevents; ///< holds all the events
};
//-----------------------------------------------------------------------------------
int
main( int argc, const char** argv )
{
	try
	{
		boost::asio::io_service io_service;
		std::cout << "io_service created\n";

		PlatiServer server( io_service, 12345 );
		std::cout << "server created\n";

		io_service.run();
	}
	catch( std::exception& e )
	{
		std::cerr << "catch error: " << e.what() << std::endl;
	}
}
//-----------------------------------------------------------------------------------

