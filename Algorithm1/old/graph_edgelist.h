#include <limits>
#include <stxxl/io>
#include <stxxl/mng>
#include <stxxl/vector>
#include <stxxl/sort>
#include <stxxl/scan>

struct edge 
{
	int src,dst;
	int edge_wt;
	edge() {}
	edge(int src_, int dst_,int wt_): src(src_), dst(dst_),edge_wt(wt_) {} 
	bool operator == (const edge & b) const {
		return ((src == b.src && dst == b.dst) ||(src == b.dst && dst == b.dst));// || edge_wt == b.edge_wt);
	}
};

struct vertex
{
	int vertex_id;
	vertex() {}
	vertex(int id): vertex_id(id){}
	bool operator == (const vertex & b) const {
		return vertex_id == b.vertex_id;
	}
};
	
struct random_edge { 

	int num_v,wt_range;
	random_edge(int nv,int wt):num_v(nv),wt_range(wt){}
	edge operator () () const {
		edge Edge(random()%num_v,random()%num_v,(random()%wt_range)+1);
		while(Edge.dst == Edge.src) Edge.dst = random()%num_v;
		return Edge; 
	} 
};
struct my_cmp
{
  	edge min_value() const { 
		return edge(std::numeric_limits<int>::min(),0,0); };
	edge max_value() const { 
		return edge(std::numeric_limits<int>::max(),0,0); };
	bool operator () (const edge & a, const edge & b) const {
		return a.src < b.src || (a.src == b.src && a.dst < b.dst);
	}
};

struct my_cmp_ver
{
  	vertex min_value() const { 
		return vertex(std::numeric_limits<int>::min()); };
	vertex max_value() const { 
		return vertex(std::numeric_limits<int>::max()); };
	bool operator () (const vertex & a, const vertex & b) const {
		return a.vertex_id < b.vertex_id;
	}
};

class graph
{
public:	stxxl::vector<edge> ExtEdgeVec;
	stxxl::vector<vertex> ExtVertexVec;
	int no_vertices;
	int no_edges;
	int wt_range;
	graph(){}
	graph(int num_v,int num_e,int wt):ExtEdgeVec(num_e),ExtVertexVec(num_v)
	{
		STXXL_MSG("Creating a graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);
		no_vertices=num_v;
		no_edges = num_e;
		wt_range = wt;
		
		
	}	
	void generate_graph();
	void print_graph();
	void isConnected();
};


