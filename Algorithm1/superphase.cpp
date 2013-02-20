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
#include "Graph.cpp"
#include "mst.h"
#include "directedGraph.cpp"
#include "vertexContract.cpp"


void reduceGraph(Graph &g,Graph &gnew,int blockValue)
{
	Graph::edgeItr eItr,newItr,newEnd;
	Graph::vertexItr vItr;
	Graph::vertexType result;	
	int i=0;


	gnew.clearList();	
	//STXXL_MSG("Reduce Graph");
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)); vItr++)
	{
		for(eItr= vItr->second,i=0; (!g.checkEdgeListEnd(eItr)) && eItr->getSrc()== (vItr->first).getVertexId() && i< blockValue ; i++,eItr++)
		{
			//STXXL_MSG("V: "<<(vItr->first).getVertexId()<<" E: "<<" (" <<(eItr->getSrc())<<", " <<(eItr->getDst())<<", "<<(eItr->getEdgeWt())<<")");			
			gnew.addEdge(*eItr);
			gnew.addEdge(*eItr);
			gnew.swapEdge();
		}
	
		//eItr++;
		//STXXL_MSG("Reduce Graph");
		if((!g.checkEdgeListEnd(eItr)) && eItr->getSrc()== (vItr->first).getVertexId())
			vItr->first.setBlockingValue(eItr->getEdgeWt());
		else
			vItr->first.setBlockingValue(std::numeric_limits<unsigned int>::max());
	}

	stxxl::sort(gnew.getFirstEdge(),gnew.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	newEnd = std::unique(gnew.getFirstEdge(),gnew.getEdgeListEnd());
	gnew.resizeList(newEnd);
	
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr));vItr++)
	{
			
		for(newItr=gnew.getFirstEdge(); !(gnew.checkEdgeListEnd(newItr)) && newItr->getSrc()!= (vItr->first).getVertexId(); newItr++);

		result.first = vItr->first;
		result.second = newItr;
		gnew.addVertex(result);


	}
	//STXXL_MSG("Reduce Graph");
	gnew.setNoEdges(gnew.getEdgeListSize()/2);
	gnew.setNoVertices(gnew.getVertexListSize());
	//gnew.print_Graph();

	STXXL_MSG("Graph reduced");

}
		
		
void maintainList(VertexContract::represVector &L,VertexContract::represVector &C)
{
	if(L.empty())
	{
		L = C;
		return;
	}
	if(C.empty())
		return;

	stxxl::sort(C.begin(),C.end(),myCmpFir(), INTERNAL_MEMORY_FOR_SORTING);
	stxxl::sort(L.begin(),L.end(),myCmpSec(), INTERNAL_MEMORY_FOR_SORTING);

	VertexContract::represVerItr it1,it2;

	it1=C.begin();
	for(it2=L.begin();it2!=L.end()&& it1!=C.end();)
	{
		if(it1->first==it2->second)
		{
			while(it1!=C.end() && it1->first==it2->second)
			{	it2->second = it1->second;
				it2++;
			}
			it1++;
		}
		else
			it2++;
	}

	STXXL_MSG("List updated");
}


void cleanEdges(Graph &g)
{

	
	typedef typename Graph::edgeItr edgeItr;
	typedef typename Graph::vertexItr vertexItr;

	
	edgeItr eItr;	
	vertexItr vItr;

	

	for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)); eItr++)
	{
		if(eItr->getSrc()==eItr->getDst())
		{
			eItr->getSrc()=eItr->getDst() =eItr->getEdgeWt()=std::numeric_limits<int>::max();
		}
	}

	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	
	edgeItr NewEnd = std::unique(g.getFirstEdge(),g.getEdgeListEnd());
	g.resizeList((NewEnd -1) - g.edge_list.begin());
	
	//g.print_Graph();
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);

	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)); vItr++)
	{
		for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)) && eItr->getSrc()!=(vItr->first).getVertexId(); eItr++);
			vItr->second = eItr;
		
	}

	g.setNoEdges(g.getEdgeListSize()/2);
	g.setNoVertices(g.getVertexListSize());

	STXXL_MSG("Edges clean");
	//g.print_Graph();
	
	

}
	
