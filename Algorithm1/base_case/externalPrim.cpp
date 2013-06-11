#include "externalPrim.h"


/**********************************************************
buildMST(Graph &g,MST &mst)
input: The resultant graph g and the mst tree storing the mst edges
output: none
************************************************************/


void ExternalPrim::buildMST(Graph &g,MST &mst)
{
	//External priority queue storing Edge objects
	typedef stxxl::PRIORITY_QUEUE_GENERATOR <Edge, myMstCmp, PQUEUE_INTERNAL_MEMORY , PQUEUE_MAX_SIZE/sizeof(Edge)> pqGen;	
	typedef pqGen::result pqType;
	typedef pqType::block_type blockType;
	stxxl::read_write_pool<blockType> pool((PQUEUE_MEM_POOL / 2) / blockType::raw_size, (PQUEUE_MEM_POOL / 2) / blockType::raw_size);
        pqType pqMST(pool);


	Graph::const_edgeItr itr;
	Graph::const_vertexItr v_itr;
	Edge e_temp;
	Graph::vertexType result;
	
        STXXL_MSG("stxxl::pq block size: " << blockType::raw_size << " bytes");	
	STXXL_MSG("Internal memory consumption of the priority queue: " << pqMST.mem_cons() << " bytes");
     
	

	//Find the first vertex having an edge in the edgeVector. We select this vertex as the source
	for(v_itr = g.getFirstVertex(); !(g.checkVertexListEnd(v_itr)) && g.checkEdgeListEnd(v_itr->second); v_itr++);
	
	//Push all edges incident to source vertex in the priority queue
	for(itr=v_itr->second; !(g.checkEdgeListEnd(itr)) && itr->getSrc() == v_itr->first.getVertexId();itr++)
	{
		pqMST.push(*itr);
	}
	

	//Run until priority queue is not empty
	do
	{
		//Extract the minimum priority edges
		assert(!pqMST.empty());
		e_temp=pqMST.top();
		pqMST.pop();
		
		//Check if its an internal edge i.e. compare it with the next topmost edge in priority queue using equals function of the Edge class
		if((e_temp.equals(pqMST.top())) && e_temp.getEdgeWt() == pqMST.top().getEdgeWt())
			pqMST.pop();
		else
		{
			//Add edge to MST
			mst.addEdge(e_temp);		

			//Find destination vertex of e_temp in the vertex vector	
			for(v_itr =g.getFirstVertex(); !(g.checkVertexListEnd(v_itr)) && v_itr->first.getVertexId() != e_temp.getDst(); v_itr++);
			
			//Push all edges incident to destination vertex of e_temp in the priority queue except for e_temp
			for(itr=v_itr->second; !(g.checkEdgeListEnd(itr)) &&  itr->getSrc() == v_itr->first.getVertexId();itr++)
			{
				if(!(e_temp.equals(*itr)))
				{
					pqMST.push(*itr);
				}
			}
			
		}
		
			
	}
	while(!pqMST.empty() && mst.getMSTSize() < (unsigned int) mst.getNoVertices()-1);
	
}
