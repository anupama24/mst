#include "directedGraph.h"


unsigned int DirectedGraph::getNoEdges()
{
	return noEdges;
}
void DirectedGraph::setNoEdges(unsigned int numE)
{
	noEdges = numE;
}


void DirectedGraph::createGraph(Graph &g,MST &mst)
{

	
	Graph::const_edgeItr minItr;
	Graph::const_vertexItr vItr;
	
	DirectedEdge *e;
		
	//g.printGraph();
	
	dirEdgeList.clear();



	STXXL_MSG("Check");
	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
	
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)); vItr++)
	{
		if(!(g.checkEdgeListEnd(vItr->second)) && vItr->second->getEdgeWt() <= (vItr->first).getBlockingValue())
		{
			minItr=vItr->second;
			e = new DirectedEdge(minItr->getDst(),minItr->getSrc(),minItr->getOrigDst(),minItr->getOrigSrc(),minItr->getEdgeWt());
			dirEdgeList.push_back(*e);
		}
		
	
	}

	noEdges=dirEdgeList.size();	

	stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),dirCmpWt(),INTERNAL_MEMORY_FOR_SORTING);
	STXXL_MSG("Directed graph created");
	std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	//printGraph();
	
}

void DirectedGraph::detectCycle(MST &mst)
{
		//dirVertexItr vItr;
		dirEdgeItr eItr,result;
		const_dirEdgeItr itr,NewEnd;
		dirEdgeType cycleEdges;
		dirEdgeType rev = dirEdgeList;
		Edge *e;

		stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

		
		stxxl::sort(rev.begin(),rev.end(),dirCmpWtRev(),INTERNAL_MEMORY_FOR_SORTING);
				
		roots.clear();
		
		
		/*for(eItr=dirEdgeList.begin(),itr=rev.begin(); eItr!=dirEdgeList.end() && itr!= rev.end(); eItr++,itr++)
		{
			STXXL_MSG(eItr->getSrc()<<" "<<eItr->getDst()<<" "<<eItr->getEdgeWt()<<"\t "<<itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
		}*/

		for(eItr=dirEdgeList.begin(),itr=rev.begin(); eItr!=dirEdgeList.end(); eItr++)
		{
			while(itr!=rev.end() && ((itr->getDst() < eItr->getSrc() && itr->getEdgeWt() == eItr->getEdgeWt()) || itr->getEdgeWt() < eItr->getEdgeWt()))
				itr++;
			if(itr!=rev.end() && eItr->equals(*itr) && eItr->getSrc() < eItr->getDst())
			{
				cycleEdges.push_back(*itr);	
				roots.push_back(*eItr);
				itr++;
						
			}		
				
		}
		rev.clear();
		//STXXL_MSG("Detect");	
		
		//stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),dirCmpEdge(),INTERNAL_MEMORY_FOR_SORTING);	
				
		if(!cycleEdges.empty())
		{
			stxxl::sort(cycleEdges.begin(),cycleEdges.end(),dirCmpWt(),INTERNAL_MEMORY_FOR_SORTING);
			itr = cycleEdges.begin();
			result = dirEdgeList.begin();
			for(eItr=dirEdgeList.begin();eItr!=dirEdgeList.end() && itr!= cycleEdges.end();eItr++)
			{
				if(*eItr==*itr)
				{
					itr++;					
				}
				else
				{	*result=*eItr;
					result++;
					e = new Edge(eItr->getOrigDst(), eItr->getOrigSrc(), eItr->getEdgeWt());
					mst.addEdge(*e);
				}
			}
			for(;eItr!=dirEdgeList.end();eItr++)
			{	*result=*eItr;
				result++;
				e = new Edge(eItr->getOrigDst(), eItr->getOrigSrc(), eItr->getEdgeWt());
				mst.addEdge(*e);
			}
			NewEnd = result;
			dirEdgeList.resize(NewEnd - dirEdgeList.begin());
			cycleEdges.clear();
			
		}

		
		noEdges=dirEdgeList.size();
		stxxl::sort(dirEdgeList.begin(),dirEdgeList.end(),dirCmpEdge(),INTERNAL_MEMORY_FOR_SORTING);	
		stxxl::sort(roots.begin(),roots.end(),dirCmpEdge(),INTERNAL_MEMORY_FOR_SORTING);
					
		//printGraph();
		
		STXXL_MSG("Number of edges: "<<noEdges<<std::endl<<" No of components: "<<roots.size());
		STXXL_MSG("Cycle detected");
		STXXL_MSG("MST size: "<<mst.getMSTSize());
		std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
		
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



		
		

	
		
	
	
