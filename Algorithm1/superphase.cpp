/** reduceGraph
Create new graph gnew=(V,E') from graph g.
E' contains the 'blockValue' lightest edges 
incident to each vertex.
Also the blockingValue of each vertex is set
to the weight of the 'blockValue' + 1 th edge
if exists else set to std::numeric_limits<unsigned int>::max() **/

void reduceGraph(Graph &g,Graph &gnew,int blockValue)
{
	STXXL_MSG("Begin reduceGraph");

	Graph::edgeItr eItr,newItr,newEnd;
	Graph::vertexItr vItr;
	Graph::vertexType result;	
	int i=0;
	
	for(vItr = g.getFirstVertex(),eItr= g.getFirstEdge(),i=0; (!g.checkEdgeListEnd(eItr)) && !(g.checkVertexListEnd(vItr)); vItr++)
	{
		i = 0;
		while((!g.checkEdgeListEnd(eItr)) && eItr->getSrc()==(vItr->first).getVertexId() && i< blockValue)
		{	 
			gnew.addEdge(*eItr);
			gnew.addEdge(*eItr);
			gnew.swapEdge();
			i++;
			eItr++;
		}
		if((!g.checkEdgeListEnd(eItr)) && eItr->getSrc()== (vItr->first).getVertexId())
			vItr->first.setBlockingValue(eItr->getEdgeWt());
		else
			vItr->first.setBlockingValue(std::numeric_limits<unsigned int>::max());

		while((!g.checkEdgeListEnd(eItr)) && eItr->getSrc()== (vItr->first).getVertexId())
			eItr++;
		
	}

	stxxl::sort(gnew.getFirstEdge(),gnew.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	newEnd = std::unique(gnew.getFirstEdge(),gnew.getEdgeListEnd());
	gnew.resizeList(newEnd,gnew.getFirstEdge());
	
	//Generate vertex list for gnew
	newItr=gnew.getFirstEdge();
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr));vItr++)
	{
			
		for( ;!(gnew.checkEdgeListEnd(newItr)) && newItr->getSrc()!= (vItr->first).getVertexId(); newItr++);
		result.first = vItr->first;
		result.second = newItr;
		gnew.addVertex(result);


	}

	gnew.setNoEdges(gnew.getEdgeListSize()/2);
	gnew.setNoVertices(gnew.getVertexListSize());
	STXXL_MSG("End reduceGraph");
	
}	
/** Maintain a list L of the form  (v, vs) for each vertex
    where, vs is the current supervertex containing vertex v
    The list L is updated after obtaining the output from 
    the earlier step which is the result vector C **/		
void maintainList(VertexContract::represVector &L,VertexContract::represVector &C)
{
	STXXL_MSG("Begin maintainList");
	
	VertexContract::represVerItr it1,it2;
	
	if(L.empty())
	{
		L = C;
		return;
	}
	if(C.empty())
		return;

	/** Sort C by its first component and the list L by the second component **/
	stxxl::sort(C.begin(),C.end(),myCmpFir(), INTERNAL_MEMORY_FOR_SORTING);
	stxxl::sort(L.begin(),L.end(),myCmpSec(), INTERNAL_MEMORY_FOR_SORTING);


	/** Concurrently read both C and L and for each (vs, vr) in C replace (v, vs) in L with (v,vr) **/
	it1=C.begin();
	for(it2=L.begin();it1!=C.end();)
	{
		if(it2!=L.end()&& it1->first==it2->second)
		{
			while(it1!=C.end() && it1->first==it2->second)
			{	it2->second = it1->second;
				it2->blockingValue = it1->blockingValue;
				it2++;
			}
			it1++;
		}
		else if(it1!=C.end()&& it1->first<it2->second)
		{	
			it1++;
		}
		else
			it2++;

	}
	//Add the new (v, vs) to L if not present
	for(it1=C.begin();it1!=C.end();it1++)
	{
		L.push_back(*it1);
	}
	
	STXXL_MSG("End maintainList");

	
}

