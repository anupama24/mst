#include "graph.h"

void graph::generate_graph()
{
	int i;
	edge *Edge;
	vertex *v;
	stxxl::vector<edge>::iterator itr;
	ExtEdgeVec.clear();
	ExtVertexVec.clear();
	
	//std::generate(ExtEdgeVec.begin(),ExtEdgeVec.end(),random_edge(no_vertices,wt_range));
	
	for(i=0;i<no_vertices;i++)
	{
		v = new vertex(i+1);
		ExtVertexVec.push_back(*v);
	}


	for(i=0;i<no_edges;i++)
	{
		do
		{
			Edge = new edge(random()%no_vertices+1,random()%no_vertices+1,(random()%wt_range)+1);
			while(Edge->dst == Edge->src) 
				Edge->dst = random()%no_vertices+1;
			itr = stxxl::find(ExtEdgeVec.begin(), ExtEdgeVec.end(), *Edge, 4);
					
		}
		while(itr!=ExtEdgeVec.end());
		ExtEdgeVec.push_back(*Edge);
				
	}

		


	stxxl::sort(ExtEdgeVec.begin(),ExtEdgeVec.end(),my_cmp(),100*1024*1024);
	stxxl::sort(ExtVertexVec.begin(),ExtVertexVec.end(),my_cmp_ver(),100*1024);
	
}

void graph::print_graph()
{
	stxxl::vector<edge>::iterator itr;
	
	STXXL_MSG("**Graph -  Vertex Listing **"<<std::endl);
	STXXL_MSG("No of vertices: "<<no_vertices<<"\nNumber of edges: "<<no_edges<<std::endl);
	
	for(itr=ExtEdgeVec.begin();itr!=ExtEdgeVec.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
		
	}
}

void graph::isConnected()
{
	bool flag=false;
	stxxl::vector<edge>::iterator itr;
	stxxl::vector<vertex>::iterator v_itr;
	stxxl::vector<vertex> tempVertex(no_vertices);
	vertex *v;
	tempVertex.clear();
	for(itr=ExtEdgeVec.begin();itr!=ExtEdgeVec.end();itr++)
	{
		v= new vertex(itr->src);
		v_itr = stxxl::find(tempVertex.begin(), tempVertex.end(), *v, 4);
		if(v_itr==tempVertex.end())
			tempVertex.push_back(*v);
		v= new vertex(itr->dst);
		v_itr = stxxl::find(tempVertex.begin(), tempVertex.end(), *v, 4);
		if(v_itr==tempVertex.end())
			tempVertex.push_back(*v);

		if(tempVertex.size()==ExtVertexVec.size())
		{
			flag=true;
			return;
		}
	}
	stxxl::sort(tempVertex.begin(),tempVertex.end(),my_cmp_ver(),100*1024);
	STXXL_MSG(" "<<tempVertex.size()<<" "<<ExtVertexVec.size()<<" ");	
	if(!flag)
	{
		for(v_itr=ExtVertexVec.begin();v_itr!=ExtVertexVec.end();v_itr++)
		{
			if((stxxl::find(tempVertex.begin(), tempVertex.end(), *v_itr, 4))==tempVertex.end())
			{
				edge *Edge = new edge(v_itr->vertex_id,random()%no_vertices+1,(random()%wt_range)+1);
				while(Edge->dst == Edge->src) 
					Edge->dst = random()%no_vertices+1;
				ExtEdgeVec.push_back(*Edge);
				no_edges++;
			}
		}
	}
	
}
		
		
		


