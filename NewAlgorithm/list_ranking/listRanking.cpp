#include "pqElem.h"
class ListRanking {

	stxxl::VECTOR_GENERATOR<unsigned int, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result rootId;
	EulerList::eulerListType disjointList;
	DirectedGraph &dag;
	StarGraph &star;
	EulerList &euler;

public:
	
	typedef EulerList::const_listItr listItr;
	ListRanking(DirectedGraph &dag_,StarGraph &star_,EulerList &euler_):dag(dag_),star(star_),euler(euler_) {
		STXXL_MSG("Creating a set of disjoint linked lists");
	}

	void createList();
	void listRank();
	void updateStarEdges();
	void printList();
};

void ListRanking::createList()
{
	listItr itr;
	DirectedGraph::const_dirEdgeItr rootItr;
	DirectedEdge *e;
	STXXL_MSG("Inside create list");
	
	for(itr = euler.getFirstElem(),rootItr=dag.getFirstRoot(); !(euler.checkEulerListEnd(itr)); itr++)
	{
		e = new DirectedEdge(itr->getSrc(),itr->getDst(),itr->getEdgeWt());
		if(rootItr!=dag.getRootEnd() && *e == *rootItr)
		{			
			rootId.push_back(itr->getPos());
			rootItr++;
					
		}
		else
		{	disjointList.push_back(*itr);
		}
		delete e;

			
	}

	stxxl::sort(disjointList.begin(),disjointList.end(),eulerCmpSucc(),INTERNAL_MEMORY_FOR_SORTING);
			
}
	
	
void ListRanking::listRank()
{

	STXXL_MSG("Begin rank list");
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqNode, pqNodeCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqNode)>::result pqNodeType;
	typedef pqNodeType::block_type nodeBlockType;
	
	stxxl::read_write_pool<nodeBlockType> pool1((PQUEUE_MEM_POOL / 2) / nodeBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / nodeBlockType::raw_size);
        pqNodeType pqAdjList(pool1);
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqRef, pqRefCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqRef)>::result pqRefType;
	typedef pqRefType::block_type refBlockType;
	
	stxxl::read_write_pool<refBlockType> pool2((PQUEUE_MEM_POOL / 2) / refBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / refBlockType::raw_size);
        pqRefType pqRefList(pool2);

	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqDist, pqDistCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqDist)>::result pqDistType;
	typedef pqDistType::block_type distBlockType;
	
	stxxl::read_write_pool<distBlockType> pool3((PQUEUE_MEM_POOL / 2) / distBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / distBlockType::raw_size);
        pqDistType pqRefDist(pool3);

	listItr itr;
	unsigned int j,r=0,dist=0;
	Edge *e;
	starElem *elem;
	

	if(rootId.size() == 0)
	{
		STXXL_MSG("Inside rank list: roots 0");
		for(itr = disjointList.begin(); itr!=disjointList.end(); itr++)	
		{
			if(itr->getSrc()< itr->getDst())
			{
				e = new Edge(itr->getDst(),itr->getSrc(),itr->getEdgeWt());
				elem = new starElem(0,*e);
				star.add(*elem);
				delete e;
				delete elem;
			}

		}
		return;
	}
	
	
	
	
	for(itr = disjointList.begin(),j=0; itr!=disjointList.end(); itr++)	
	{
		r =0;
		if(itr->getSucc() != std::numeric_limits<unsigned int>::max())
		{

			while(j<rootId.size() && itr->getSucc() > rootId[j])
				j++;

			if(j<rootId.size() && itr->getSucc()==rootId[j])
			{
				r = itr->getDst();
				j++;
			}
			else
				r =0;
			if(itr->getPos() < itr->getSucc())
			{
				//Edge e(itr->getSrc(),itr->getDst(),itr->getEdgeWt());
				pqNode pqElem(itr->getSucc()+1, itr->getPos()+1,r);
				pqAdjList.push(pqElem);
			}
			else
			{
				//Edge e(itr->getSrc(),itr->getDst(),itr->getEdgeWt());
				pqNode pqElem(itr->getPos()+1, itr->getSucc()+1, r);				
				pqAdjList.push(pqElem);
			}
		}
	}

	rootId.clear();
	disjointList.clear();

	STXXL_MSG("Adjacency queue created");
	
	while(!pqAdjList.empty())
	{
		
		pqNode pqElem=pqAdjList.top();
		pqNode w = pqElem;
		pqAdjList.pop();
		
		if(w.srcId == pqAdjList.top().srcId)
		{
			
			if (w.dstId == std::numeric_limits<unsigned int>::min() && pqAdjList.top().dstId == std::numeric_limits<unsigned int>::min())
			{
				if(w.rank!=0)
					dist = w.rank;
				else
					dist= pqAdjList.top().rank;
		
				
				pqDist refDist(w.srcId,dist);	
				pqRefDist.push(refDist);
				pqAdjList.pop();
				continue;
			}
			else	
			{	
				r = pqAdjList.top().rank ^ w.rank;
				pqNode pqElem(w.dstId,pqAdjList.top().dstId, r);
				pqAdjList.push(pqElem);		
				pqAdjList.pop();
			}
			
			
		}
		else if (w.dstId == std::numeric_limits<unsigned int>::min())
		{
				dist = w.rank;
				pqDist refDist(w.srcId,dist);	
				pqRefDist.push(refDist);
				continue;
		}
		else
		{	
			r= w.rank;
			pqNode pqElem(w.dstId,std::numeric_limits<unsigned int>::min(), r);
			pqAdjList.push(pqElem);	
		}

		
		pqRef ref(w.srcId,w.dstId);
		pqRefList.push(ref);
	
	}

	while(!pqRefDist.empty())
	{
		pqDist refDist=pqRefDist.top();
		pqRefDist.pop();	
		
		dist = refDist.refDist;
		Edge e(0,dist,std::numeric_limits<unsigned int>::max());
		starElem elem(((refDist.nodeId)-1),e);
		star.add(elem);
		r = refDist.nodeId;

		while(!pqRefList.empty() && pqRefList.top().refNodeId == r)
		{
			pqDist refDist(pqRefList.top().nodeId,dist);
			pqRefDist.push(refDist);
			pqRefList.pop();
			
		}
	}
	
	//printList(star);	
	STXXL_MSG("End rankList");
	

}

