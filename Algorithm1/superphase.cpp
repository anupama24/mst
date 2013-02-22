#include "config.h"
#include "graph/graph.cpp"
#include "mst.h"
#include "directed_graph/directedGraph.cpp"
#include "vertexContract.cpp"
#include "base_case/externalPrim.cpp"


/* reduceGraph
Create new graph G'=(V,E') from graph G.
E' contains the 'block value' lightest edges incident to each vertex
*/

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
	gnew.resizeList(newEnd,gnew.getFirstEdge());
	
	newItr=gnew.getFirstEdge();
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr));vItr++)
	{
			
		for( ;!(gnew.checkEdgeListEnd(newItr)) && newItr->getSrc()!= (vItr->first).getVertexId(); newItr++);

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
		
/* Maintain a list C of the form  (v, vs) for vertex
   The list C is updated after obtaining the output from the earlier step
*/		
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
				it2->blockingValue = it1->blockingValue;
				it2++;
			}
			it1++;
		}
		else
			it2++;
	}

	STXXL_MSG("List updated");
}

/* Remove duplicate edges */
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
			eItr->setSrc(std::numeric_limits<unsigned int>::max());
			eItr->setDst(std::numeric_limits<unsigned int>::max());
			eItr->setEdgeWt(std::numeric_limits<unsigned int>::max());
		}
	}

	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	
	edgeItr NewEnd = std::unique(g.getFirstEdge(),g.getEdgeListEnd());
	g.resizeList((NewEnd -1),g.getFirstEdge());
	
	//g.print_Graph();
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);

	eItr=g.getFirstEdge();
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)); vItr++)
	{
		for(; !(g.checkEdgeListEnd(eItr)) && eItr->getSrc()!=(vItr->first).getVertexId(); eItr++);
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
	unsigned int min,curr_v;
	typedef typename Graph::vertexItr vertexItr;
	
	vertexItr vItr,tempItr;
	
	Edge e;
	STXXL_MSG("Size of vertex main: "<<g.getVertexListSize());

	/*for(vItr=g.vmain.begin();vItr!= g.vmain.end();vItr++)
	{
		
		STXXL_MSG(" V: "<<(vItr->first).getVertexId()<<" & "<<(vItr->first).blocking_value);
			
	}*/	


	vItr = g.getFirstVertex();
	for(it1=list.begin();it1!=list.end();)
	{
		//STXXL_MSG("it1->second"<<(it1->second));
		for(; !(g.checkVertexListEnd(vItr)) && (vItr->first).getVertexId() != it1->second; vItr++);
		min = (vItr->first).getBlockingValue();
		curr_v = it1->second;

		while(it1!=list.end() && it1->second == curr_v)
		{
			if(min > it1->blockingValue)
			{
				min = it1->blockingValue;
			}
			it1++;
		}
		(vItr->first).setBlockingValue(min);
		

	}

	stxxl::sort(list.begin(),list.end(),myCmpFir(), INTERNAL_MEMORY_FOR_SORTING);
	tempItr = g.getFirstVertex();

	for(it1=list.begin();it1!=list.end();it1++)
	{
		for(; !(g.checkVertexListEnd(tempItr)) && (tempItr->first).getVertexId() != it1->first; tempItr++);
		(tempItr->first).setVertexId(std::numeric_limits<unsigned int>::max());
		(tempItr->first).setBlockingValue(std::numeric_limits<unsigned int>::max());
		tempItr->second = g.getEdgeListEnd();
	
	}
	
	stxxl::sort(g.getFirstVertex(),g.getVertexListEnd(),Graph::myCmpVer(g), INTERNAL_MEMORY_FOR_SORTING);
	
	vertexItr NewEnd = std::unique(g.getFirstVertex(),g.getVertexListEnd());
	g.resizeVertexList((NewEnd -1),g.getFirstVertex());
	g.setNoVertices(g.getVertexListSize());

	STXXL_MSG("Size of vertex main: "<<g.getVertexListSize());
	/*for(vItr=getFirstVertex();!(g.checkVertexListEnd(vItr));vItr++)
	{
		STXXL_MSG(" V: "<<(vItr->first).getVertexId()<<" & "<<(vItr->first).getBlockingValue());
			
	}*/
	STXXL_MSG("Blocking value set");

}