void minBlockingValue(Graph &g,VertexContract::represVector &list)
{
	stxxl::sort(list.begin(),list.end(),myCmpSec(), INTERNAL_MEMORY_FOR_SORTING);

	VertexContract::represVerItr it1,it2;
	int min,curr_v;
	typedef typename Graph::vertexItr vertexItr;
	
	vertexItr vItr,tempItr;
	
	Edge e;
	STXXL_MSG("Size of vertex main: "<<g.getVertexListSize());

	/*for(vItr=g.vmain.begin();vItr!= g.vmain.end();vItr++)
	{
		
		STXXL_MSG(" V: "<<(vItr->first).getVertexId()<<" & "<<(vItr->first).blocking_value);
			
	}*/	

	int i,j;
	for(it1=list.begin();it1!=list.end();)
	{
		//STXXL_MSG("it1->second"<<(it1->second));
		for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)) && (vItr->first).getVertexId() != it1->second; vItr++);
		min = (vItr->first).getBlockingValue();
		curr_v = it1->second;

		while(it1!=list.end() && it1->second == curr_v)
		{
			for(tempItr = g.getFirstVertex(); !(g.checkVertexListEnd(tempItr)) && (tempItr->first).getVertexId() != it1->first; tempItr++);
			if(!(g.checkVertexListEnd(tempItr)) && min > (tempItr->first).getBlockingValue())
			{
				min = (tempItr->first).getBlockingValue();
			}
			it1++;
		}
		(vItr->first).setBlockingValue(min);
		

	}

	for(it1=list.begin();it1!=list.end();it1++)
	{
		for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)) && (vItr->first).getVertexId() != it1->second; vItr++);
		
		curr_v = it1->second;

		while(it1!=list.end() && it1->second == curr_v)
		{
			for(tempItr = g.getFirstVertex(); !(g.checkVertexListEnd(tempItr)) && (tempItr->first).getVertexId() != it1->first; tempItr++);
			(tempItr->first).setVertexId(it1->second);
			(tempItr->first).setBlockingValue((vItr->first).getBlockingValue());
			g.vmain[j].second = g.vmain[i].second;
			it1++;
		}		
		
	}
	
	stxxl::sort(g.vmain.begin(),g.vmain.end(),Graph::my_cmp_ver(g), INTERNAL_MEMORY_FOR_SORTING);
	vertexItr NewEnd = std::unique(g.vmain.begin(),g.vmain.end());
	g.vmain.resize((NewEnd) - g.vmain.begin());
	g.no_vertices = g.vmain.size();

	STXXL_MSG("Size of vertex main: "<<g.vmain.size());
	/*for(vItr=g.vmain.begin();vItr!= g.vmain.end();vItr++)
	{
		STXXL_MSG(" V: "<<(vItr->first).getVertexId()<<" & "<<(vItr->first).blocking_value);
			
	}*/
	STXXL_MSG("Blocking value set");

}

void replaceEdge(Graph &g,stxxl::vector<std::pair<int,int> > &result)
{
	stxxl::vector<std::pair<int,int> >::iterator it;
	
	typedef typename Graph::edgeItr edgeItr;
	typedef typename Graph::vertexItr vertexItr;

	
	edgeItr eItr;	
	vertexItr vItr;
	
	stxxl::sort(result.begin(),result.end(),my_cmp_list(),INTERNAL_MEMORY_FOR_SORTING);
	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),my_cmp_getSrc()(),INTERNAL_MEMORY_FOR_SORTING);

	/*for(it=result.begin();it!=result.end();it++)
	{
		STXXL_MSG(" "<<it->first<<"-->"<<it->second);
	}
	*/

	it=result.begin();
	
	

	for(eItr=g.edge_list.begin();eItr!=g.edge_list.end() && it!=result.end();)
	{
		if(it->first==eItr->getSrc())
		{
			while(it!=result.end() && it->first==eItr->getSrc())
			{	eItr->getSrc() = it->second;
				eItr++;
			}
			it++;
		}
		else
			eItr++;
			
	}

	

	stxxl::sort(g.edge_list.begin(),g.edge_list.end(),my_cmp_getDst()(),INTERNAL_MEMORY_FOR_SORTING);
	//STXXL_MSG("Vertex Contraction");
	//g.print_Graph();
	
	it=result.begin();
	for(eItr=g.edge_list.begin();eItr!=g.edge_list.end()&& it!=result.end();)
	{
		if(it->first==eItr->getDst())
		{
			while(it!=result.end() && it->first==eItr->getDst())
			{	eItr->getDst() = it->second;
				eItr++;
			}
			it++;
		}
		else
			eItr++;
	}
	//STXXL_MSG("Vertex Contraction");
	//g.print_Graph();
	minBlockingValue(g,result);
	cleanEdges(g);	
	STXXL_MSG("Edge replaced");
	
}
	


