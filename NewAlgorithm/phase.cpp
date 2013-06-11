int findTrailingZeros(unsigned int no)
{

	STXXL_MSG("Begin findTrailingZeros");
	int c;  // output: c will count trailing zero bits,
		
	if (no)
	{
	  no = (no ^ (no - 1)) >> 1;  // Set the number's trailing 0s to 1s and zero rest
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

void hook(Graph &inputGraph,Buckets &bucket,Graph::edgeType &edgeSetS)
{
	STXXL_MSG("Begin hook");
	typedef Buckets::const_extIterator extIterator;
	extIterator itr;
	
	Graph::const_vertexItr vItr;
	unsigned int bucketId = bucket.size() -1;

	for(vItr= inputGraph.getFirstVertex(),itr = bucket.beginExternalBucket(bucketId); !(inputGraph.checkVertexListEnd(vItr)) && itr!= bucket.endExternalBucket(bucketId);vItr++)
	{
		
		while(itr!= bucket.endExternalBucket(bucketId) && itr->getSrc() < vItr->first.getVertexId())
			itr++;
		if(itr!= bucket.endExternalBucket(bucketId) && itr->getSrc()== vItr->first.getVertexId())
		{
			//STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
			if(itr->getDst() != std::numeric_limits<unsigned int>::max())
				edgeSetS.push_back(*itr);
			itr++;
		}
	}
	STXXL_MSG("End hook");

}

void createForest(Buckets &bucket,Graph::edgeType &graphH,int fi,int starInBucket[])
{
	STXXL_MSG("Begin createForest");

	unsigned int bucketId,revFi;
	int j;
	typedef Buckets::const_extIterator extIterator;
	extIterator itr;
	revFi = bucket.size()-fi-1;

	graphH.clear();

	for(bucketId = revFi+1;bucketId <  bucket.size()-1; bucketId++)
	{
		if(starInBucket[bucketId]!=1)
		{
			for(itr = bucket.beginExternalBucket(bucketId); itr!= bucket.endExternalBucket(bucketId);itr++)
			{
				//STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
				if(itr->getDst() != std::numeric_limits<unsigned int>::max())
					graphH.push_back(*itr);
			}
		}
	}


	//hook(inputGraph,bucket,graphH);

	
	
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
	dag.detectCycle();

	EulerList euler(dag.getNoEdges());
	euler.createList(dag);
	
	//euler.printList();
	
	ListRanking rank(dag,star,euler);
	rank.createList();
	rank.listRank();
	rank.updateStarEdges();
	//rank.printList();
	//star.print();


	dag.addEdgesMST(star,mst,inputGraph);
	
	STXXL_MSG("End createStarGraph "<<star.size());


}


void findRi(Buckets &bucket,StarGraph &star,Graph &inputGraph,int fi)
{

	STXXL_MSG("Begin findRi");
	//bucket.printBuckets();
	typedef Buckets::const_extIterator extIterator;
	extIterator itr;
	StarGraph::starItr starItr;
	
	Graph::vertexType newVertex;
	inputGraph.clearList();
	STXXL_MSG("Vertices: "<<inputGraph.getNoVertices()<<"Edges: "<<inputGraph.getNoEdges());	


	int revFi = bucket.size()-fi-1;

	stxxl::sort(star.begin(),star.end(),starCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	starItr = star.begin();

	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
	{
		while(starItr!=star.end() && starItr->starEdge.getDst() < itr->getSrc())
			starItr++;
		
		if(itr!= bucket.endExternalBucket(revFi) && starItr!=star.end() && starItr->starEdge.getDst() == itr->getSrc() && itr->getDst() == std::numeric_limits<unsigned int>::max())
		{
			Vertex v(starItr->starEdge.getDst(),itr->getEdgeWt());
			newVertex.first= v;
			newVertex.second = inputGraph.getEdgeListEnd();
			inputGraph.addVertex(newVertex);

	
		}
		
	}

	
	inputGraph.setNoVertices(inputGraph.getVertexListSize());
	
	stxxl::sort(bucket.beginExternalBucket(revFi),bucket.endExternalBucket(revFi),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	
	STXXL_MSG("End findRi "<<inputGraph.getNoVertices());	

}

void updateVi(Buckets &bucket,StarGraph &star,Graph &inputGraph)
{

	STXXL_MSG("Begin updateVi");
	
	StarGraph::const_starItr starItr;
	typedef Buckets::const_extIterator extIterator;
	extIterator itr,prev;
	Graph::vertexType newVertex;

	inputGraph.clearList();
	STXXL_MSG("Vertices: "<<inputGraph.getNoVertices()<<"Edges: "<<inputGraph.getNoEdges());	

	stxxl::sort(star.begin(),star.end(),starCmpDst(),INTERNAL_MEMORY_FOR_SORTING);
	prev = bucket.beginExternalBucket(0);
	starItr = star.begin();

	if(starItr->starEdge.getDst() == prev->getSrc())
	{
		Vertex v(starItr->starEdge.getDst());
		newVertex.first= v;
		newVertex.second = inputGraph.getEdgeListEnd();
		inputGraph.addVertex(newVertex);
	}

	

	for(itr = bucket.beginExternalBucket(0)+1; itr!= bucket.endExternalBucket(0);itr++)
	{
		while(starItr!=star.end() && starItr->starEdge.getDst() < itr->getSrc())
			starItr++;
		
		if(itr!= bucket.endExternalBucket(0) && starItr!=star.end() && starItr->starEdge.getDst() == itr->getSrc() && prev->getSrc() != itr->getSrc())
		{
			Vertex v(starItr->starEdge.getDst());
			newVertex.first= v;
			newVertex.second = inputGraph.getEdgeListEnd();
			inputGraph.addVertex(newVertex);
			prev = itr;	
		}
		
	}

	
	
	inputGraph.setNoVertices(inputGraph.getVertexListSize());
	
	STXXL_MSG("End updateVi "<<inputGraph.getNoVertices());	

}

void createXi(Buckets &bucket,Graph &inputGraph,Graph::edgeType &Xi,int fi)
{

	STXXL_MSG("Begin createXi");

	typedef Buckets::const_extIterator extIterator;
	extIterator itr;
	Graph::const_vertexItr vItr;
	int revFi = bucket.size()-fi-1;
	vItr=inputGraph.getFirstVertex();
	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
	{
			while(!(inputGraph.checkVertexListEnd(vItr)) && vItr->first.getVertexId() < itr->getSrc())
				vItr++;
			if(!(inputGraph.checkVertexListEnd(vItr)) && itr!= bucket.endExternalBucket(revFi) && vItr->first.getVertexId() == itr->getSrc() && itr->getEdgeWt() < vItr->first.getRi())
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



void removeDuplicateEdges(Buckets &bucket,Graph &inputGraph,int fi)
{
	STXXL_MSG("Begin removeDuplicateEdges");

	typedef Buckets::extIterator extIterator;
	extIterator itr,newEnd;

	/*Graph::vertexType newVertex;
	Vertex *v;
	if(fi!= bucket.size()-1)
		inputGraph.clearList();
	*/
	int revFi = bucket.size()-fi-1;
	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
	{
		if(itr->getSrc()==itr->getDst())
		{
			itr->setSrc(std::numeric_limits<unsigned int>::max());
			itr->setDst(std::numeric_limits<unsigned int>::max());
			itr->setEdgeWt(std::numeric_limits<unsigned int>::max());
		}		

	}
	stxxl::sort(bucket.beginExternalBucket(revFi),bucket.endExternalBucket(revFi),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	
	newEnd = bucket.beginExternalBucket(revFi);
	for(itr = bucket.beginExternalBucket(revFi)+1; itr!= bucket.endExternalBucket(revFi);itr++)
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
			
		}
	}

	if(itr->getSrc() != std::numeric_limits<unsigned int>::max() || itr->getDst() != std::numeric_limits<unsigned int>::max() || itr->getEdgeWt() != std::numeric_limits<unsigned int>::max())
		newEnd++;
	
	bucket.resizeBucket(newEnd,bucket.beginExternalBucket(revFi),revFi);

	
	
	/*
	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
	{
		STXXL_MSG(itr->getSrc()<<" "<<itr->getDst()<<" "<<itr->getEdgeWt());
	}
		*/
	STXXL_MSG("End removeDuplicateEdges "<<bucket.bucketSize(revFi));

}

void cleanUp(Buckets &bucket,StarGraph &star,Graph &inputGraph,int fi)
{
	STXXL_MSG("Begin cleanUp");

	typedef Buckets::extIterator extIterator;
	extIterator itr;
	StarGraph::const_starItr starItr;

	int revFi = bucket.size()-fi-1;
	starItr = star.begin();
	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
	{
		while(starItr!=star.end() && starItr->starEdge.getSrc() < itr->getSrc())
			starItr++;
		if(starItr!=star.end() && starItr->starEdge.getSrc() == itr->getSrc())
			itr->setSrc(starItr->starEdge.getDst());
	}

	
	STXXL_MSG("Begin cleanUp");

	stxxl::sort(bucket.beginExternalBucket(revFi),bucket.endExternalBucket(revFi),myCmpDst(),INTERNAL_MEMORY_FOR_SORTING);

	STXXL_MSG("Begin cleanUp");

	starItr = star.begin();
	for(itr = bucket.beginExternalBucket(revFi); itr!= bucket.endExternalBucket(revFi);itr++)
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
	//bucket.printBuckets();
}

void createNewEdgeList(Buckets &bucket,Graph &inputGraph,unsigned int bucketId)
{
	STXXL_MSG("Begin createNewEdgeList");

	typedef Buckets::const_extIterator extIterator;
	extIterator itr;
	for(itr = bucket.beginExternalBucket(bucketId); itr!= bucket.endExternalBucket(bucketId);itr++)
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

void mergeStars(StarGraph &star,StarGraph &oldStar)
{
	
	STXXL_MSG("Begin mergeStars ");

	StarGraph::starItr itr,final,result;


	stxxl::sort(oldStar.begin(),oldStar.end(),starCmpDst(),INTERNAL_MEMORY_FOR_SORTING);

	for(itr = oldStar.begin(),final=star.begin();itr!= oldStar.end();itr++)
	{
		while(final!=star.end() && final->starEdge.getSrc() < itr->starEdge.getDst())
			final++;
		if(final->starEdge.getSrc()==itr->starEdge.getDst())
		{
			itr->starEdge.setDst(final->starEdge.getDst());
		}
	}
	//stxxl::sort(beginExternalBucket(i),endExternalBucket(i),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	for(itr = oldStar.begin();itr!= oldStar.end();itr++)
	{
		star.add(*itr);
	}
	oldStar.clear();
	stxxl::sort(star.begin(),star.end(),starCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	result = star.begin();
	for(itr=star.begin()+1;itr!=star.end();itr++)
	{
		if(itr!=star.end() && !(result->starEdge == itr->starEdge))
		{
			result++;
			*result = *itr;
		}
	} 
	result++;
	star.resize(result,star.begin());

	STXXL_MSG("End mergeStars "<<star.size());
}
