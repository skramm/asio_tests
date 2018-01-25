/**
bgl_serial_1.cpp

test of serializing a graph

*/

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/list.hpp>
#include <boost/graph/adj_list_serialize.hpp>

//-------------------------------------------------------------------------------------------
/// Some typedefs for readability
typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS
	> graph_t;

typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef boost::graph_traits<graph_t>::edge_descriptor   edge_t;


//-------------------------------------------------------------------
graph_t
GenerateRandomGraph( size_t nb_vertices, size_t nb_egdes )
{
	graph_t g;
	auto current_time = time(0);
	std::srand( current_time );

	for( size_t i=0;i<nb_vertices; i++ )
		add_vertex(g);

	for( size_t i=0;i<nb_egdes; i++ )
	{
		int v1 = static_cast<int>(std::rand()*1.0/RAND_MAX*nb_vertices );
		int v2 = static_cast<int>(std::rand()*1.0/RAND_MAX*nb_vertices );
		if( v1 != v2 )
		{
			auto p = boost::edge( v1, v2, g );  // returns a pair<edge_descriptor, bool>
			if( !p.second )                     // add edge only if not already present
				boost::add_edge( v1, v2, g );
		}
	}
	return g;
}
//-------------------------------------------------------------------
/*
namespace boost {
namespace serialization {

template<class Archive>
void serialize( Archive & ar, const graph_t& g, const unsigned int version )
{
    ar << g;
}

} // namespace serialization
} // namespace boost
*/

//-------------------------------------------------------------------
void
SaveGraph( const graph_t& g, std::string filename )
{
	std::ofstream fs( filename );
	boost::archive::text_oarchive oa( fs );
	oa << g;
}
//-------------------------------------------------------------------
graph_t
ReadGraph( std::string filename )
{
	graph_t g;
	std::ifstream fs( filename );
	boost::archive::text_iarchive ia( fs );
	ia >> g;
	return g;
}
//-------------------------------------------------------------------
int main( int argc, const char** argv )
{
//	SHOW_INFO;

	std::string filename( "mygraph.bgl");
	{
		size_t nb_egdes = 15;
		size_t nb_vertices = 8;
		if( argc > 1 )
			nb_vertices = std::atoi( argv[1] );
		if( argc > 2 )
			nb_egdes = std::atoi( argv[2] );

		std::cout << "- create graph of " << nb_vertices << " vertices and " << nb_egdes << " edges\n";
		graph_t g = GenerateRandomGraph( nb_vertices, nb_egdes );

		std::cout << "- save graph to " << filename << "\n";

		SaveGraph( g, filename );
	}
	{
		graph_t g = ReadGraph( filename );
		std::cout << "- Read graph\n";
		std::cout << " - NbVertices=" << boost::num_vertices( g ) << "\n";
	}
}
//-------------------------------------------------------------------
