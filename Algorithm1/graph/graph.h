#ifndef GRAPH_H
#define GRAPH_H
#endif 

#include "../data_structures/vertex.h"
#include "../data_structures/edge.h"

class Graph
{
public:
	
	//Vector of Edges
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result edgeType;
	//Edge Vector Iterator	
	typedef typename edgeType::iterator edgeItr;
	//Vertex type: Pair of Vertex and iterator to edge vector	
	typedef std::pair<Vertex,edgeItr> vertexType;
	//Vector of vertices
	typedef stxxl::VECTOR_GENERATOR<vertexType,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result vertexVector;
	//Vertex Vector Iterator
	typedef typename vertexVector::iterator vertexItr;

private:	
	vertexVector vertexList;
	edgeType edgeList;
	unsigned int noVertices;
	unsigned int noEdges;

public:
	Graph(){}
	Graph(unsigned int num_v,unsigned int num_e):vertexList(num_v),edgeList(2*num_e),noVertices(num_v),noEdges(num_e)
	{
		
		STXXL_MSG("Creating a graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);
		
		
		//generate_graph(noVertices,noEdges,wt_range);
	}

	//Comparator for vertex vector pair
	struct myCmpVer
	{
		Graph &parent;
		myCmpVer(Graph &g):parent(g){}
		vertexType min_value()  { 
			return vertexType(Vertex(std::numeric_limits<unsigned int>::min()),parent.edgeList.end()); };
		vertexType max_value() const { 
			return vertexType(Vertex(std::numeric_limits<unsigned int>::max()),parent.edgeList.end()); };
	  	bool operator () (const vertexType & a, const vertexType & b) const {
			return a.first.getVertexId() < b.first.getVertexId() || (a.first.getVertexId() == b.first.getVertexId() && a.first.getBlockingValue() < b.first.getBlockingValue());
		}
	};

	/** Returns a random vertex id. */
	inline int randomNodeID() {
	    return (int)(rand()%noVertices + 1);
	}

	/** Returns a random edge weight. */
	inline int randomEdgeWeight() {
	    const int maxEdgeWeight = 1e3;
	    return (rand() % maxEdgeWeight +1);
	}

	inline bool checkEdgeListEnd(edgeItr itr)
	{
		if(itr==edgeList.end())
			return true;
		else
			return false;
	}

	inline bool checkVertexListEnd(vertexItr itr)
	{
		if(itr==vertexList.end())
			return true;
		else
			return false;
	}
	inline void clearList()
	{
		edgeList.clear();
		vertexList.clear();
	}

	inline unsigned int getEdgeListSize()
	{
		return edgeList.size();
	}

	inline unsigned int getVertexListSize()
	{
		return vertexList.size();
	}
	

	unsigned int getNoVertices();
	unsigned int getNoEdges();
	void setNoVertices(unsigned int numV);
	void setNoEdges(unsigned int numE);
	void generateGraph();
	void generateEdges();
	void generateVertexList();
	void printGraph();
	void addEdge(Edge &temp);
	void addVertex(vertexType &v);
	void swapEdge();

	vertexItr getFirstVertex();
	edgeItr getFirstEdge();
	edgeItr getEdgeListEnd();
	vertexItr getVertexListEnd();
	void resizeList(edgeItr end,edgeItr begin);
	void resizeVertexList(vertexItr end,vertexItr begin);
	


};

		

	

	