/* Remove duplicate and multiple edges */
void cleanEdges(Graph &g)
{

	STXXL_MSG("Begin cleanEdges");
	
	Graph::edgeItr eItr,NewEnd;
	Graph::vertexItr vItr;

	//Remove edges of type (u,u)
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
	
	//Iterate over edge list and retain the lightest copy of the edge among the multiple copies of that edge
	NewEnd = g.getFirstEdge() ;
	for(eItr=g.getFirstEdge()+1; !(g.checkEdgeListEnd(eItr)); eItr++)
	{
		if(eItr->getSrc() == std::numeric_limits<unsigned int>::max() && eItr->getDst() == std::numeric_limits<unsigned int>::max() && eItr->getEdgeWt() == std::numeric_limits<unsigned int>::max())
		{
			NewEnd++;
			break;
		}
		if(!(*NewEnd == *eItr))
		{
			NewEnd++;
			*NewEnd = *eItr;
			
		}
	}
	if(eItr->getSrc() != std::numeric_limits<unsigned int>::max() || eItr->getDst() != std::numeric_limits<unsigned int>::max() || eItr->getEdgeWt()!= std::numeric_limits<unsigned int>::max())
		NewEnd++;
	
	g.resizeList(NewEnd,g.getFirstEdge());
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);

	//Update the vertexList	
	eItr=g.getFirstEdge();
	for(vItr = g.getFirstVertex(); !(g.checkVertexListEnd(vItr)); vItr++)
	{
		for(; !(g.checkEdgeListEnd(eItr)) && eItr->getSrc()!=(vItr->first).getVertexId(); eItr++);
		vItr->second = eItr;

	}

	g.setNoEdges(g.getEdgeListSize()/2);
	g.setNoVertices(g.getVertexListSize());

	STXXL_MSG("Edges clean: "<<" V: "<<g.getNoVertices()<<" E: "<<g.getNoEdges());
	STXXL_MSG("End cleanEdges");

}

/** Finds the minimum blockingValue among the blockingValue of
all vertices that are contracted to form the supervertex **/	
void minBlockingValue(Graph &g,VertexContract::represVector &list)
{

	STXXL_MSG("Begin minBlockingValue");
	
	stxxl::sort(list.begin(),list.end(),myCmpSec(), INTERNAL_MEMORY_FOR_SORTING);
	
	VertexContract::represVerItr it1;
	unsigned int min,curr_v;
	Graph::vertexItr vItr,tempItr,result;
	Edge e;

	STXXL_MSG("Size of vertex main: "<<g.getVertexListSize()<<" List size: "<<list.size());
	
	for(vItr = g.getFirstVertex(),it1=list.begin();it1!=list.end();)
	{
		//Find the supervertex in the vertexList
		for(;!(g.checkVertexListEnd(vItr)) && (vItr->first).getVertexId() != it1->second; vItr++);

		//Initialize the min blockingValue to the blockingValue of the supervertex
		min = (vItr->first).getBlockingValue();
		curr_v = it1->second;
		
		//For all vertices that are contracted to form that supervertex find min blockingValue
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

	STXXL_MSG("Min blocking value set");

	stxxl::sort(list.begin(),list.end(),myCmpFir(), INTERNAL_MEMORY_FOR_SORTING);

	//Remove vertices that have been contracted from the vertexList	
	for(tempItr = g.getFirstVertex(),it1=list.begin(); it1!=list.end(); it1++)
	{
		for(; !(g.checkVertexListEnd(tempItr)) && (tempItr->first).getVertexId() != it1->first; tempItr++);
		(tempItr->first).setVertexId(std::numeric_limits<unsigned int>::max());
		
	}

	result = g.getFirstVertex();
	for(vItr=g.getFirstVertex();!(g.checkVertexListEnd(vItr));vItr++)
	{
		
		if((vItr->first).getVertexId() != std::numeric_limits<unsigned int>::max())
		{
			*result=*vItr;
			result++;
		}
		
	}
	
	vertexItr NewEnd = result;
	g.resizeVertexList(NewEnd,g.getFirstVertex());
	g.setNoVertices(g.getVertexListSize());

	STXXL_MSG("Size of vertex main: "<<g.getVertexListSize());
	STXXL_MSG("End minBlockingValue");
	
}

/** Replace each edge (u,v) in edgeList with (ur,vr) **/
void replaceEdge(Graph &g,VertexContract::represVector &result)
{

	STXXL_MSG("Begin replaceEdge");
	
	VertexContract::represVerItr it;
	
	Graph::edgeItr eItr;	
	
	/** Sort the result vector containing (v, vr) by the first component and 
	edgeList by the second component **/
	stxxl::sort(result.begin(),result.end(),myCmpFir(),INTERNAL_MEMORY_FOR_SORTING);
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	
	/** Scan both vectors simultaneously and replace (u,v) with (u,vr) **/
	it=result.begin();
	for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)) && it!=result.end();)
	{
		if(it->first==eItr->getDst())
		{
			while(!(g.checkEdgeListEnd(eItr)) && it!=result.end() && it->first==eItr->getDst())
			{	eItr->setDst(it->second);
				eItr++;
			}
			it++;
		}
		else
			eItr++;
	}
	
	/** Sort edgeList by the first component and repeat above process for (u,vr) with (ur,vr)**/
	stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	it=result.begin();
	for(eItr=g.getFirstEdge(); !(g.checkEdgeListEnd(eItr)) && it!=result.end();)
	{
		if(it->first==eItr->getSrc())
		{
			while(!(g.checkEdgeListEnd(eItr)) && it!=result.end() && it->first==eItr->getSrc())
			{	eItr->setSrc(it->second);
				eItr++;
			}
			it++;
		}
		else
			eItr++;
			
	}

	minBlockingValue(g,result);
	cleanEdges(g);	
	STXXL_MSG("End replaceEdge");
	
}
	

