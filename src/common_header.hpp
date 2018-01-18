// common_header.hpp
#include <string>

/// from https://stackoverflow.com/questions/3708706/
std::string
GetBoostVersion()
{
	std::string dot( "." );
	return std::string( "Boost "
          + std::to_string( BOOST_VERSION / 100000     ) + dot  // major version
          + std::to_string( BOOST_VERSION / 100 % 1000 ) + dot  // minor version
          + std::to_string( BOOST_VERSION % 100 )               // patch level
          + '\n');
}
