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

	for(v_itr = g.vmain.begin();v_itr != g.vmain.end();v_itr++)
	{
		if(v_itr->second!=g.edge_list.end() && v_itr->second->edge_wt <= (v_itr->first).blocking_value)
		{
			min_itr=v_itr->second;
			e = new DirectedEdge(min_itr->dst,min_itr->src,min_itr->edge_wt);
			dirEdgeList.push_back(*e);
		}
	}

	no_edges=dirEdgeList.size();	

	stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),cmp_dir_edge(),50*1024*1024);

	
}

void DirectedGraph::detectCycle()
{
		dirVertex_itr v_itr;
		dirEdge_itr e_itr,NewEnd;
		typedef stxxl::VECTOR_GENERATOR<int>::result bool_type;
		bool_type visited;

		bool_type::size_type i;	
		int j;
		visited.clear();
		//STXXL_MSG("Detect"<<visited.size());
		for(i = 0; i < no_edges; ++i)
		{
			j=0;
			visited.push_back(j);				
		}
		
		stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),cmp_wt(),50*1024*1024);
		roots.clear();
		cycleEdges.clear();
		for(e_itr=dirEdgeList.begin();e_itr!=dirEdgeList.end();e_itr++)
		{

			//STXXL_MSG("e_itr->src "<<(e_itr->src));
			if(!visited[(e_itr->src)-1])
			{
				//STXXL_MSG("Detect");
				visited[(e_itr->src)-1] = 1;
				roots.push_back(e_itr->src);
				//STXXL_MSG("Root:" <<e_itr->src);
			}
			//STXXL_MSG("Detect");
			if(!visited[(e_itr->dst)-1])
			{
				visited[(e_itr->dst)-1] = 1;
			}
			else
			{
				cycleEdges.push_back(*e_itr);		
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
		/*int flag;
		for(e_itr=dirEdgeList.begin();e_itr!=dirEdgeList.end();e_itr++)
		{	flag =0;
			for(int i=0;i<dirVertex.size();i++)
			{
				if(dirVertex[i].first.vertex_id == e_itr->dst)
				{	flag=1;
					break;
				}
		
			}
			if(flag==0)
			{
				v = new vertex(e_itr->dst);
				result.first = *v;	
				result.second = dirEdgeList.end();
				dirVertex.push_back(result);
			}
		}	
	*/
		no_edges=dirEdgeList.size();
		no_vertices = dirVertex.size();
		STXXL_MSG("No of vertices: "<<no_vertices);
		STXXL_MSG("Number of edges: "<<no_edges<<std::endl);

		visited.clear();
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
	
	dag.printGraph();

	STXXL_MSG("Directed Graph creation elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_e) / (Timer.mseconds() / 1000.)) << " edges per sec");

    	std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	return 0;
}
	

*/	


		
		

	
		
	
	
