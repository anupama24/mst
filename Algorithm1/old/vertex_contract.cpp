struct pq_vertex
{
	vertex v;
	int id;
	int edge_wt;
	pq_vertex(){}
	pq_vertex(vertex v_,int id_,int wt_):v(v_),id(id_),edge_wt(wt_){}
};

struct pq_cmp 
{
     bool operator ()  (const pq_vertex & a, const pq_vertex & b) const
     {
	 return a.edge_wt > b.edge_wt;
     }

     pq_vertex min_value() const
     {
	 return pq_vertex(NULL,0,std::numeric_limits < int >::max());
     }
};
class VertexContract
{

	typedef typename DirectedGraph::vector_type vector_type;
	typedef typename DirectedGraph::dirEdge_itr iter;
	typedef typename DirectedGraph::dirVertex_itr vertex_itr;

	
	vector_type list;
public:
	typedef std::pair<int,int> repres_vertex;
	stxxl::vector<repres_vertex> result;	
	VertexContract(){}
	VertexContract(int num_e):list(2*num_e){}

	void contractVertices(DirectedGraph &dag);
	void createList(DirectedGraph &dag);
	//void replaceEdge(graph &g);

};

struct my_cmp_list
{
  	VertexContract::repres_vertex min_value() const { 
		return VertexContract::repres_vertex(std::numeric_limits<int>::min(),0); };
	VertexContract::repres_vertex max_value() const { 
		return VertexContract::repres_vertex(std::numeric_limits<int>::max(),0); };
	bool operator () (const VertexContract::repres_vertex & a, const VertexContract::repres_vertex & b) const {
		return a.first < b.first || (a.first == b.first && a.second < b.second);
	}
};

struct my_cmp_sec
{
  	VertexContract::repres_vertex min_value() const { 
		return VertexContract::repres_vertex(std::numeric_limits<int>::min(),0); };
	VertexContract::repres_vertex max_value() const { 
		return VertexContract::repres_vertex(std::numeric_limits<int>::max(),0); };
	bool operator () (const VertexContract::repres_vertex & a, const VertexContract::repres_vertex & b) const {
		return a.second < b.second || (a.second == b.second && a.first < b.first);
	}
};


void VertexContract::createList(DirectedGraph &dag)
{
	iter e_itr,temp,end_itr;
	vertex_itr v_itr;

	//list.clear();
	list.resize(2*dag.no_edges);
	DirectedEdge *e;

	STXXL_MSG("List "<<list.size());
	for(int i = 0; i<list.size();i++)
	{
		e = new DirectedEdge(0,0,0);
		list[i] = *e;
	}

	//dag.printGraph();
	for(e_itr=dag.dirEdgeList.begin();e_itr!=dag.dirEdgeList.end();e_itr++)
		list.push_back(*e_itr);
	
	stxxl::sort(list.end()-dag.no_edges-1,list.end(),cmp_wt(),50*1024*1024);

	//STXXL_MSG("Odd");
	/*for(e_itr=list.begin();e_itr!=list.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->src)<<", " <<(e_itr->dst)<<", "<<(e_itr->edge_wt)<<") ");
		
	}*/

	e_itr=list.end()-dag.no_edges;
	end_itr= list.begin();

	for(;e_itr!=list.end();e_itr++)
	{
		*end_itr= *e_itr;
		end_itr++;
		//STXXL_MSG(" (" <<(e_itr->src)<<", " <<(e_itr->dst)<<", "<<(e_itr->edge_wt)<<") ");
		for(v_itr = dag.dirVertex.begin(); v_itr!=dag.dirVertex.end() && (v_itr->first).vertex_id != e_itr->dst; v_itr++);
		if(v_itr!=dag.dirVertex.end())
			for(temp=v_itr->second;temp!=dag.dirEdgeList.end() && temp->src == e_itr->dst; temp++)
			{
				*end_itr = *temp;
				//STXXL_MSG(" (" <<(temp->src)<<", " <<(temp->dst)<<", "<<(temp->edge_wt)<<") ");
				end_itr++;
			}
		
	}
	
	list.resize(end_itr-list.begin());
	STXXL_MSG("List created");
	for(e_itr=list.begin();e_itr!=list.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->src)<<", " <<(e_itr->dst)<<", "<<(e_itr->edge_wt)<<") ");
		
	}
	
}
void VertexContract::contractVertices(DirectedGraph &dag)
{

	const unsigned volume = 100 * 1024 * 1024;
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR < pq_vertex, pq_cmp, 10 * 1024 * 1024, volume/sizeof(pq_vertex)> pq_gen;	
	typedef pq_gen::result pq_type;
	typedef pq_type::block_type block_type;
	const unsigned mem_for_pools = 128 * 1024 * 1024;

	stxxl::read_write_pool<block_type> pool((mem_for_pools / 2) / block_type::raw_size, (mem_for_pools / 2) / block_type::raw_size);
        pq_type pq_fwd(pool);
	pq_vertex *pq_elem;

	createList(dag);
	stxxl::vector<int>::iterator root_itr;
	vertex_itr v_itr;
	iter e_itr,temp;
	repres_vertex new_repres;
	vertex *v; 

	
	for(root_itr=dag.roots.begin();root_itr!=dag.roots.end();root_itr++)
	{
		for(v_itr = dag.dirVertex.begin(); v_itr!=dag.dirVertex.end() && (v_itr->first).vertex_id != *root_itr; v_itr++);
			for(temp=v_itr->second;temp!=dag.dirEdgeList.end() && temp->src == *root_itr; temp++)
			{	
				v = new vertex(temp->dst);
				STXXL_MSG("PQ push:"<<v->vertex_id<<" "<<*root_itr<<" "<<temp->edge_wt);
				pq_elem = new pq_vertex(*v,*root_itr,temp->edge_wt);
				pq_fwd.push(*pq_elem);
			}
		
	}
	
	for(e_itr=list.begin();e_itr!=list.end()&&!pq_fwd.empty();e_itr++)
	{
		assert(!pq_fwd.empty());
		*pq_elem=pq_fwd.top();
		pq_fwd.pop();
	
		new_repres.first=(pq_elem->v).vertex_id;
		new_repres.second=pq_elem->id;
		result.push_back(new_repres);

		for(temp=e_itr+1;temp!=list.end() && temp->src == e_itr->dst && !(*temp==*(temp-1));temp++)
		{
			v = new vertex(temp->dst);
			//STXXL_MSG("PQ push:"<<v->vertex_id<<" "<<pq_elem->id<<" "<<temp->edge_wt);
			pq_elem = new pq_vertex(*v,pq_elem->id,temp->edge_wt);
			pq_fwd.push(*pq_elem);
		}
		e_itr=temp-1;
		//STXXL_MSG(" (" <<(e_itr->src)<<", " <<(e_itr->dst)<<", "<<(e_itr->edge_wt)<<") ");
	
	}

	while(!pq_fwd.empty())
	{
		*pq_elem=pq_fwd.top();
		pq_fwd.pop();
	
		new_repres.first=(pq_elem->v).vertex_id;
		new_repres.second=pq_elem->id;
		result.push_back(new_repres);
	}
			

	dag.roots.clear();
	list.clear();
	//pq_fwd.~priority_queue();
	STXXL_MSG("Vertices contracted");
	
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

	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();


	VertexContract vc(dag.no_edges);
	vc.contractVertices(dag);
	
	replaceEdge(g,vc.result);
	g.print_graph();

	STXXL_MSG("Vertex Contraction elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	return 0;
}

*/
