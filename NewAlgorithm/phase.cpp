int findTrailingZeros(unsigned int no)
{

	STXXL_MSG("Begin findTrailingZeros");
	int c;  // output: c will count trailing zero bits,
		
	if (no)
	{
	  no = (no ^ (no - 1)) >> 1;  // Set v's trailing 0s to 1s and zero rest
	  for (c = 0; no; c++)
	  {
	    no >>= 1;
	  }
	}
	else
	{
	  c = CHAR_BIT * sizeof(no);
	}
	c = c+1;
	STXXL_MSG("End findTrailingZeros "<<c);
	return c;
}

void hook(Graph &inputGraph,Buckets* bucket,Graph::edgeType &edgeSetS)
{
	STXXL_MSG("Begin hook");
	typedef Buckets::extIterator extIterator;
	extIterator itr;
	Graph::const_vertexItr vItr;
	unsigned int bucketId = bucket->size() -1;
	for(vItr= inputGraph.getFirstVertex(),itr = bucket->beginExternalBucket(bucketId); !(inputGraph.checkVertexListEnd(vItr)) && itr!= bucket->endExternalBucket(bucketId);vItr++)
	{
		
		while(itr->getSrc() < vItr->first.getVertexId())
			itr++;
		if(itr->getSrc()== vItr->first.getVertexId())
		{
			//STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
			if(itr->getDst() != std::numeric_limits<unsigned int>::max())
				edgeSetS.push_back(*itr);
			itr++;
		}
	}
	STXXL_MSG("End hook");

}

void createForest(Buckets* bucket,Graph &inputGraph,Graph::edgeType &graphH,int fi,int starInBucket[])
{
	STXXL_MSG("Begin createForest");

	unsigned int bucketId,revFi;
	int j;
	typedef Buckets::extIterator extIterator;
	extIterator itr;
	Graph::const_edgeItr eItr;
	revFi = bucket->size()-fi-1;

	graphH.clear();

	if( bucket->bucketSize(bucket->size() -1)==0)
	{
		for(j= bucket->size() -1; j >= 0 && starInBucket[j]!=1 ;j--);
		bucket->saveStarGraph(starInBucket,j);
	}


	for(bucketId = revFi+1;bucketId <  bucket->size()-1; bucketId++)
	{
		for(itr = bucket->beginExternalBucket(bucketId); itr!= bucket->endExternalBucket(bucketId);itr++)
		{
			//STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
			if(itr->getDst() != std::numeric_limits<unsigned int>::max())
				graphH.push_back(*itr);
		}
	}


	hook(inputGraph,bucket,graphH);

	
	
	/*STXXL_MSG("Print createForest");	
	for(eItr = graphH.begin();eItr!=graphH.end();eItr++)
		STXXL_MSG(" (" <<(eItr->getSrc())<<", " <<(eItr->getDst())<<", "<<(eItr->getEdgeWt())<<") ");
	*/
	STXXL_MSG("End createForest");
}

void createStarGraph(StarGraph &star,Graph &inputGraph,Graph::edgeType &graphH,MST &mst)
{
	STXXL_MSG("Begin createStarGraph");

	DirectedGraph dag(graphH.size());
	dag.createGraph(graphH);	
	dag.detectCycle(mst,inputGraph);

	EulerList euler(dag.getNoEdges());
	euler.createList(dag);
	
	//euler.printList();
	
	ListRanking rank(star,euler);
	rank.createList(dag);
	rank.listRank();
	//rank.updateStarEdges();
	//rank.printList();
	//star.print();


	dag.addEdgesMST(star,mst,inputGraph);
	
	STXXL_MSG("End createStarGraph "<<star.size());


}


