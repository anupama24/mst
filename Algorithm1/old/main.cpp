#include<math.h>
#include<iostream>
#include<ctime>
#include <limits>
#include <stxxl/vector>
#include <stxxl/priority_queue>
#include <stxxl/scan>
#include <stxxl/sort>
#include <stxxl/map>

#include "config.h"
#include "graph.h"
#include "graph.cpp"
#include"external_prims.cpp"
#include "directedGraph.cpp"
#include"vertex_contract.cpp"
#include "sueprphase.cpp"

using namespace std;

int main(int argc, char *argv[])
{
	int num_v,num_e;	

	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	std::cin>>num_v;
	STXXL_MSG("Edges :");
	std::cin>>num_e;
	
	graph g(num_v,num_e);
	g.generate_graph();
	g.print_graph();
	
	
	
	float c,t;
	float B = (float) BLOCK_SIZE/(float)(sizeof(edge));
	t = log10((float) num_v*B / (float) num_e)/log10(2.0);
	c = log10(t) / log10(1.5);
	int count = ceil(c);
	float M = (20 * 1024 * 1024)/(float)(sizeof(edge) + sizeof(vertex));
	float N = num_v+num_e;
	STXXL_MSG("N: "<<N<<" M: "<<M<<" B: "<<B<<"Sizeof Edge: "<<sizeof(edge));

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;

	if(N > (M/2))
	{
		
		Timer.start();
		superphaseAlgo(g,count);
		STXXL_MSG("Elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(g.no_edges) / (Timer.mseconds() / 1000.)) << " edges per sec");
	        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
			
	}

	
	if(g.no_edges != 0)
	{
		stats_begin = *stxxl::stats::get_instance();
    		Timer.reset();
		Timer.start();
		external_prim mst(g.no_vertices,g.no_edges);		
		mst.build_mst(g);
		
		STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(g.no_edges) / (Timer.mseconds() / 1000.)) << " edges per sec");

      		  std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	}
	
	
	return 0;
