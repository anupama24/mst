#include<math.h>
#include<iostream>
#include<ctime>
#include <limits>
#include <stxxl/vector>
#include <stxxl/priority_queue>
#include <stxxl/scan>
#include <stxxl/sort>
#include <stxxl/map>

#include "node.h"
#include "pqElem.h"

#include "../config.h"
#include "../graph.cpp"
#include "../directedGraph.cpp"
#include "../euler_tour/eulerList.cpp"

class ListRanking {

public:
	
	ListRanking() {
		STXXL_MSG("Creating a set of disjoint linked lists, addr=" << this);
	}

	void listRank(DirectedGraph dag,EulerList euler);
		
};

void ListRanking::listRank(DirectedGraph dag,EulerList euler)
{
	EulerList::listItr itr;
	Node *newNode;
	
	unsigned int temp,i,j,succ,fir;	
	bool deleted;
	
	for(i=0,succ=0; i<euler.eulerList.size();i++)
	{
		deleted = false;
		if(!visited[i])
		{
			visited[i] = 0;
			fir = succ;
			if(!dag.isRoot(eulerList[i].getSrc(),eulerList[i].getDst(),eulerList[i].getEdgeWt())
			{
				newNode = new Node(eulerList[i].getSrc(),eulerList[i].getDst(),eulerList[i].getEdgeWt());
				linkedList.push_back(*newNode);			
			}
			else
				deleted = true;


			//STXXL_MSG(" (" <<(eulerList[i].getSrc())<<", " <<(eulerList[i].getDst())<<", "<<(eulerList[i].getEdgeWt())<<") ");

			temp = eulerList[i].getSucc();
			while(temp!=i)
			{
				visited[temp]=1;
				if(!deleted && dag.isRoot(eulerList[temp].getSrc(),eulerList[temp].getDst(),eulerList[temp].getEdgeWt())		
				{
					deleted =true;
				}
				else
				{	j = succ;
					linkedList[j].setSucc(++succ);
					newNode = new Node(eulerList[temp].getSrc(),eulerList[temp].getDst(),eulerList[temp].getEdgeWt());
					linkedList.push_back(*newNode);
				}
					
				//STXXL_MSG(" (" <<(eulerList[temp].getSrc())<<", " <<(eulerList[temp].getDst())<<", "<<(eulerList[temp].getEdgeWt())<<") ");
				
				temp = eulerList[temp].getSucc();
			}
			if(fir!=succ)
			{
				linkedList[succ].setSucc(fir);
			}
					
		}
	}
}
	
	
void ListRanking::listRank()
{

	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqNode, pqNodeCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqVertex)>::result pqNodeType;
	typedef pqNodeType::block_type nodeBlockType;
	
	stxxl::read_write_pool<nodeBlockType> pool1((PQUEUE_MEM_POOL / 2) / nodeBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / nodeBlockType::raw_size);
        pqNodeType pqAdjList(pool1);
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqRef, pqRefCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqVertex)>::result pqRefType;
	typedef pqRefType::block_type refBlockType;
	
	stxxl::read_write_pool<refBlockType> pool2((PQUEUE_MEM_POOL / 2) / refBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / refBlockType::raw_size);
        refBlockType pqRefList(pool2);

	typedef stxxl::PRIORITY_QUEUE_GENERATOR <pqDist, pqDistCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqVertex)>::result pqDistType;
	typedef pqDistType::block_type distBlockType;
	
	stxxl::read_write_pool<distBlockType> pool3((PQUEUE_MEM_POOL / 2) / distBlockType::raw_size, (PQUEUE_MEM_POOL / 2) / distBlockType::raw_size);
        pqDistType pqDist(pool3);

	typedef STACK_GENERATOR < unsigned int, external, grow_shrink2, 1, BLOCK_SIZE >::result rankStackType;
	prefetch_pool<rankStackType::block_type> p_pool(10);
        write_pool<rankStackType::block_type> w_pool(10);
        rankStackType rankStack(p_pool, w_pool, 0);

	pqNode *pqElem,*v,*w;
	listItr itr;
	unsigned int i,r,dist;
	pqRef *ref;
	pqDist *refDist;	
	for(itr = liskedList.begin(), i=0; itr!=lisnkedList.end(); itr++,i++)	
	{
		if(itr->getSucc() != std::numeric_limits<unsigned int>::max())
		{
			if((itr+1)->getSucc() == std::numeric_limits<unsigned int>::max())
				r = (itr+1)->getDst();
			else
				r =0;
			if(i < itr->getSucc())
			{
				pqElem = new pqNode(itr->getSucc(), i, r);
				pqAdjList.push(*pqElem);
			}
			else
			{
				pqElem = new pqNode(i, itr->getSucc(), r);
				pqAdjList.push(*pqElem);
			}
		}
	}
	
	while(!pqAdjList.empty())
	{
		*w = pqAdjList.top();
		pqAdjList.pop();
		if(w->srcId == 0)
		{	dist = w->rank;
			if(!pqAdjList.empty())
			{
				*v= pqAdjList.top();
				pqAdjList.pop();
				dist = dist ^ v->rank;
			}
			refDist=new pqDist(w->srcId,dist);	
			pqDist.push(*refDist);
			break;
		}
			
		if(w->srcId == pqAdjList.top()->srcId)
		{
			*v= pqAdjList.top();
			pqAdjList.pop();
			r = v->rank ^ w->rank;
			pqElem = new pqNode(w->dstId,v->dstId, r);
			pqAdjList.push(*pqElem);
		}
		else
		{	
			r= w->rank;
			pqElem = new pqNode(w->dstId,std::numeric_limits<unsigned int>::min(), r);
			pqAdjList.push(*pqElem);	
		}

		rankStank.push(w->rank);
		ref = new pqRef(w->srcId,w->dstId);
		pqRefList.push(*ref);
	
	}

	while(!pqDist.empty())
	{
		*refDist = pqDist.top();
		pqDist.pop();	
		dist = rankStank.top();
		rankStack.pop();
		liskedList[refDist->nodeId].setRoot(refDist->refDist);

		*ref = pqRefList.top();
		while(ref->refNodeId == refDist->nodeId)
		{
			pqRefList.pop();
			refDist = new pqDist(ref->nodeId,refDist->refDist
	
		
			
		
		
		
			
				

	



