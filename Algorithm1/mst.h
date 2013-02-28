#ifndef MST_H
#define MST_H
#endif 


class MST
{
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result mstType;
	typedef Graph::edgeItr mstItr;
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

	unsigned int getMSTSize();
	unsigned int getNoVertices();
	unsigned int getNoEdges();
	void setNoVertices(unsigned int numV);
	void setNoEdges(unsigned int numE);
	void printMST();
	void addEdge(Edge &temp);
	void removeEdge(mstItr itr);
	inline mstItr mstBegin()
	{
		return mstTree.begin();
	}
	inline mstItr mstEnd()
	{
		return mstTree.end();
	}
	void clean();
	

	

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

void MST::removeEdge(mstItr itr)
{
	itr->setSrc(std::numeric_limits<unsigned int>::max());
	itr->setDst(std::numeric_limits<unsigned int>::max());
	itr->setEdgeWt(std::numeric_limits<unsigned int>::max());
}

void MST::clean()
{
	stxxl::sort(mstTree.begin(),mstTree.end(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);	
	mstItr NewEnd = std::unique(mstTree.begin(),mstTree.end());			
	mstTree.resize(NewEnd - mstTree.begin());
}

void MST::printMST()
{
	mstItr itr;
	
	STXXL_MSG("** Minimum Spanning Tree **");
	STXXL_MSG("No of vertices: "<<noVertices);
	STXXL_MSG("Number of edges: "<<mstTree.size());
	
	for(itr=mstTree.begin();itr!=mstTree.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getOrigSrc())<<", " <<(itr->getOrigDst())<<", "<<(itr->getEdgeWt())<<") ");
		
	}

}
