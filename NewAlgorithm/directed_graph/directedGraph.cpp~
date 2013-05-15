#include "directedGraph.h"


unsigned int DirectedGraph::getNoVertices()
{
	return noVertices;
}
void DirectedGraph::setNoVertices(unsigned int numV)
{
	noVertices = numV;
}

unsigned int DirectedGraph::getNoEdges()
{
	return noEdges;
}
void DirectedGraph::setNoEdges(unsigned int numE)
{
	noEdges = numE;
}


void DirectedGraph::createGraph(Graph::edgeType &edgeSet)
{

	
	Graph::edgeItr minItr;
	DirectedEdge *e;
		
	//g.printGraph();
	
	dirEdgeList.clear();

	STXXL_MSG("Check");
	
	for(minItr = edgeSet.begin(); minItr != edgeSet.end(); minItr++)
	{
		e = new DirectedEdge(minItr->getSrc(),minItr->getDst(),minItr->getEdgeWt());
		dirEdgeList.push_back(*e);
	}

	noEdges=dirEdgeList.size();	

	stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),dirCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	STXXL_MSG("Directed graph created");
	//printGraph();
	
}

void DirectedGraph::detectCycle(MST &mst,Graph &inputGraph)
{
	dirEdgeType destEdgeList;
	dirEdgeItr eItr,dstItr,result;

	destEdgeList = dirEdgeList;
	stxxl::sort(destEdgeList.begin(),destEdgeList.end(),dirCmpDst(),INTERNAL_MEMORY_FOR_SORTING);

	typedef stxxl::VECTOR_GENERATOR<pairType, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result pairListType;
	pairListType cycleList;

	pairListType::iterator listItr,parItr;
	pairType elem;
	Edge *e;

	
	dstItr = destEdgeList.begin();	
	for(eItr=dirEdgeList.begin();eItr!=dirEdgeList.end();eItr++)
	{
		while(dstItr!= destEdgeList.end()  && eItr->getSrc()==dstItr->getDst())
		{
			elem.first = dstItr->getSrc();
			elem.second = eItr->getDst();
			cycleList.push_back(elem);
			dstItr++;
		}
		while(dstItr!= destEdgeList.end() && dstItr->getDst()<=eItr->getSrc())
			dstItr++;
	}

	//STXXL_MSG("S' created");
	

	stxxl::sort(cycleList.begin(),cycleList.end(),cmpFirst(),INTERNAL_MEMORY_FOR_SORTING);
	Graph::vertexItr vItr = inputGraph.getFirstVertex();

	result = dirEdgeList.begin();
	for(listItr = cycleList.begin(),eItr=dirEdgeList.begin();eItr!=dirEdgeList.end() && listItr!= cycleList.end();eItr++)
	{
		
		while(listItr!= cycleList.end() && listItr->first < eItr->getSrc())
			listItr++;
		while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < eItr->getSrc())
				vItr++;
		if(listItr->first == listItr->second && listItr->first == eItr->getSrc() && eItr->getSrc() < eItr->getDst())
		{
		 	roots.push_back(*eItr);	
		}
		
		else
		{	*result=*eItr;
			result++;
			/*if(vItr->first.getVertexId() == eItr->getSrc())
			{
				e = new Edge(eItr->getOrigDst(), eItr->getOrigSrc(), eItr->getEdgeWt());
				mst.addEdge(*e);
			}*/
		}
	}

	//STXXL_MSG("S'' created");

	for(;eItr!=dirEdgeList.end();eItr++)
	{	
		while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < eItr->getSrc())
				vItr++;
		*result=*eItr;
		result++;
		/*if(vItr->first.getVertexId() == eItr->getSrc())
		{
			e = new Edge(eItr->getOrigDst(), eItr->getOrigSrc(), eItr->getEdgeWt());
			mst.addEdge(*e);
		}*/
	}
	dirEdgeList.resize(result - dirEdgeList.begin());

	noEdges=dirEdgeList.size();
	//stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),dirCmpEdge(),INTERNAL_MEMORY_FOR_SORTING);	
	
				
	//printGraph();
	
	STXXL_MSG("Number of edges: "<<noEdges<<std::endl<<" No of roots: "<<roots.size());
	//STXXL_MSG("MST size: "<<mst.getMSTSize());
	STXXL_MSG("Cycle detected");
	
	destEdgeList.clear();	
	cycleList.clear();			
		
	

}
		
void DirectedGraph::addEdgesMST(StarGraph &star,MST &mst,Graph &inputGraph)
{
	StarGraph::starItr starItr;
	dirEdgeItr eItr;
	starItr = star.begin();
	Edge *e;

	Graph::vertexItr vItr = inputGraph.getFirstVertex();
	if(roots.size()!=0)
	{	
		for(eItr=dirEdgeList.begin();eItr!=dirEdgeList.end();eItr++)
		{
			while(starItr!=star.end() && starItr->starEdge.getSrc() < eItr->getSrc())
				starItr++;
			while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < eItr->getSrc())
				vItr++;
			if(starItr->starEdge.getSrc() == eItr->getSrc() && vItr->first.getVertexId() == eItr->getSrc())
			{
				e = new Edge(eItr->getOrigDst(), eItr->getOrigSrc(), eItr->getEdgeWt());
				mst.addEdge(*e);
			}
		}
	}
	STXXL_MSG("MST size: "<<mst.getMSTSize());
}
		

	


void DirectedGraph::copyEdgeList(dirEdgeType &list)
{
	list = dirEdgeList;
}

DirectedGraph::dirEdgeItr DirectedGraph::getFirstRoot()
{
	return roots.begin();
}
DirectedGraph::dirEdgeItr DirectedGraph::getFirstEdge()
{
	return dirEdgeList.begin();
}
DirectedGraph::dirEdgeItr DirectedGraph::getEdgeListEnd()
{
	return dirEdgeList.end();
}

DirectedGraph::dirEdgeItr DirectedGraph::getRootEnd()
{
	return roots.end();
}

bool DirectedGraph::isRoot(unsigned int src,unsigned int dst,unsigned int wt)
{
	DirectedEdge *e;
	e = new DirectedEdge(src,dst,wt);	
	
	dirEdgeItr temp = stxxl::find(roots.begin(),roots.end(),*e, 4);

	return (temp!=roots.end());

}



void DirectedGraph::printGraph()
{
	dirEdgeItr itr;
	
	STXXL_MSG("**Directed Graph -  Edge Listing **"<<std::endl);
	STXXL_MSG("No of vertices: "<<noVertices);
	STXXL_MSG("Number of edges: "<<noEdges<<std::endl);
	
	for(itr=dirEdgeList.begin();itr!=dirEdgeList.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
	}
	STXXL_MSG("\n**Directed Graph Components -  Roots Listing **");
	for(itr=roots.begin();itr!=roots.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
	}
	
}

/*
int main()
{
	unsigned int num_v,num_e;
	
	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	std::cin>>num_v;
	STXXL_MSG("Edges :");
	std::cin>>num_e;
	
	Graph g(num_v,num_e);
	g.generateGraph();
	g.printGraph();

	
	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;
	Timer.start();
	DirectedGraph dag(num_v,num_e);
	dag.createGraph(g);	
	dag.detectCycle();
	
	

	STXXL_MSG("Directed Graph creation elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_e) / (Timer.mseconds() / 1000.)) << " edges per sec");

    	std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	dag.printGraph();
	return 0;
}
	
*/



		
		

	
		
	
	
