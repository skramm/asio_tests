/**
\file plati_graph.hpp
\brief wrapper around the graph modeling the city. Uses BGL
*/

#ifndef HG_PLATI_GRAPH_HPP
#define HG_PLATI_GRAPH_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>

//---------------------------------------------------------------
/// data structure for the spheres (= vertices of the graph)
struct platiVertex
{
	double lat;
	double lon;
	std::string filename;
};

//-------------------------------------------------------------------
/// serialization function
namespace boost {
namespace serialization {
	template<class Archive>
	void serialize( Archive& ar, platiVertex& pv, const unsigned int /*version*/ )
	{
		ar & pv.lat;
		ar & pv.lon;
		ar & pv.filename;
	}
} // namespace serialization
} // namespace boost


//-------------------------------------------------------------------
typedef boost::adjacency_list<
	boost::vecS
	,boost::vecS
	,boost::undirectedS
	,platiVertex
	> plati_graph_t;

typedef boost::graph_traits<plati_graph_t>::vertex_descriptor vertex_t;
typedef boost::graph_traits<plati_graph_t>::edge_descriptor   edge_t;

//---------------------------------------------------------------
/// Holds the graph of spheres. Encapsulates a BGL graph
struct PlatinumData_graph
{
	private:
		plati_graph_t _graph;

	public:
		void Load( std::string filename )
		{
			std::ifstream fs( filename );
			boost::archive::text_iarchive ia( fs );
			ia >> _graph;
		}

		void SaveGraph( std::string filename ) const
		{
			std::ofstream fs( filename );
			boost::archive::text_oarchive oa( fs );
			oa << _graph;
		}
};

//---------------------------------------------------------------

#endif // HG_PLATI_GRAPH_HPP

