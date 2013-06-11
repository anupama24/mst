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
	typedef typename edgeType::const_iterator const_edgeItr;
	//Vertex type: Pair of Vertex and iterator to edge vector	
	typedef std::pair<Vertex,edgeItr> vertexType;
	//Vector of vertices
	typedef stxxl::VECTOR_GENERATOR<vertexType,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result vertexVector;
	//Vertex Vector Iterator
	typedef typename vertexVector::iterator vertexItr;
	typedef typename vertexVector::const_iterator const_vertexItr;

private:	
	vertexVector vertexList;
	edgeType edgeList;
	unsigned int noVertices;
	unsigned int noEdges;

public:
	Graph(){}
	Graph(unsigned int num_v,unsigned int num_e):vertexList(num_v),edgeList(2*num_e),noVertices(num_v),noEdges(num_e)
	{
		srand(time(NULL));
		STXXL_MSG("Creating a graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);
		
			
	}

	/** Returns a random vertex id. */
	inline unsigned int randomNodeID() {
	    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * noVertices + 1);
	}

	/** Returns a random edge weight. */
	inline unsigned int randomEdgeWeight() {
	    const int maxEdgeWeight = 1e3;
	    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * maxEdgeWeight);
	}

	inline bool checkEdgeListEnd(const_edgeItr itr)
	{
		if(itr==edgeList.end())
			return true;
		else
			return false;
	}

	inline bool checkEdgeListEnd(edgeItr itr)
	{
		if(itr==edgeList.end())
			return true;
		else
			return false;
	}

	inline bool checkVertexListEnd(const_vertexItr itr)
	{
		if(itr==vertexList.end())
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
		noVertices = 0;
		noEdges = 0;
	}
	
	inline void flush()
	{
		edgeList.flush();
		vertexList.flush();
		
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
	void addEdge(const Edge &temp);
	void addVertex(vertexType &v);
	void swapEdge();

	vertexItr getFirstVertex();
	edgeItr getFirstEdge();
	edgeItr getEdgeListEnd();
	vertexItr getVertexListEnd();
	void resizeList(edgeItr end,edgeItr begin);
	void resizeVertexList(vertexItr end,vertexItr begin);
	void clearEdgeList()
	{
		edgeList.clear();
	}
	


};

		

	

	