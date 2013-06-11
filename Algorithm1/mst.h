#ifndef MST_H
#define MST_H

/****************************************************************
Class MST
Members: mstTree - stxxl::vector containing edges of the mst tree
	 noVertices - no of vertices in MST tree
	 noEdges - no of edges in MST tree
Methods: Methods to print the mst tree and add an edge to it
****************************************************************/

class MST
{
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result mstType;
	typedef Graph::const_edgeItr mstItr;
	mstType mstTree;
	unsigned int noVertices;
	unsigned int noEdges;


public:
	//Constructor
	MST(){}
	MST(int num_v):mstTree(num_v),noVertices(num_v),noEdges(num_v-1)
	{}

	//Clears the mstTree vector
	void clearMST()
	{
		mstTree.clear();
	}
	//Returns iterator to the beginning of mst tree vector
	inline mstItr mstBegin()
	{
		return mstTree.begin();
	}
	//Returns iterator to the end of mst tree vector
	inline mstItr mstEnd()
	{
		return mstTree.end();
	}

	unsigned int getMSTSize();
	unsigned int getNoVertices();
	unsigned int getNoEdges();
	void setNoVertices(unsigned int numV);
	void setNoEdges(unsigned int numE);
	void printMST();
	void addEdge(Edge &temp);

};

//Returns size of the mst tree vector
unsigned int MST::getMSTSize()
{
	return mstTree.size();
}

/** Getters and setters for noVertices and noEdges **/
unsigned int MST::getNoVertices()
{
	return noVertices;
}
void MST::setNoVertices(unsigned int numV)
{
	noVertices = numV;
}

unsigned int MST::getNoEdges()
{
	return noEdges;
}
void MST::setNoEdges(unsigned int numE)
{
	noEdges = numE;
}

/** Adds an edges to mst tree vector at the end */
void MST::addEdge(Edge &temp)
{
	mstTree.push_back(temp);
}

/** Prints the mst tree */
void MST::printMST()
{
	mstItr itr;
	
	STXXL_MSG("** Minimum Spanning Tree **");
	STXXL_MSG("No of vertices: "<<noVertices);
	STXXL_MSG("Number of edges: "<<mstTree.size());
	
	for(itr=mstTree.begin();itr!=mstTree.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
		
	}

}
#endif 
