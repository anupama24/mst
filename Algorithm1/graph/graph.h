#ifndef GRAPH_H
#define GRAPH_H


#include "../data_structures/vertex.h"
#include "../data_structures/edge.h"
/*******************************************************************************************************************************
Class Graph
Members: vertexList - List of vertex and edge list iterator pairs having all the vertices of the graph
	 edgeList - stxxl::vector of edges. Stores twice the no. of edges has copy of each edge (u,v) and (v,u) in the graph
	 noVertcies - total no. of vertices in the graph
	 noEdges - total no. of edges in the graph
Methods: Methods to create random and complete graphs. Method to generate the edgeList and vertexList
*********************************************************************************************************************************/
class Graph
{
public:
	
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result edgeType;
	//Edge Vector Iterators	
	typedef typename edgeType::iterator edgeItr;
	typedef typename edgeType::const_iterator const_edgeItr;
	//Vertex type: Pair of Vertex and iterator to edge vector	
	typedef std::pair<Vertex,edgeItr> vertexType;
	typedef stxxl::VECTOR_GENERATOR<vertexType,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result vertexVector;
	//Vertex Vector Iterators
	typedef typename vertexVector::iterator vertexItr;
	typedef typename vertexVector::const_iterator const_vertexItr;

private:	
	vertexVector vertexList;
	edgeType edgeList;
	unsigned int noVertices;
	unsigned int noEdges;

public:
	//Constructors
	Graph(){}
	Graph(unsigned int num_v,unsigned int num_e):vertexList(num_v),edgeList(2*num_e),noVertices(num_v),noEdges(num_e)
	{
		//Seed initialization for rand function
		srand(time(NULL));		
		STXXL_MSG("Creating a graph, addr=" << this);
		STXXL_MSG("Vertices: " << num_v);
		STXXL_MSG("Edges: " << num_e);
		
	}

	//Comparator for vertex vector pair
	struct myCmpVer
	{
		//Parent graph for whose vertex list this comparator is called
		//Used to initialize the edge iterator field to the end of edgeList
		Graph &parent;
		myCmpVer(Graph &g):parent(g){}

		vertexType min_value()  { 
			return vertexType(Vertex(std::numeric_limits<unsigned int>::min()),parent.edgeList.end()); };
		vertexType max_value() const { 
			return vertexType(Vertex(std::numeric_limits<unsigned int>::max()),parent.edgeList.end()); };
	  	
		//Sorts according to vertex id first and then based on blocking value
		bool operator () (const vertexType & a, const vertexType & b) const {
			return a.first.getVertexId() < b.first.getVertexId() || (a.first.getVertexId() == b.first.getVertexId() && a.first.getBlockingValue() < b.first.getBlockingValue());
		}
	};

	/** Returns a random vertex id. */
	inline unsigned int randomNodeID() {
	    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * noVertices + 1);
	}

	/** Returns a random edge weight. */
	inline unsigned int randomEdgeWeight() {
	    const int maxEdgeWeight = 1e3;
	    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * maxEdgeWeight + 1);
	}

	/** Checks if the edgeList const iterator is the end of edgeList or not **/
	inline bool checkEdgeListEnd(const_edgeItr itr)
	{
		if(itr==edgeList.end())
			return true;
		else
			return false;
	}
	
	/** Checks if the edgeList iterator is the end of edgeList or not **/
	inline bool checkEdgeListEnd(edgeItr itr)
	{
		if(itr==edgeList.end())
			return true;
		else
			return false;
	}

	/** Checks if the vertexList const iterator is pointing to end of vertexList or not **/
	inline bool checkVertexListEnd(const_vertexItr itr)
	{
		if(itr==vertexList.end())
			return true;
		else
			return false;
	}

	/** Checks if the vertexList iterator is pointing to end of vertexList or not **/
	inline bool checkVertexListEnd(vertexItr itr)
	{
		if(itr==vertexList.end())
			return true;
		else
			return false;
	}

	/** Clears both edge and vertex list */
	inline void clearList()
	{
		edgeList.clear();
		vertexList.clear();
	}

	/** Returns size of edgeList */
	inline unsigned int getEdgeListSize()
	{
		return edgeList.size();
	}

	/** Returns size of vertexList */
	inline unsigned int getVertexListSize()
	{
		return vertexList.size();
	}
	

	unsigned int getNoVertices();
	unsigned int getNoEdges();
	void setNoVertices(unsigned int numV);
	void setNoEdges(unsigned int numE);
	void generateRandomGraph();
	void generateCompleteGraph();

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
#endif 