void ListRanking::printList()
{
	STXXL_MSG("** Ranked List **");
	
	for(StarGraph::starItr itr=star.begin();itr!=star.end();itr++)
	{
		STXXL_MSG(" (" <<itr->pos<<", " <<itr->starEdge.getSrc()<<", "<<itr->starEdge.getDst()<<", "<<itr->starEdge.getEdgeWt()<<") ");
				
	}
}

void ListRanking::updateStarEdges()
{

	
	STXXL_MSG("Inside update star edges");
	StarGraph::starItr itr,result;
	unsigned int i;

	//stxxl::sort(star.begin(),star.end(),starCmpPos(),INTERNAL_MEMORY_FOR_SORTING);
	
	for(i=0,itr=star.begin();i<euler.getSize() && itr!=star.end();i++)
	{
		while(itr!=star.end() && itr->pos < i)
			itr++;
		if(itr->pos== i && itr->starEdge.getDst()!=0)
		{	itr->starEdge.setSrc(euler.getEulerElem(i).getSrc());
			if(euler.getEulerElem(i).getSrc() == itr->starEdge.getDst())
				itr->starEdge.setSrc(euler.getEulerElem(i).getDst());
			itr->starEdge.setEdgeWt(euler.getEulerElem(i).getEdgeWt());
			itr++;
		}
	}
	
	euler.clear();

	STXXL_MSG("Star size: "<<star.size());

	stxxl::sort(star.begin(),star.end(),starCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	result = star.begin();
	for(itr=star.begin()+1;itr!=star.end();itr++)
	{
		if(itr!=star.end() && !(result->starEdge == itr->starEdge) && itr->starEdge.getEdgeWt() != std::numeric_limits<unsigned int>::max() && itr->starEdge.getDst()!=0 )	{
			if(result->starEdge.getSrc() !=0)
				result++;
			*result = *itr;
		}
	} 
	result++;
	star.resize(result,star.begin());

		

	STXXL_MSG("Star resize: "<<star.size());
	
			
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
	ListRanking rank;

	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();
	

	rank.createList(dag,euler);
	rank.listRank(euler);
	euler.printList();

	STXXL_MSG("List Ranking elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");
        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;

	

	return 0;
}			
*/		
		
		
			
				

	



