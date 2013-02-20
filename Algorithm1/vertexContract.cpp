#include<math.h>
#include<iostream>
#include<ctime>
#include <limits>
#include <stxxl/vector>
#include <stxxl/priority_queue>
#include <stxxl/scan>
#include <stxxl/sort>
#include <stxxl/map>

#include "config.h"
#include "graph.cpp"
#include "mst.h"
#include "directedGraph.cpp"


struct pqVertex
{
	Vertex v;
	unsigned int id;
	unsigned int edgeWt;
	pqVertex(){}
	pqVertex(Vertex v_,unsigned int id_,unsigned int wt_):v(v_),id(id_),edgeWt(wt_){}
};

struct PQCmp 
{
     bool operator ()  (const pqVertex & a, const pqVertex & b) const
     {
	 return a.edgeWt > b.edgeWt;
     }

     pqVertex min_value() const
     {
	 return pqVertex(NULL,0,std::numeric_limits <unsigned int >::max());
     }
};


class VertexContract
{
public: 
	typedef typename DirectedGraph::dirEdgeType dirEdgeType;
	typedef typename DirectedGraph::dirEdgeItr edgeItr;
	

	typedef std::pair<DirectedEdge,edgeItr> structList;
	typedef stxxl::VECTOR_GENERATOR<structList,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result listType;
	
	typedef std::pair<int,int> represVertex;
	typedef stxxl::VECTOR_GENERATOR<represVertex, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result represVector;
	typedef represVector::iterator represVerItr;

private:
	dirEdgeType list;
	represVector resultVector;	

public:

	
	VertexContract(){}
	VertexContract(unsigned int num_e):list(2*num_e){}

	void contractVertices(DirectedGraph &dag);
	void createList(DirectedGraph &dag);
	//void replaceEdge(graph &g);

};

struct myCmpFir
{
  	VertexContract::represVertex min_value() const { 
		return VertexContract::represVertex(std::numeric_limits<int>::min(),0); };
	VertexContract::represVertex max_value() const { 
		return VertexContract::represVertex(std::numeric_limits<int>::max(),0); };
	bool operator () (const VertexContract::represVertex & a, const VertexContract::represVertex & b) const {
		return a.first < b.first || (a.first == b.first && a.second < b.second);
	}
};

struct myCmpSec
{
  	VertexContract::represVertex min_value() const { 
		return VertexContract::represVertex(std::numeric_limits<int>::min(),0); };
	VertexContract::represVertex max_value() const { 
		return VertexContract::represVertex(std::numeric_limits<int>::max(),0); };
	bool operator () (const VertexContract::represVertex & a, const VertexContract::represVertex & b) const {
		return a.second < b.second || (a.second == b.second && a.first < b.first);
	}
};


struct myCmpList
{

	VertexContract::dirEdgeType &parent;
	myCmpList(VertexContract::dirEdgeType &l):parent(l)
	{
		//parent = l;
	}

	VertexContract::structList min_value() const { 
		return VertexContract::structList(DirectedEdge(0,0,std::numeric_limits<unsigned int>::min()),parent.end()); };
	
  	VertexContract::structList max_value() const { 
		return VertexContract::structList(DirectedEdge(0,0,std::numeric_limits<unsigned int>::max()),parent.end()); };
	
	bool operator () (const VertexContract::structList & a, const VertexContract::structList & b) const {
		return a.first.getEdgeWt() < b.first.getEdgeWt();
	}
};


