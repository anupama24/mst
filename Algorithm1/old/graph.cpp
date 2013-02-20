


void graph::generate_graph()
{
	int i;
	vertex *v;
	edge *Edge;
	vertex_type result;
	edge_itr itr;
	vmain.clear();
	
	edge_list.clear();
	//std::generate(edge_list.begin(),edge_list.end(),random_edge(no_vertices,1e3));
	
	//srand ( time(NULL) );

	for(i=0;i<no_vertices;i++)
	{	
		do
		{	Edge = new edge(i+1,randomNodeID(),randomEdgeWeight());
				while(Edge->dst == Edge->src) 
					Edge->dst = randomNodeID();
			itr = stxxl::find(edge_list.begin(), edge_list.end(), *Edge, 4);
					
		}
		while(itr!=edge_list.end());
		edge_list.push_back(*Edge);
		edge_list.push_back(*Edge);
		edge_list.back().swap();
		
	}
		
		

	for(i=0;i<(no_edges-no_vertices);i++)
	{
		do
		{	Edge = new edge(randomNodeID(),randomNodeID(),randomEdgeWeight());
			while(Edge->dst == Edge->src) 
				Edge->dst = randomNodeID();
			itr = stxxl::find(edge_list.begin(), edge_list.end(), *Edge, 4);
					
		}
		while(itr!=edge_list.end());
		edge_list.push_back(*Edge);
		edge_list.push_back(*Edge);
		edge_list.back().swap();
				
	}

	
	stxxl::sort(edge_list.begin(),edge_list.end(),cmp_edge_wt(),150 * 1024*1024);
	edge_itr NewEnd = std::unique(edge_list.begin(),edge_list.end());
	edge_list.resize(NewEnd - edge_list.begin());
	
	STXXL_MSG(" Edge list: "<<edge_list.size());

	for(i=0;i<no_vertices;i++)
	{
		v = new vertex(i+1);
		result.first = *v;
		for(itr=edge_list.begin();itr!=edge_list.end()&&itr->src!=v->vertex_id;itr++);
		/*if(itr==edge_list.end())
		{
			edge *Edge = new edge(v->vertex_id,randomNodeID(),randomEdgeWeight());
			while(Edge->dst <= Edge->src) 
				Edge->dst = randomNodeID();
			edge_list.push_back(*Edge);
			edge_list.push_back(*Edge);
			edge_list.back().swap();
			stxxl::sort(edge_list.begin(),edge_list.end(),cmp_edge_wt(),150 * 1024*1024);
			itr = stxxl::find(edge_list.begin(), edge_list.end(), *Edge, 4);

		}*/
		
		result.second= itr;
		vmain.push_back(result);

			
	}
	
	no_edges = edge_list.size()/2;
}


void graph::print_graph()
{
	edge_itr itr;
	
	STXXL_MSG("**Graph -  Vertex Listing **"<<std::endl);
	STXXL_MSG("No of vertices: "<<no_vertices<<"\nNumber of edges: "<<no_edges<<std::endl);
	
	for(itr=edge_list.begin();itr!=edge_list.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
		
	}

	/*vertex_itr v_itr;
	vertex_type result;
	edge e;
	STXXL_MSG("Size of vertex main: "<<vmain.size());
	for(v_itr=vmain.begin();v_itr!= vmain.end();v_itr++)
	{
		result = *v_itr;
		e = *(result.second);
		STXXL_MSG(" V: "<<(v_itr->first).vertex_id<<" ("<<(e.src)<<", " <<(e.dst)<<", "<<(e.edge_wt)<<") ");
			
	}*/
}


/*
int main()
{
	int num_v,num_e,ed;	

	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	cin>>num_v;
	STXXL_MSG("Edges :");
	cin>>num_e;
	STXXL_MSG("Weight Range: ");
	cin>>ed;

	graph g(num_v,num_e,ed);

	
	g.generate_graph();
	g.print_graph();
	return 1;
}

*/
