

class DirectedEdge 
{
public:	
	int src,dst;
	int edge_wt;
	DirectedEdge() {}
	DirectedEdge(int src_, int dst_,int wt_): src(src_), dst(dst_),edge_wt(wt_) {} 
	bool operator == (const DirectedEdge & b) const {
		return (src == b.src && dst == b.dst && edge_wt==b.edge_wt);
	}
	bool equals (const DirectedEdge & b) const {
		return (src == b.dst && dst == b.src && edge_wt==b.edge_wt);
	}	
};

struct cmp_wt
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<int>::min()); };
	DirectedEdge max_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<int>::max()); };
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.edge_wt < b. edge_wt || (a.edge_wt==b.edge_wt && a.src< b.src) || (a.edge_wt==b.edge_wt && a.src == b.src && a.dst < b.dst);
	}
};

struct cmp_dir_edge
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<int>::min(),0,0); };
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<int>::max(),0,0); };
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.src < b.src || (a.src == b.src && a.edge_wt < b.edge_wt);
	}
};

struct cmp_dir_src
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<int>::min(),0,0); };
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<int>::max(),0,0); };
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.src < b.src || (a.src == b.src && a.dst < b.dst);
	}
};

struct cmp_dir_dst
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<int>::min(),0,0); };
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<int>::max(),0,0); };
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.dst < b.dst || (a.dst == b.dst && a.src < b.src);
	}
};


class DirectedGraph
{
public:
	typedef stxxl::VECTOR_GENERATOR<DirectedEdge, PAGE_SIZE, NO_OF_PAGES,  BLOCK_SIZE,stxxl::striping>::result vector_type;
	typedef typename vector_type::iterator dirEdge_itr;
	typedef std::pair<vertex,dirEdge_itr> vertex_type;
	typedef typename stxxl::vector<vertex_type>::iterator dirVertex_itr;

	
	
	stxxl::vector<vertex_type> dirVertex;
	stxxl::vector<int> roots;
	vector_type dirEdgeList;
	int no_vertices;
	int no_edges;

	
	vector_type cycleEdges;

	DirectedGraph(){}

	DirectedGraph(int num_v,int num_e):dirVertex(num_v),dirEdgeList(2*num_v),no_vertices(num_v),no_edges(num_e)
	{
		STXXL_MSG("Creating a directed graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);

		
		//dirEdgeList.clear();
		//dirVertex.clear();
					
	}
	void createGraph(graph &g);
	void detectCycle();
	void dfs(dirVertex_itr itr);
	void printGraph();
};

		
