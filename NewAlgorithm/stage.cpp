void phaseSteps(Graph &inputGraph,Buckets &bucket,StarGraph &star,MST &mst,int starInBucket[],unsigned int i,int phaseCount,unsigned int limit)
{
	Graph::edgeType edgeSetS,graphH,Xi;
	StarGraph oldStar;

	int fi = findTrailingZeros(i);

	//createForest(bucket,graphH,fi,starInBucket);
	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
	hook(inputGraph,bucket,graphH);
	stxxl::stats_data stats_end(*stxxl::stats::get_instance());

	stats_total = stats_total + stats_end - stats_begin;

	STXXL_MSG("List size: "<<graphH.size());

	if(graphH.size()!=0)
	{
		stats_begin = *stxxl::stats::get_instance();
		createStarGraph(star,inputGraph,graphH,mst);
		int revFi = bucket.size()-fi-1;
		bucket.createStarGraph(oldStar,starInBucket,revFi);
		mergeStars(star,oldStar);

		stats_end = *stxxl::stats::get_instance();
		stats_total = stats_total + stats_end - stats_begin;


		if((int) i != phaseCount-1)
		{
			stats_begin = *stxxl::stats::get_instance();

			cleanUp(bucket,star,inputGraph,fi);
			findRi(bucket,star,inputGraph,fi);	
			if(inputGraph.getNoVertices()<= limit ||inputGraph.getNoVertices()<= 0)
				return;	
			createXi(bucket,inputGraph,Xi,fi);
			bucket.constructBucketsFromXi(Xi,inputGraph,fi);
			stats_end = *stxxl::stats::get_instance();
			stats_total = stats_total + stats_end - stats_begin;


		}
		
		bucket.cleanUpBucket(star,fi);
		//bucket.printBuckets();
		

		for(int j= bucket.size()-1; j>revFi; j--)
			starInBucket[j]=0;
		starInBucket[revFi] = 1;
		/*bucket.saveStarGraph(star,starInBucket,fi);*/
	}
}


void stage(Graph &inputGraph,MST &mst)
{

	StarGraph star;
	int alpha,count,gj,phaseCount=0,i,j;
	float temp,c=0;
	
	
	alpha = ceil(inputGraph.getNoEdges()/(float) inputGraph.getNoVertices());

	float B = (float) BLOCK_SIZE/(float)(sizeof(Edge));
	temp = log10(B)/log10(alpha);
	temp = log10(temp) / log10(2);
	count = ceil(temp);

	c = log10(alpha)/log10(2);
	c = log10(c)/log10(2);
	gj = ceil(c) + 2;

	Buckets bucket(inputGraph,gj);

	unsigned int upperLimit = ceil(inputGraph.getNoEdges()/ B);

	STXXL_MSG("Count of stages: "<<count<<" "<<" "<<inputGraph.getNoVertices()<<temp<<" "<<alpha<<" Limit: "<<upperLimit);


	for(j=0;j<count;j++)
	{
		c = pow(alpha,pow(2,j));
		c = log10(c)/log10(2);
		c = log10(c)/log10(2);
		gj = ceil(c) + 2;
		
		if(j==0)
		{
			bucket.initBuckets(gj);
			bucket.constructFirstBucket();
		}
		else
			bucket.initBuckets(gj);

		bucket.constructBuckets();
		inputGraph.clearEdgeList();
		//bucket.printBuckets();

		int *starInBucket=new int[gj];

		temp = log10(alpha)/log10(2);
		temp = pow(2,j) * temp;
		phaseCount = ceil(temp);
		STXXL_MSG("Count of phases: "<<phaseCount);
		for(i=1;i< phaseCount; i++)
		{
			star.clear();
			phaseSteps(inputGraph,bucket,star,mst,starInBucket,i,phaseCount,upperLimit);
			if(inputGraph.getNoVertices()<= upperLimit ||inputGraph.getNoVertices()<= 0 ||inputGraph.getNoVertices()== star.size())
				break;
		}
		if(phaseCount!=1)
		{
			stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
			bucket.createStarGraph(star,starInBucket,0);
			//star.print();
			cleanUp(bucket,star,inputGraph,gj-1);
			stxxl::stats_data stats_end(*stxxl::stats::get_instance());
			stats_total = stats_total + stats_end - stats_begin;

			updateVi(bucket,star,inputGraph);
			STXXL_MSG("End clean bucket "<<bucket.bucketSize(0));	
			//createNewEdgeList(bucket,inputGraph,gj-1);
			

		}
		
		
		bucket.cleanUpBucket();
		if(inputGraph.getNoVertices()== star.size() || bucket.bucketSize(0) == 0 || inputGraph.getNoVertices() <= upperLimit || inputGraph.getNoVertices() <= 0 )
		{
			STXXL_MSG("Inside if: "<<bucket.bucketSize(0)<<" "<<inputGraph.getNoVertices());	
			if(bucket.bucketSize(0)  != 0 && inputGraph.getNoVertices() != 0)
				createNewEdgeList(bucket,inputGraph,0);
			delete[] starInBucket;
		 	return;
		}
		delete[] starInBucket;
		
	}

	if(bucket.bucketSize(0)  != 0 && inputGraph.getNoVertices() != 0)
	{
		STXXL_MSG("Inside if: "<<bucket.bucketSize(0)<<" "<<inputGraph.getNoVertices());	
		createNewEdgeList(bucket,inputGraph,0);
	 	
	}
	
}
