


void reduceGraph(graph &g,graph &gnew,int blockValue)
{
	typedef typename graph::edge_itr edge_itr;
	typedef typename graph::vertex_itr vertex_itr;
	std::pair<vertex,edge_itr> result;	
	
	gnew.vmain.clear();
	gnew.edge_list.clear();

	vertex_itr v_itr;
	edge_itr e_itr,new_itr;

	int i=0;

	//STXXL_MSG("Reduce Graph");
	for(v_itr = g.vmain.begin();v_itr != g.vmain.end();v_itr++)
	{
		for(e_itr= v_itr->second,i=0;e_itr!=g.edge_list.end() && e_itr->src== (v_itr->first).vertex_id && i< blockValue ; i++,e_itr++)
		{
			//STXXL_MSG("V: "<<(v_itr->first).vertex_id<<" E: "<<" (" <<(e_itr->src)<<", " <<(e_itr->dst)<<", "<<(e_itr->edge_wt)<<")");			
			gnew.edge_list.push_back(*e_itr);
			gnew.edge_list.push_back(*e_itr);
			gnew.edge_list.back().swap();
		}
	
		//e_itr++;
		//STXXL_MSG("Reduce Graph");
		if(e_itr!=g.edge_list.end() && e_itr->src== (v_itr->first).vertex_id)
			v_itr->first.blocking_value = e_itr->edge_wt;
		else
			v_itr->first.blocking_value = std::numeric_limits<int>::max();
	}

	stxxl::sort(gnew.edge_list.begin(),gnew.edge_list.end(),cmp_edge_wt(),50*1024*1024);
	edge_itr NewEnd = std::unique(gnew.edge_list.begin(),gnew.edge_list.end());
	gnew.edge_list.resize(NewEnd - gnew.edge_list.begin());
	
	for(v_itr = g.vmain.begin();v_itr != g.vmain.end();v_itr++)
	{
	
		
		for(new_itr=gnew.edge_list.begin();new_itr!=gnew.edge_list.end()&&new_itr->src!=(v_itr->first).vertex_id ;new_itr++);

		result.first = v_itr->first;
		result.second = new_itr;
		gnew.vmain.push_back(result);


	}
	//STXXL_MSG("Reduce Graph");
	gnew.no_edges = gnew.edge_list.size()/2;
	gnew.no_vertices = gnew.vmain.size();
	//gnew.print_graph();

	STXXL_MSG("Graph reduced");

}
		
		
void maintainList(stxxl::vector<std::pair<int,int> > &L,stxxl::vector<std::pair<int,int> > &C)
{
	if(L.empty())
	{
		L = C;
		return;
	}
	if(C.empty())
		return;

	stxxl::sort(C.begin(),C.end(),my_cmp_list(), 50*1024*1024);
	stxxl::sort(L.begin(),L.end(),my_cmp_sec(), 50*1024*1024);

	stxxl::vector<std::pair<int,int> >::iterator it1,it2;

	it1=C.begin();
	for(it2=L.begin();it2!=L.end()&& it1!=C.end();)
	{
		if(it1->first==it2->second)
		{
			while(it1!=C.end() && it1->first==it2->second)
			{	it2->second = it1->second;
				it2++;
			}
			it1++;
		}
		else
			it2++;
	}

	STXXL_MSG("List updated");
}


void cleanEdges(graph &g)
{

	
	typedef typename graph::edge_itr edge_itr;
	typedef typename graph::vertex_itr vertex_itr;

	
	edge_itr e_itr;	
	vertex_itr v_itr;

	

	for(e_itr=g.edge_list.begin();e_itr!=g.edge_list.end();e_itr++)
	{
		if(e_itr->src==e_itr->dst)
		{
			e_itr->src=e_itr->dst =e_itr->edge_wt=std::numeric_limits<int>::max();
		}
	}

	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),my_cmp_src(),50*1024*1024);
	
	edge_itr NewEnd = std::unique(g.edge_list.begin(),g.edge_list.end());
	g.edge_list.resize((NewEnd -1) - g.edge_list.begin());
	
	//g.print_graph();
	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),cmp_edge_wt(),50*1024*1024);
	for(v_itr = g.vmain.begin();v_itr != g.vmain.end();v_itr++)
	{
		for(e_itr=g.edge_list.begin();e_itr!=g.edge_list.end()&&e_itr->src!=(v_itr->first).vertex_id;e_itr++);
			v_itr->second = e_itr;
		
	}

	g.no_vertices = g.vmain.size();
	g.no_edges = g.edge_list.size()/2;

	STXXL_MSG("Edges clean");
	//g.print_graph();
	
	

}
	
