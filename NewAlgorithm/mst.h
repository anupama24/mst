#ifndef MST_H
#define MST_H
#endif 


class MST
{
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result mstType;
	typedef Graph::const_edgeItr mstItr;
	mstType mstTree;
	unsigned int noVertices;
	unsigned int noEdges;


public:

	MST(){}
	MST(int num_v):mstTree(num_v),noVertices(num_v),noEdges(num_v-1)
	{}

	void clearMST()
	{
		mstTree.clear();
	}
	void flush()
	{
		mstTree.flush();
	}


	unsigned int getMSTSize();
	unsigned int getNoVertices();
	unsigned int getNoEdges();
	void setNoVertices(unsigned int numV);
	void setNoEdges(unsigned int numE);
	void printMST();
	void addEdge(Edge &temp);
	inline mstItr mstBegin()
	{
		return mstTree.begin();
	}
	inline mstItr mstEnd()
	{
		return mstTree.end();
	}


};

unsigned int MST::getMSTSize()
{
	return mstTree.size();
}

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

void MST::addEdge(Edge &temp)
{
	mstTree.push_back(temp);
}


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