void VertexContract::createList(DirectedGraph &dag)
{
	edgeItr e_itr,end_itr;
	//DirectedEdge *e;
	
	
	dirEdgeType secCompList,firCompList,mergeList;
	listType tempList;
	structList temp;

	list.clear();secCompList.clear();firCompList.clear();mergeList.clear();
	tempList.clear();
	

	dag.copyEdgeList(secCompList);
	firCompList = secCompList;
	//dag.copyEdgeList(firCompList);

	//sort one instance of the list of edges by the second component
	stxxl::sort(secCompList.begin(),secCompList.end(),dirCmpDst(),INTERNAL_MEMORY_FOR_SORTING);	
	//sort another instance by the first component
	stxxl::sort(firCompList.begin(),firCompList.end(),dirCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);	

	edgeItr srcItr, dstItr;
	
	for(srcItr = firCompList.begin(), dstItr = secCompList.begin() ; dstItr!= secCompList.end();dstItr++)
	{
		
		mergeList.push_back(*dstItr);
		
		temp.first = *dstItr;
		temp.second= mergeList.end() -1;
		tempList.push_back(temp);

		while(srcItr->getSrc() < dstItr->getDst()&& srcItr != firCompList.end())
			srcItr++;
		while(srcItr->getSrc() == dstItr->getDst()&& srcItr != firCompList.end())
		{	mergeList.push_back(*srcItr);
			srcItr++;
		}
	}
	
	STXXL_MSG("Print merge list");
	for(e_itr=mergeList.begin();e_itr!=mergeList.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
		
	}

	
	//Sort list structure on edge weight
	stxxl::sort(tempList.begin(),tempList.end(),myCmpList(mergeList),INTERNAL_MEMORY_FOR_SORTING);	
				
	for(listType::iterator itr=tempList.begin(); itr!=tempList.end();itr++)
	{
		e_itr = itr->second;
		list.push_back(*e_itr);
		e_itr++;
		while(e_itr->getSrc() == itr->first.getDst() )
		{
			list.push_back(*e_itr);
			e_itr++;
			if(!e_itr->equals(*(e_itr+1)))
				break;
		
			
		}
	}
			
	
	STXXL_MSG("List created");
	for(e_itr=list.begin();e_itr!=list.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
		
	}
	
}
void VertexContract::contractVertices(DirectedGraph &dag)
{

	typedef stxxl::PRIORITY_QUEUE_GENERATOR < pqVertex, PQCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqVertex)> pqGen;	
	typedef pqGen::result pqType;
	typedef pqType::block_type blockType;
	
	stxxl::read_write_pool<blockType> pool((PQUEUE_MEM_POOL / 2) / blockType::raw_size, (PQUEUE_MEM_POOL / 2) / blockType::raw_size);
        pqType pqFwdProcess(pool);
	pqVertex *pqElem;

	createList(dag);
	edgeItr rootItr,e_itr,temp;
	represVertex newRepres;
	Vertex *v; 

	
	for(rootItr=dag.getFirstRoot(); !(dag.checkRootsEnd(rootItr)); rootItr++)
	{
		temp = stxxl::find(dag.getFirstEdge(), dag.getEdgeListEnd(), *rootItr, 4);
		for(;!(dag.checkEdgeListEnd(temp)) && temp->getSrc() == rootItr->getSrc(); temp++)
			{	
				v = new Vertex(temp->getDst());
				//STXXL_MSG("PQ push:"<<v->getVertexId()<<" "<<rootItr->getSrc()<<" "<<temp->getEdgeWt());
				pqElem = new pqVertex(*v,rootItr->getSrc(),temp->getEdgeWt());
				pqFwdProcess.push(*pqElem);
			}
		
	}
	
	for(e_itr=list.begin();e_itr!=list.end()&&!pqFwdProcess.empty();e_itr++)
	{
		assert(!pqFwdProcess.empty());
		*pqElem=pqFwdProcess.top();
		pqFwdProcess.pop();
	
		newRepres.first=(pqElem->v).getVertexId();
		newRepres.second=pqElem->id;
		resultVector.push_back(newRepres);

		for(temp=e_itr+1;temp!=list.end() && temp->getSrc() == e_itr->getDst() && !(*temp==*(temp-1));temp++)
		{
			v = new Vertex(temp->getDst());
			//STXXL_MSG("PQ push:"<<v->getVertexId()<<" "<<pqElem->id<<" "<<temp->getEdgeWt());
			pqElem = new pqVertex(*v,pqElem->id,temp->getEdgeWt());
			pqFwdProcess.push(*pqElem);
		}
		e_itr=temp-1;
		//STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
	
	}

	while(!pqFwdProcess.empty())
	{
		*pqElem=pqFwdProcess.top();
		pqFwdProcess.pop();
	
		newRepres.first=(pqElem->v).getVertexId();
		newRepres.second=pqElem->id;
		resultVector.push_back(newRepres);
	}
			

	//dag.roots.clear();
	list.clear();
	//pqFwdProcess.~priority_queue();
	STXXL_MSG("Vertices contracted");


	for(represVerItr itr = resultVector.begin(); itr!=resultVector.end();itr++)
	{	
		
		STXXL_MSG(" "<<itr->first<<" --> " <<itr->second);
	}
	
	
}

	
		




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

	VertexContract vc(dag.getNoEdges());

	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();


	vc.contractVertices(dag);
	
	STXXL_MSG("Vertex Contraction elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	return 0;
}
