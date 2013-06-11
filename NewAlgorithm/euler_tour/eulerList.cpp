#include "eulerElem.h"
#include "eulerList.h"

inline EulerElem EulerList::getEulerElem(unsigned int pos)
{
	assert (pos < eulerList.size());	
	return eulerList[pos];
} 
void EulerList::createList(DirectedGraph dag)
{
	DirectedGraph::const_dirEdgeItr eItr;
	unsigned int pos;
	//eulerListType twinPointer(eulerList.size());

	STXXL_MSG("Inside EulerList::createList");
	eulerList.clear();
	EulerElem *elem;

	for(eItr= dag.getFirstEdge(); !(dag.checkEdgeListEnd(eItr)) ;eItr++)
	{
		elem = new EulerElem(eItr->getSrc(),eItr->getDst(),eItr->getEdgeWt(),0);
		eulerList.push_back(*elem);
		eulerList.push_back(*elem);
		eulerList.back().swap();
		delete elem;
	}

	

	//twinPointer = eulerList;*/
	stxxl::sort(eulerList.begin(),eulerList.end(),eulerCmpDst(),INTERNAL_MEMORY_FOR_SORTING);

	unsigned int temp;
	

	for(unsigned int i=0; i<eulerList.size();)
	{
		temp=i;
		while(eulerList[i+1].getDst() == eulerList[temp].getDst())
		{
			//twinElem = twinPointer[i].getTwinPos();
			eulerList[i].setSucc(i+1);
			i++;
		}

		//twinElem = twinPointer[i].getTwinPos();
		eulerList[i].setSucc(temp);
		i++;		
	}

	stxxl::sort(eulerList.begin(),eulerList.end(),eulerCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	listItr itr;
	for(itr = eulerList.begin(),pos=0; itr!= eulerList.end(); itr++,pos++)
	{
		itr->setPos(pos);
	}

	//stxxl::sort(eulerList.begin(),eulerList.end(),eulerCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	//eulerList = twinPointer;
	STXXL_MSG("End EulerList::createList "<<eulerList.size());

}

void EulerList::printList()
{
	
	unsigned int temp;	
	stxxl::VECTOR_GENERATOR<unsigned int, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result visited(eulerList.size());
	
	for(unsigned int j=0; j<eulerList.size();j++)
		visited[j] = 0;

	STXXL_MSG("** Euler List **");
	
	for(unsigned int i=0; i<eulerList.size();i++)
	{
		if(!visited[i])
		{
			visited[i] = 0;
			STXXL_MSG(" (" <<(eulerList[i].getSrc())<<", " <<(eulerList[i].getDst())<<", "<<(eulerList[i].getSucc())<<") ");
			temp = eulerList[i].getSucc();
			while(temp!=i)
			{
				visited[temp]=1;
				STXXL_MSG(" (" <<(eulerList[temp].getSrc())<<", " <<(eulerList[temp].getDst())<<", "<<(eulerList[temp].getSucc())<<") ");
				temp = eulerList[temp].getSucc();
			}
		}
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

	EulerList euler(dag.getNoEdges());

	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();

	euler.createList(dag);
	
	STXXL_MSG("Euler List elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	euler.printList();

	return 0;
} 
*/
