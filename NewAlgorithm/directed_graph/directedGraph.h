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
	
	
private:
	
	dirEdgeType roots;
	dirEdgeType dirEdgeList;
	unsigned int noEdges;
	
public:
	DirectedGraph(){}

	DirectedGraph(unsigned int num_e):dirEdgeList(),noEdges(num_e)
	{
		STXXL_MSG("Creating a directed graph, addr=" << this);
		STXXL_MSG("Edges: " << num_e);

		
		//dirEdgeList.clear();
		//dirVertex.clear();
					
	}

	~DirectedGraph()
	{
		dirEdgeList.flush();
		roots.flush();
		dirEdgeList.clear();
		roots.clear();
	}
		

	inline bool checkRootsEnd(dirEdgeItr itr)
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

	void clear()
	{
		dirEdgeList.flush();
		roots.flush();
		dirEdgeList.clear();
		roots.clear();
	}

	unsigned int getNoEdges();
	void setNoEdges(unsigned int numE);
	
	void createGraph(Graph::edgeType &edgeSet);
	void detectCycle();
	void addEdgesMST(StarGraph &star,MST &mst,Graph &inputGraph);
	void printGraph();
	
	dirEdgeItr getFirstRoot();
	dirEdgeItr getFirstEdge();	
	dirEdgeItr getEdgeListEnd();
	dirEdgeItr getRootEnd();
	
	
	
};

		
