#ifndef DIRGRAPH_H
#define DIRGRAPH_H
#endif 

#include "../data_structures/directedEdge.h"

class DirectedGraph
{
public:
	typedef stxxl::VECTOR_GENERATOR<DirectedEdge, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result dirEdgeType;
	typedef typename dirEdgeType::iterator dirEdgeItr;
	typedef typename dirEdgeType::const_iterator const_dirEdgeItr;
	//typedef std::pair<Vertex,dirEdgeItr> vertexType;
	//typedef stxxl::VECTOR_GENERATOR<vertexType,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result dirVertexVector;
	//typedef typename dirVertexVector::iterator dirVertexItr;

	
private:
	//dirVertexVector dirVertexList;
	dirEdgeType roots;
	dirEdgeType dirEdgeList;
	unsigned int noEdges;
	
public:
	DirectedGraph(){}

	DirectedGraph(unsigned int num_v,unsigned int num_e):dirEdgeList(num_v+1),noEdges(num_e)
	{
		STXXL_MSG("Creating a directed graph, addr=" << this);
		STXXL_MSG("Edges: " << num_e);

		
		//dirEdgeList.clear();
		//dirVertex.clear();
					
	}

	inline bool checkRootsEnd(const_dirEdgeItr itr)
	{
		if(itr==roots.end())
			return true;
		else
			return false;
	}

	inline bool checkEdgeListEnd(dirEdgeItr itr)
	{
		if(itr==dirEdgeList.end())
			return true;
		else
			return false;
	}

	inline bool checkEdgeListEnd(const_dirEdgeItr itr)
	{
		if(itr==dirEdgeList.end())
			return true;
		else
			return false;
	}

	
	unsigned int getNoEdges();
	void setNoEdges(unsigned int numE);
	void createGraph(Graph &g,MST &mst);
	void detectCycle(MST &mst);
	void printGraph();
	void copyEdgeList(dirEdgeType &list);
	dirEdgeItr getFirstRoot();
	dirEdgeItr getFirstEdge();	
	dirEdgeItr getEdgeListEnd();
	dirEdgeItr getRootEnd();
	bool isRoot(unsigned int src,unsigned int dst,unsigned int wt);
	void clearList()
	{
		dirEdgeList.clear();
		roots.clear();
	}
	
	
};

		