void replaceEdge(Graph &g,VertexContract::represVector &result)
{
	VertexContract::represVerItr it;
	
	typedef typename Graph::edgeItr edgeItr;
	typedef typename Graph::vertexItr vertexItr;
	
	edgeItr eItr;	
	vertexItr vItr;
	
	stxxl::sort(result.begin(),result.end(),myCmpFir(),INTERNAL_MEMORY_FOR_SORTING);
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);

	/*for(it=result.begin();it!=result.end();it++)
	{
		STXXL_MSG(" "<<it->first<<"-->"<<it->second);
	}
	*/

	it=result.begin();
	for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)) && it!=result.end();)
	{
		if(it->first==eItr->getSrc())
		{
			while(it!=result.end() && it->first==eItr->getSrc())
			{	eItr->setSrc(it->second);
				eItr++;
			}
			it++;
		}
		else
			eItr++;
			
	}

	

	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	//STXXL_MSG("Vertex Contraction");
	//g.print_Graph();
	
	it=result.begin();
	for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)) && it!=result.end();)
	{
		if(it->first==eItr->getDst())
		{
			while(it!=result.end() && it->first==eItr->getDst())
			{	eItr->setDst(it->second);
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
	


void superphaseAlgo(Graph &g,int count,MST &mst)
{
	VertexContract::represVector list,result;	
	Graph gnew;

	int logNi,blockValue;
	
	float Ni = 2,prevNi,rtNi,lgNi;
	float B = (float) BLOCK_SIZE/(float)(sizeof(Edge) + sizeof(Vertex));

	DirectedGraph dag(g.getNoVertices(),g.getNoEdges());
	dag.createGraph(g,mst);
	dag.detectCycle();
			
	VertexContract vc(dag.getNoEdges());
	vc.contractVertices(dag,result);
	replaceEdge(g,result);
	//g.print_Graph();
	//list = vc.result;

	STXXL_MSG("Vertices "<<g.getNoVertices()<<" Edge: "<<g.getNoEdges()<<" count: "<<count);

	for(int i=0;i<count;i++)
	{
		list.clear();
		rtNi = sqrt(Ni);
		blockValue = ceil(rtNi);
		reduceGraph(g,gnew,blockValue);
		lgNi = log10(rtNi) / log10(2.0);
		logNi = ceil(lgNi);
	
		STXXL_MSG("GNEW: Vertices "<<gnew.getNoVertices()<<" Edge: "<<gnew.getNoEdges()<<" logNi:"<<logNi<<"Block Value: "<<blockValue);

		for(int j= 0; j< logNi;j++)
		{
			dag.createGraph(gnew,mst);
			dag.detectCycle();
			
			//VertexContract vc(dag.getNoEdges());
			vc.contractVertices(dag,result);

			replaceEdge(gnew,result);
			maintainList(list,result);
			result.clear();

			STXXL_MSG("GNEW: Vertices "<<gnew.getNoVertices()<<" Edge: "<<gnew.getNoEdges());
			if(gnew.getNoEdges() == 0)
				break;
		}
		
		replaceEdge(g,list);
		prevNi= Ni;
		Ni= prevNi * sqrt(prevNi);
		if(g.getNoVertices() < (g.getNoEdges()/B) || g.getNoEdges() == 0)
			return;
		
	}

	
	
	

}

			
	
int main()
{
	int num_v,num_e;	

	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	std::cin>>num_v;
	STXXL_MSG("Edges :");
	std::cin>>num_e;
	
	Graph g(num_v,num_e);
	g.generateGraph();
	g.printGraph();
	
	MST mst(g.getNoVertices());
	mst.clearMST();

	float c,t;
	float B = (float) BLOCK_SIZE/(float)(sizeof(Edge));
	t = log10((float) num_v*B / (float) num_e)/log10(2.0);
	c = log10(t) / log10(1.5);
	int count = ceil(c);
	float M = (20 * 1024 * 1024)/(float)(sizeof(Edge));
	float N = num_v+num_e;
	STXXL_MSG("N: "<<N<<" M: "<<M<<" B: "<<B<<"Sizeof Edge: "<<sizeof(Edge));

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;

	if(N > (M/2))
	{

		
		Timer.start();

		superphaseAlgo(g,count,mst);
		STXXL_MSG("Elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(g.getNoEdges()) / (Timer.mseconds() / 1000.)) << " edges per sec");

	        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
		
	
	
	}

	

	if(g.getNoEdges() != 0)
	{
		stats_begin = *stxxl::stats::get_instance();
    		Timer.reset();
		Timer.start();
		ExternalPrim prim;
		prim.buildMST(g,mst);
	
		
		STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<" seconds : " << (double(g.getNoEdges()) / (Timer.mseconds() / 1000.)) << " edges per sec");

		std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
		mst.printMST();	
	}
	
	return 0;
}

