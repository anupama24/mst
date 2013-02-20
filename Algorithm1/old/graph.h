

struct random_edge { 

	int num_v,wt_range;
	random_edge(int nv,int wt):num_v(nv),wt_range(wt){}
	edge operator () () const {
		edge Edge((random()%num_v)+1,(random()%num_v)+1,(random()%wt_range)+1);
		while(Edge.dst == Edge.src) Edge.dst = (random()%num_v)+1;
		return Edge; 
	} 

	
};

class graph
{
	public:
	
	typedef stxxl::VECTOR_GENERATOR<edge, PAGE_SIZE, NO_OF_PAGES,  BLOCK_SIZE,stxxl::striping>::result vector_type;
	typedef typename vector_type::iterator edge_itr;
	typedef std::pair<vertex,edge_itr> vertex_type;
	typedef typename stxxl::vector<vertex_type>::iterator vertex_itr;

	
	stxxl::vector<vertex_type> vmain;
	vector_type edge_list;
	int no_vertices;
	int no_edges;
	graph(){}

	struct my_cmp_ver
	{
		graph &parent;
		my_cmp_ver(graph &g):parent(g){}
		vertex_type min_value()  { 
			return vertex_type(vertex(std::numeric_limits<int>::min()),parent.edge_list.end()); };
		vertex_type max_value() const { 
			return vertex_type(vertex(std::numeric_limits<int>::max()),parent.edge_list.end()); };
	  	bool operator () (const vertex_type & a, const vertex_type & b) const {
			return a.first.vertex_id < b.first.vertex_id || (a.first.vertex_id == b.first.vertex_id && a.first.blocking_value < b.first.blocking_value);
		}
	};

	graph(int num_v,int num_e):vmain(num_v),edge_list(2*num_e),no_vertices(num_v),no_edges(num_e)
	{
		
		STXXL_MSG("Creating a graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);
		
		
		//generate_graph(no_vertices,no_edges,wt_range);
	}

	void generate_graph();
	void print_graph();


	inline int randomNodeID() {
	    return (int)(rand()%no_vertices + 1);
	}

	/** Returns a random edge weight. */
	inline int randomEdgeWeight() {
	    const int maxEdgeWeight = 1e3;
	    return (rand() % maxEdgeWeight +1);
	}
};

		

	

	