void minBlockingValue(graph &g,stxxl::vector<std::pair<int,int> > &list)
{
	stxxl::sort(list.begin(),list.end(),my_cmp_sec(), 50*1024*1024);

	stxxl::vector<std::pair<int,int> >::iterator it1,it2;
	int min,curr_v;
	typedef typename graph::vertex_itr vertex_itr;
	
	vertex_itr v_itr;
	
	edge e;
	STXXL_MSG("Size of vertex main: "<<g.vmain.size());
	/*for(v_itr=g.vmain.begin();v_itr!= g.vmain.end();v_itr++)
	{
		
		STXXL_MSG(" V: "<<(v_itr->first).vertex_id<<" & "<<(v_itr->first).blocking_value);
			
	}*/	

	int i,j;
	for(it1=list.begin();it1!=list.end();)
	{
		//STXXL_MSG("it1->second"<<(it1->second));
		for(i =0 ;i < g.no_vertices;i++)
		{	
			if(g.vmain[i].first.vertex_id == it1->second)
				break;
		}
		min = g.vmain[i].first.blocking_value;
		curr_v = it1->second;
		while(it1!=list.end() && it1->second == curr_v)
		{
			for(j=0 ;j < g.no_vertices;j++)
			{	
				if(g.vmain[j].first.vertex_id == it1->first)
					break;
			}
			if(min > g.vmain[j].first.blocking_value)
			{
				min = g.vmain[j].first.blocking_value;
			}
			it1++;
		}
		g.vmain[i].first.blocking_value = min;
		

	}

	for(it1=list.begin();it1!=list.end();it1++)
	{
		for(i =0 ;i < g.no_vertices;i++)
		{	
			if(g.vmain[i].first.vertex_id == it1->second)
				break;
		}
		for(j=0 ;j < g.no_vertices;j++)
		{	
			if(g.vmain[j].first.vertex_id == it1->first)
				break;
		}		
		g.vmain[j].first.vertex_id = it1->second;
		g.vmain[j].first.blocking_value = g.vmain[i].first.blocking_value;
		g.vmain[j].second = g.vmain[i].second;
	}
	
	stxxl::sort(g.vmain.begin(),g.vmain.end(),graph::my_cmp_ver(g), 50*1024*1024);
	vertex_itr NewEnd = std::unique(g.vmain.begin(),g.vmain.end());
	g.vmain.resize((NewEnd) - g.vmain.begin());
	g.no_vertices = g.vmain.size();

	STXXL_MSG("Size of vertex main: "<<g.vmain.size());
	/*for(v_itr=g.vmain.begin();v_itr!= g.vmain.end();v_itr++)
	{
		STXXL_MSG(" V: "<<(v_itr->first).vertex_id<<" & "<<(v_itr->first).blocking_value);
			
	}*/
	STXXL_MSG("Blocking value set");

}

void replaceEdge(graph &g,stxxl::vector<std::pair<int,int> > &result)
{
	stxxl::vector<std::pair<int,int> >::iterator it;
	
	typedef typename graph::edge_itr edge_itr;
	typedef typename graph::vertex_itr vertex_itr;

	
	edge_itr e_itr;	
	vertex_itr v_itr;
	
	stxxl::sort(result.begin(),result.end(),my_cmp_list(),50*1024*1024);
	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),my_cmp_src(),50*1024*1024);

	/*for(it=result.begin();it!=result.end();it++)
	{
		STXXL_MSG(" "<<it->first<<"-->"<<it->second);
	}
	*/

	it=result.begin();
	
	

	for(e_itr=g.edge_list.begin();e_itr!=g.edge_list.end() && it!=result.end();)
	{
		if(it->first==e_itr->src)
		{
			while(it!=result.end() && it->first==e_itr->src)
			{	e_itr->src = it->second;
				e_itr++;
			}
			it++;
		}
		else
			e_itr++;
			
	}

	

	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),my_cmp_dst(),50*1024*1024);
	//STXXL_MSG("Vertex Contraction");
	//g.print_graph();
	
	it=result.begin();
	for(e_itr=g.edge_list.begin();e_itr!=g.edge_list.end()&& it!=result.end();)
	{
		if(it->first==e_itr->dst)
		{
			while(it!=result.end() && it->first==e_itr->dst)
			{	e_itr->dst = it->second;
				e_itr++;
			}
			it++;
		}
		else
			e_itr++;
	}
	//STXXL_MSG("Vertex Contraction");
	//g.print_graph();
	minBlockingValue(g,result);
	cleanEdges(g);	
	STXXL_MSG("Edge replaced");
	
}
	


void superphaseAlgo(graph &g,int count)
{
	typedef std::pair<int,int> repres_vertex;
	stxxl::vector<repres_vertex> list;	
	graph gnew;

	int logNi,blockValue;
	
	float Ni = 2,prevNi,rtNi,lgNi;
	float B = (float) BLOCK_SIZE/(float)(sizeof(edge) + sizeof(vertex));

	DirectedGraph dag(g.no_vertices,g.no_edges);
	dag.createGraph(g);
	dag.detectCycle();
			
	VertexContract vc(dag.no_edges);
	vc.contractVertices(dag);
	replaceEdge(g,vc.result);
	//g.print_graph();
	//list = vc.result;

	STXXL_MSG("Vertices "<<g.no_vertices<<" Edge: "<<g.no_edges<<" count: "<<count);

	for(int i=0;i<count;i++)
	{
		list.clear();
		rtNi = sqrt(Ni);
		blockValue = ceil(rtNi);
		reduceGraph(g,gnew,blockValue);
		lgNi = log10(rtNi) / log10(2.0);
		logNi = ceil(lgNi);
	
		STXXL_MSG("GNEW: Vertices "<<gnew.no_vertices<<" Edge: "<<gnew.no_edges<<" logNi:"<<logNi<<"Block Value: "<<blockValue);

		for(int j= 0; j< logNi;j++)
		{
			dag.createGraph(gnew);
			dag.detectCycle();
			
			//VertexContract vc(dag.no_edges);
			vc.contractVertices(dag);

			replaceEdge(gnew,vc.result);
			maintainList(list,vc.result);
			vc.result.clear();

			STXXL_MSG("GNEW: Vertices "<<gnew.no_vertices<<" Edge: "<<gnew.no_edges);
			if(gnew.no_edges == 0)
				break;
		}
		
		replaceEdge(g,list);
		prevNi= Ni;
		Ni= prevNi * sqrt(prevNi);
		if(g.no_vertices < (g.no_edges/B) || g.no_edges == 0)
			return;
		
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
}
*/
