#include "externalPrim.h"

void ExternalPrim::buildMST(Graph &g,MST &mst)
{
	
	typedef stxxl::PRIORITY_QUEUE_GENERATOR <Edge, myMstCmp, PQUEUE_INTERNAL_MEMORY , PQUEUE_MAX_SIZE/sizeof(Edge)> pqGen;	
	typedef pqGen::result pqType;
	typedef pqType::block_type blockType;
	
	//const unsigned mem_for_pools = 128 * 1024 * 1024 ;
	
	Graph::edgeItr itr;
	Graph::vertexItr v_itr;
	Edge e_temp;
	Graph::vertexType result;
	//int i = 1;
        

	stxxl::read_write_pool<blockType> pool((PQUEUE_MEM_POOL / 2) / blockType::raw_size, (PQUEUE_MEM_POOL / 2) / blockType::raw_size);
        pqType pqMST(pool);
	
	STXXL_MSG("stxxl::pq block size: " << blockType::raw_size << " bytes");	
	STXXL_MSG("Internal memory consumption of the priority queue: " << pqMST.mem_cons() << " bytes");
     
	//mst.clearMST();

	for(v_itr = g.getFirstVertex(); !(g.checkVertexListEnd(v_itr)) && g.checkEdgeListEnd(v_itr->second); v_itr++);
	
	for(itr=v_itr->second; !(g.checkEdgeListEnd(itr)) && itr->getSrc() == v_itr->first.getVertexId();itr++)
	{
		//STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
		pqMST.push(*itr);
	}
	
	do
	{
		assert(!pqMST.empty());
		e_temp=pqMST.top();

		//STXXL_MSG(" (" <<(e_temp.src)<<", " <<(e_temp.dst)<<", "<<(e_temp.edge_wt)<<") ");
		
		pqMST.pop();
		
		if((e_temp.equals(pqMST.top())) && e_temp.getEdgeWt() == pqMST.top().getEdgeWt())
			pqMST.pop();
		else
		{
			mst.addEdge(e_temp);			
			//index = e_temp.dst;
			for(v_itr =g.getFirstVertex(); !(g.checkVertexListEnd(v_itr)) && v_itr->first.getVertexId() != e_temp.getDst(); v_itr++);
			
			for(itr=v_itr->second; !(g.checkEdgeListEnd(itr)) &&  itr->getSrc() == v_itr->first.getVertexId();itr++)
			{
				if(!(e_temp.equals(*itr)))
				{
					//STXXL_MSG(" (" <<(itr->src)<<", " <<(itr->dst)<<", "<<(itr->edge_wt)<<") ");
					pqMST.push(*itr);
				}
			}
			
		}
		
			
	}
	while(!pqMST.empty() && mst.getMSTSize() < (unsigned int) mst.getNoVertices()-1);
	//mst.setNoEdges(g.getNoVertices()-1);
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
	MST mst(num_v,num_e);
	ExternalPrim prim;

	g.generateGraph();
	g.printGraph();

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
    	stxxl::timer Timer;
	Timer.start();
	
	prim.buildMST(g,mst);
	mst.printMST();
	
	STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;

	return 0;
}

*/
