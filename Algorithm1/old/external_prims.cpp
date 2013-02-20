class external_prim
{
	
	int no_vertex;
	int no_edges;
	typedef stxxl::VECTOR_GENERATOR<edge, PAGE_SIZE, NO_OF_PAGES,  BLOCK_SIZE,stxxl::striping>::result vector_type;
	vector_type mst_tree;

	struct mst_cmp 
	{
	     bool operator ()  (const edge & a, const edge & b) const
	     {
		 return a.edge_wt > b.edge_wt;
	     }

	     edge min_value() const
	     {
		 return edge(0,0,std::numeric_limits < int >::max());
	     }
	};

public:
	//graph g;

	typedef typename vector_type::iterator edge_itr;
	typedef std::pair<vertex,edge_itr> vertex_type;
	typedef typename stxxl::vector<vertex_type>::iterator vertex_itr;

	external_prim(){}
	
	external_prim(int num_v,int num_e):no_vertex(num_v),no_edges(num_e),mst_tree(num_v)
	{
		
	}

	void print_mst_tree();
	void build_mst(graph &g);

};



void external_prim::build_mst(graph &g)
{
	const unsigned volume = 100 * 1024 * 1024;
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR < edge, mst_cmp, 10 *1024 * 1024, volume/sizeof(edge)> pq_gen;	
	typedef pq_gen::result pq_type;
	typedef pq_type::block_type block_type;
	const unsigned mem_for_pools = 128 * 1024 * 1024 ;
	
	edge_itr itr;
	vertex_itr v_itr;
	edge e_temp;
	vertex_type result;

        

	stxxl::read_write_pool<block_type> pool((mem_for_pools / 2) / block_type::raw_size, (mem_for_pools / 2) / block_type::raw_size);
        pq_type pq_mst(pool);
	
	 STXXL_MSG("stxxl::pq block size: " << block_type::raw_size << " bytes");	
	 STXXL_MSG("Internal memory consumption of the priority queue: " << pq_mst.mem_cons() << " bytes");
     
	mst_tree.clear();

	
	
	/*unsigned int index= (random() % no_vertex) +1;
	vertex *v = new vertex(start);
	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
	for(v_itr = g.vmain.begin(); v_itr!=g.vmain.end() && v_itr->first != *v; v_itr++);
	std::cout << "Find:"<<stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;


	if(index>g.vmain.size())
	{
		STXXL_MSG("Start vertex not found");
		return;
	}
	*/

	//STXXL_MSG("Start vertex is"<<g.vmain[index-1].first.vertex_id);
	//STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");

	//unsigned int index= 1;
	//itr=g.vmain[index-1].second;
	for(v_itr = g.vmain.begin(); v_itr!=g.vmain.end() && v_itr->second == g.edge_list.end(); v_itr++);

	for(itr=v_itr->second;itr!=g.edge_list.end() && itr->src == v_itr->first.vertex_id;itr++)
	{
		//STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
		pq_mst.push(*itr);
	}
	
	do
	{
		assert(!pq_mst.empty());
		e_temp=pq_mst.top();
		//STXXL_MSG(" (" <<(e_temp.src)<<", " <<(e_temp.dst)<<", "<<(e_temp.edge_wt)<<") ");
		pq_mst.pop();
		if((e_temp.equals(pq_mst.top()))&&e_temp.edge_wt==pq_mst.top().edge_wt)
			pq_mst.pop();
		else
		{
			mst_tree.push_back(e_temp);			
			//index = e_temp.dst;
			for(v_itr = g.vmain.begin(); v_itr!=g.vmain.end() && v_itr->first.vertex_id != e_temp.dst; v_itr++);
			itr=v_itr->second;
			for(;itr!=g.edge_list.end() &&  itr->src == v_itr->first.vertex_id ;itr++)
			{
				if(!(e_temp.equals(*itr)))
				{
					//STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
					pq_mst.push(*itr);
				}
			}
			
		}
		
			
	}
	while(mst_tree.size()< (unsigned int) no_vertex-1);
	no_edges= mst_tree.size();
	
		
	

	
}

void external_prim::print_mst_tree()
{
	edge_itr itr;
	
	STXXL_MSG("**Minimum Spanning Tree **");
	STXXL_MSG("No of vertices: "<<no_vertex);
	STXXL_MSG("Number of edges: "<<no_edges);
	
	for(itr=mst_tree.begin();itr!=mst_tree.end();itr++)
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
	

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;
	Timer.start();
	external_prim mst(num_v,num_e);
	mst.g.generate_graph();
	mst.g.print_graph();
	STXXL_MSG("Graph creation elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_e) / (Timer.mseconds() / 1000.)) << " edges per sec");

    	std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();

	mst.build_mst();
	mst.print_mst_tree();
	
	STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;

//	STXXL_MSG("Done, output size=" << g.ExtEdgeVec.size());
	return 0;
}
*/

