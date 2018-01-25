/**
bgl_serial_2.cpp

test of serializing a graph that has specific type of vertex

*/

#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
//#include <boost/serialization/vector.hpp>
//#include <boost/serialization/list.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include "boost/graph/graphviz.hpp"

//-------------------------------------------------------------------------------------------
/// vertex
struct myVertex
{
//	friend class boost::serialization::access;
	int a;
	float b;
	std::string name;
/*
template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & a;
        ar & b;
    }*/
};

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	myVertex
	> graph_t;

typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
//typedef boost::graph_traits<graph_t>::edge_descriptor   edge_t;


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

namespace boost {
namespace serialization {

template<class Archive>
void serialize( Archive& ar, myVertex& mv, const unsigned int version )
{
    ar & mv.a;
    ar & mv.b;
}

} // namespace serialization
} // namespace boost


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


template <class T1, class T2, class graph_t>
class node_writer
{
	public:
		node_writer(T1 v1, T2 v2, const graph_t& g ) : _v1(v1),_v2(v2), _g(g)
		{}
		template <class Vertex>
		void operator()( std::ostream &out, const Vertex& v ) const
		{
			out << _v1[v] << "-" << _v2[v];
		}
	private:
		T1 _v1;
		T2 _v2;
		graph_t _g;
};

template <class T1, class T2, class graph_t>
inline
node_writer<T1,T2,graph_t>
make_node_writer( T1 v1, T2 v2, const graph_t& g )
{
	return node_writer<T1,T2,graph_t>(v1,v2,g);
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

		g[0].name = "I Am The First";
		g[0].a = 42;
		g[0].b = 3.1415;

		std::cout << "- save graph to " << filename << "\n";
		SaveGraph( g, filename );
	}
	{
		graph_t g = ReadGraph( filename );
		std::cout << "- Read graph\n";
		std::cout << " - NbVertices=" << boost::num_vertices( g ) << "\n";

		boost::dynamic_properties dp;
		dp.property( "color", boost::get( &myVertex::name, g ) );

		boost::write_graphviz(
			std::cout,
			g,
			make_node_writer(
				boost::get( &myVertex::name, g ),
				boost::get( &myVertex::a, g ),
				g
			)
		);

	}
}
//-------------------------------------------------------------------