/** Performs the calculated number 'count' of superphases **/
void superphaseAlgo(Graph &g,int count,MST &mst,unsigned int limit)
{
	
	VertexContract::represVector list,result;	
	Graph gnew;

	int logNi,blockValue;
	float Ni = 2,prevNi,rtNi,lgNi;
	
	DirectedGraph dag(g.getNoVertices(),g.getNoEdges());

	/** To maintain invariant V(i+1) <= V/N(i+1) 
	call contraction method so that V0 <= V/2 **/
	dag.createGraph(g);
	dag.detectCycle(mst);
	VertexContract vc;
	vc.contractVertices(dag,result);
	replaceEdge(g,result);
	
	STXXL_MSG("Vertices "<<g.getNoVertices()<<" Edge: "<<g.getNoEdges()<<" count: "<<count);

	for(int i=0;i<count;i++)
	{
		list.clear();
		rtNi = sqrt(Ni);
		blockValue = ceil(rtNi);
		reduceGraph(g,gnew,blockValue);
		
		lgNi = log10(rtNi) / log10(2.0);
		logNi = ceil(lgNi);
	
		STXXL_MSG("GNEW: Vertices "<<gnew.getNoVertices()<<" Edge: "<<gnew.getNoEdges()<<" logNi:"<<logNi<<" Block Value: "<<blockValue);

		//logNi phases on reduced graph gnew
		for(int j= 0; j< logNi;j++)
		{
			dag.createGraph(gnew);
			dag.detectCycle(mst);
			vc.contractVertices(dag,result);
			if(result.size()==0)
				break;
			replaceEdge(gnew,result);
			maintainList(list,result);
			result.clear();
			STXXL_MSG("GNEW: Vertices "<<gnew.getNoVertices()<<" Edge: "<<gnew.getNoEdges());
			//Stop if no of vertices reduce to E/B
			if(gnew.getNoEdges() == 0 || gnew.getNoVertices() <= limit)
				break;
		}
		gnew.clearList();
		if(list.size()!=0)
			replaceEdge(g,list);
		list.clear();
		prevNi= Ni;
		Ni= prevNi * sqrt(prevNi);
		//Stop if no of vertices reduce to E/B
		if(g.getNoVertices() <= limit || g.getNoEdges() == 0)
		{	
			return;
		}
		
	}
}