void findRi(Buckets* bucket,StarGraph star,Graph &inputGraph,int fi)
{

	STXXL_MSG("Begin findRi");
	//bucket->printBuckets();
	typedef Buckets::extIterator extIterator;
	extIterator itr;
	StarGraph::starItr starItr;
	
	Graph::vertexType newVertex;
	Vertex *v;
	inputGraph.clearList();
	STXXL_MSG("Vertices: "<<inputGraph.getNoVertices()<<"Edges: "<<inputGraph.getNoEdges());	


	int revFi = bucket->size()-fi-1;

	stxxl::sort(star.begin(),star.end(),starCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	starItr = star.begin();

	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
		while(starItr!=star.end() && starItr->starEdge.getDst() < itr->getSrc())
			starItr++;
		
		if(itr!= bucket->endExternalBucket(revFi) && starItr!=star.end() && starItr->starEdge.getDst() == itr->getSrc() && itr->getDst() == std::numeric_limits<unsigned int>::max())
		{
			v = new Vertex(starItr->starEdge.getDst(),itr->getEdgeWt());
			newVertex.first= *v;
			newVertex.second = inputGraph.getEdgeListEnd();
			inputGraph.addVertex(newVertex);

	
		}
		
	}

	
	inputGraph.setNoVertices(inputGraph.getVertexListSize());
	
	stxxl::sort(bucket->beginExternalBucket(revFi),bucket->endExternalBucket(revFi),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	
	STXXL_MSG("End findRi "<<inputGraph.getNoVertices());	

}

void createXi(Buckets* bucket,Graph &inputGraph,Graph::edgeType &Xi,int fi)
{

	STXXL_MSG("Begin createXi");

	typedef Buckets::extIterator extIterator;
	extIterator itr;
	Graph::vertexItr vItr;
	int revFi = bucket->size()-fi-1;
	vItr=inputGraph.getFirstVertex();
	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
			//STXXL_MSG(itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
			while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < itr->getSrc())
				vItr++;
			if(!(inputGraph.checkVertexListEnd(vItr)) && itr!= bucket->endExternalBucket(revFi) && vItr->first.getVertexId() == itr->getSrc() && itr->getEdgeWt() < vItr->first.getRi())
			{
				if(itr->getDst() != std::numeric_limits<unsigned int>::max())
				{
					//STXXL_MSG(itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
					Xi.push_back(*itr);
				}

			}
		
	}
	STXXL_MSG("End createXi "<<Xi.size());
}

/*
void createXi(Buckets* bucket,Graph &inputGraph,Graph::edgeType &Xi,int fi)
{

	STXXL_MSG("Begin createXi");

	typedef Buckets::extIterator extIterator;
	extIterator itr;
	Graph::vertexItr vItr;
	int flag =0;

	vItr=inputGraph.getFirstVertex();
	for(itr = bucket->beginExternalBucket(fi); itr!= bucket->endExternalBucket(fi);itr++)
	{
			while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < itr->getSrc())
			{	vItr++;	
				flag =0;
			}
			if(!(inputGraph.checkVertexListEnd(vItr)) && itr!= bucket->endExternalBucket(fi) && vItr->first.getVertexId() == itr->getSrc() && flag ==0)
			{
				if(itr->getDst() != std::numeric_limits<unsigned int>::max())
				{
					//STXXL_MSG(itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
					Xi.push_back(*itr);
				}
				else if(itr->getDst() == std::numeric_limits<unsigned int>::max())
				{	flag =1;
					vItr->first.setRi(itr->getEdgeWt());
				}

			}
		
	}
	STXXL_MSG("End createXi");
}


*/

