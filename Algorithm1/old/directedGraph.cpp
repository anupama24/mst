#include "directedGraph.h"




void DirectedGraph::createGraph(graph &g)
{

	
	typedef typename graph::edge_itr edge_itr;
	typedef typename graph::vertex_itr vertex_itr;
	DirectedEdge *e;
	
	dirEdge_itr itr;
	vertex_itr v_itr;
	edge_itr min_itr;

	//g.print_graph();
	
	dirEdgeList.clear();

	STXXL_MSG("Check");
	for(v_itr = g.vmain.begin();v_itr != g.vmain.end();v_itr++)
	{
		//STXXL_MSG("Check Inside for");
		if(v_itr->second!=g.edge_list.end() && v_itr->second->edge_wt <= (v_itr->first).blocking_value)
		{
			//STXXL_MSG("Check inside if");
			min_itr=v_itr->second;
			e = new DirectedEdge(min_itr->dst,min_itr->src,min_itr->edge_wt);
			dirEdgeList.push_back(*e);
		}
	}

	no_edges=dirEdgeList.size();	

	stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),cmp_dir_edge(),50*1024*1024);
	STXXL_MSG("Directed graph created");
	//printGraph();
	
}

void DirectedGraph::detectCycle()
{
		dirVertex_itr v_itr;
		dirEdge_itr e_itr,NewEnd;
		typedef stxxl::VECTOR_GENERATOR<int>::result bool_type;
		
		stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),cmp_wt(),50*1024*1024);
		roots.clear();
		cycleEdges.clear();
		for(e_itr=dirEdgeList.begin();e_itr!=dirEdgeList.end();e_itr++)
		{

			if((e_itr+1)!=dirEdgeList.end() && e_itr->equals(*(e_itr+1)))
			{
				cycleEdges.push_back(*(e_itr+1));	
				roots.push_back(e_itr->src);
				e_itr++;					
			}
		}
		
		//STXXL_MSG("Detect");	
		stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),cmp_dir_edge(),50*1024*1024);	
		
		if(!cycleEdges.empty())
		{
			for(e_itr=cycleEdges.begin();e_itr!=cycleEdges.end();e_itr++)
			{
				NewEnd = std::remove(dirEdgeList.begin(),dirEdgeList.end(),*e_itr);
				dirEdgeList.resize(NewEnd - dirEdgeList.begin());
			}
		}
		//STXXL_MSG("Detect");
		vertex_type result;
		vertex *v;
		dirVertex.clear();
		for(e_itr=dirEdgeList.begin();e_itr!=dirEdgeList.end();)
		{
			if(e_itr!=dirEdgeList.end())
			{
				v = new vertex(e_itr->src);
				result.first = *v;
				result.second= e_itr;
				dirVertex.push_back(result);
			}
			while(e_itr!=dirEdgeList.end()&&e_itr->src==v->vertex_id)
			{
				e_itr++;
			}
			
			//STXXL_MSG("Vertex:"<<(result.first).vertex_id<<" "<<(result.second)->src);
			
		}
			
		
		no_edges=dirEdgeList.size();
		no_vertices = dirVertex.size();
		STXXL_MSG("No of vertices: "<<no_vertices);
		STXXL_MSG("Number of edges: "<<no_edges<<std::endl);
		STXXL_MSG("Cycle detected");
		//printGraph();
		
}


void DirectedGraph::printGraph()
{
	dirEdge_itr itr;
	
	STXXL_MSG("**Directed Graph -  Edge Listing **"<<std::endl);
	STXXL_MSG("No of vertices: "<<no_vertices);
	STXXL_MSG("Number of edges: "<<no_edges<<std::endl);
	
	for(itr=dirEdgeList.begin();itr!=dirEdgeList.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
		
	}

	
}
/*

int main()
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
	
	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;
	Timer.start();
	DirectedGraph dag(num_v,num_e);
	dag.createGraph(g);
	
	dag.detectCycle();
	
	

	STXXL_MSG("Directed Graph creation elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_e) / (Timer.mseconds() / 1000.)) << " edges per sec");

    	std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	dag.printGraph();
	return 0;
}
	

*/	


		
		

	
		
	
	