void superphaseAlgo(Graph &g,int count)
{
	typedef std::pair<int,int> repres_vertex;
	stxxl::vector<repres_vertex> list;	
	Graph gnew;

	int logNi,blockValue;
	
	float Ni = 2,prevNi,rtNi,lgNi;
	float B = (float) BLOCK_SIZE/(float)(sizeof(edge) + sizeof(vertex));

	DirectedGraph dag(g.no_vertices,g.no_edges);
	dag.createGraph(g);
	dag.detectCycle();
			
	VertexContract vc(dag.no_edges);
	vc.contractVertices(dag);
	replaceEdge(g,vc.result);
	//g.print_Graph();
	//list = vc.result;

	STXXL_MSG("Vertices "<<g.no_vertices<<" Edge: "<<g.no_edges<<" count: "<<count);

	for(int i=0;i<count;i++)
	{
		list.clear();
		rtNi = sqrt(Ni);
		blockValue = ceil(rtNi);
		reduceGraph(g,gnew,blockValue);
		lgNi = log10(rtNi) / log10(2.0);
		logNi = ceil(lgNi);
	
		STXXL_MSG("GNEW: Vertices "<<gnew.no_vertices<<" Edge: "<<gnew.no_edges<<" logNi:"<<logNi<<"Block Value: "<<blockValue);

		for(int j= 0; j< logNi;j++)
		{
			dag.createGraph(gnew);
			dag.detectCycle();
			
			//VertexContract vc(dag.no_edges);
			vc.contractVertices(dag);

			replaceEdge(gnew,vc.result);
			maintainList(list,vc.result);
			vc.result.clear();

			STXXL_MSG("GNEW: Vertices "<<gnew.no_vertices<<" Edge: "<<gnew.no_edges);
			if(gnew.no_edges == 0)
				break;
		}
		
		replaceEdge(g,list);
		prevNi= Ni;
		Ni= prevNi * sqrt(prevNi);
		if(g.no_vertices < (g.no_edges/B) || g.no_edges == 0)
			return;
		
	}

	
	
	

}

			
/*		
int main()
{
	int num_v,num_e;	

	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	std::cin>>num_v;
	STXXL_MSG("Edges :");
	std::cin>>num_e;
	
	Graph g(num_v,num_e);
	g.generate_Graph();
	g.print_Graph();
	
	
	
	float c,t;
	float B = (float) BLOCK_SIZE/(float)(sizeof(edge));
	t = log10((float) num_v*B / (float) num_e)/log10(2.0);
	c = log10(t) / log10(1.5);
	int count = ceil(c);
	float M = (20 * 1024 * 1024)/(float)(sizeof(edge) + sizeof(vertex));
	float N = num_v+num_e;
	STXXL_MSG("N: "<<N<<" M: "<<M<<" B: "<<B<<"Sizeof Edge: "<<sizeof(edge));

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;

	if(N > (M/2))
	{

		
		Timer.start();

		superphaseAlgo(g,count);
		STXXL_MSG("Elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(g.no_edges) / (Timer.mseconds() / 1000.)) << " edges per sec");

	        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
		
	
	
	}

	

	if(g.no_edges != 0)
	{
		stats_begin = *stxxl::stats::get_instance();
    		Timer.reset();
		Timer.start();
		external_prim mst(g.no_vertices,g.no_edges);		
		mst.build_mst(g);
		
		STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(g.no_edges) / (Timer.mseconds() / 1000.)) << " edges per sec");

      		  std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	}
	
	
	return 0;
}
*/
