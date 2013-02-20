#ifndef MST_H
#define MST_H
#endif 


class MST
{
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result mstType;
	mstType mstTree;
	unsigned int noVertices;
	unsigned int noEdges;


public:

	MST(){}
	MST(int num_v,int num_e):mstTree(num_v),noVertices(num_v),noEdges(num_e)
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
	Graph::edgeItr itr;
	
	STXXL_MSG("** Minimum Spanning Tree **");
	STXXL_MSG("No of vertices: "<<noVertices);
	STXXL_MSG("Number of edges: "<<noEdges);
	
	for(itr=mstTree.begin();itr!=mstTree.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
		
	}

}