void removeDuplicateEdges(Buckets* bucket,Graph &inputGraph,int fi)
{
	STXXL_MSG("Begin removeDuplicateEdges");

	typedef Buckets::extIterator extIterator;
	extIterator itr,newEnd;

	/*Graph::vertexType newVertex;
	Vertex *v;
	if(fi!= bucket->size()-1)
		inputGraph.clearList();
	*/
	int revFi = bucket->size()-fi-1;
	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
		if(itr->getSrc()==itr->getDst())
		{
			itr->setSrc(std::numeric_limits<unsigned int>::max());
			itr->setDst(std::numeric_limits<unsigned int>::max());
			itr->setEdgeWt(std::numeric_limits<unsigned int>::max());
		}		

	}
	stxxl::sort(bucket->beginExternalBucket(revFi),bucket->endExternalBucket(revFi),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	
	newEnd = bucket->beginExternalBucket(revFi);
	/*if(newEnd->getDst() == std::numeric_limits<unsigned int>::max() && fi!= bucket->size()-1)
	{
		STXXL_MSG("V: "<<newEnd->getSrc()<<" Ri: "<<newEnd->getEdgeWt());
		v = new Vertex(newEnd->getSrc());
		newVertex.first= *v;
		newVertex.second = inputGraph.getEdgeListEnd();
		inputGraph.addVertex(newVertex);
	}*/

	for(itr = bucket->beginExternalBucket(revFi)+1; itr!= bucket->endExternalBucket(revFi);itr++)
	{

		if(itr->getSrc() == std::numeric_limits<unsigned int>::max() && itr->getDst() == std::numeric_limits<unsigned int>::max() && itr->getEdgeWt() == std::numeric_limits<unsigned int>::max())
		{
			//NewEnd = eItr;
			newEnd++;
			break;
		}
		if(!(*newEnd == *itr))
		{
			//STXXL_MSG(newEnd->getSrc()<<" "<<newEnd->getDst()<<" "<<newEnd->getEdgeWt());
			newEnd++;
			*newEnd = *itr;
			/*if(newEnd->getDst() == std::numeric_limits<unsigned int>::max() && fi!= bucket->size()-1)
			{
				STXXL_MSG("V: "<<newEnd->getSrc()<<" Ri: "<<newEnd->getEdgeWt());
				v = new Vertex(newEnd->getSrc());
				newVertex.first= *v;
				newVertex.second = inputGraph.getEdgeListEnd();
				inputGraph.addVertex(newVertex);
			}*/
		}
	}
	
	bucket->resizeBucket(newEnd,bucket->beginExternalBucket(revFi),revFi);

	
	
	/* if(fi!= bucket->size()-1)
		inputGraph.setNoVertices(inputGraph.getVertexListSize());
	
	
	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
		STXXL_MSG(itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
	}
		*/
	STXXL_MSG("End removeDuplicateEdges "<<bucket->bucketSize(revFi));

}

void cleanUp(Buckets* bucket,StarGraph &star,Graph &inputGraph,int fi)
{
	STXXL_MSG("Begin cleanUp");

	typedef Buckets::extIterator extIterator;
	extIterator itr;
	StarGraph::starItr starItr;

	int revFi = bucket->size()-fi-1;
	starItr = star.begin();
	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
		while(starItr!=star.end() && starItr->starEdge.getSrc() < itr->getSrc())
			starItr++;
		if(starItr!=star.end() && starItr->starEdge.getSrc() == itr->getSrc())
			itr->setSrc(starItr->starEdge.getDst());
	}

	stxxl::sort(bucket->beginExternalBucket(revFi),bucket->endExternalBucket(revFi),myCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	starItr = star.begin();
	for(itr = bucket->beginExternalBucket(revFi); itr!= bucket->endExternalBucket(revFi);itr++)
	{
		if(itr->getDst()!= std::numeric_limits<unsigned int>::max())
		{
			while(starItr!=star.end() && starItr->starEdge.getSrc() < itr->getDst())
				starItr++;
			if(starItr!=star.end() && starItr->starEdge.getSrc() == itr->getDst())
				itr->setDst(starItr->starEdge.getDst());
		}
	}

	
	STXXL_MSG("End cleanUp ");
	removeDuplicateEdges(bucket,inputGraph,fi);
	//bucket->printBuckets();
}

void createNewEdgeList(Buckets *bucket,Graph &inputGraph,unsigned int bucketId)
{
	STXXL_MSG("Begin createNewEdgeList");

	typedef Buckets::extIterator extIterator;
	extIterator itr;
	for(itr = bucket->beginExternalBucket(bucketId); itr!= bucket->endExternalBucket(bucketId);itr++)
	{
		if(itr->getDst() != std::numeric_limits<unsigned int>::max())
			inputGraph.addEdge(*itr);
	}

	Graph::vertexItr vItr;
	Graph::edgeItr eItr;
	eItr = inputGraph.getFirstEdge();
	for(vItr=inputGraph.getFirstVertex(); !(inputGraph.checkVertexListEnd(vItr)); vItr++)
	{
		for( ; !(inputGraph.checkEdgeListEnd(eItr)) && eItr->getSrc()!=vItr->first.getVertexId();eItr++);
		vItr->second = eItr;
	}
	inputGraph.setNoEdges(inputGraph.getEdgeListSize()/2);
	//inputGraph.printGraph();

	STXXL_MSG("End createNewEdgeList "<<inputGraph.getNoEdges());	

}
