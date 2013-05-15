void phaseSteps(Graph &inputGraph,Buckets* bucket,StarGraph &star,MST &mst,int starInBucket[],unsigned int i)
{
	Graph::edgeType edgeSetS,graphH,Xi;
	

	//hook(inputGraph,bucket,edgeSetS);
	int fi = findTrailingZeros(i);

	createForest(bucket,inputGraph,graphH,fi,starInBucket);
	inputGraph.clearEdgeList();
	STXXL_MSG("List size: "<<graphH.size());

	if(graphH.size()!=0)
	{

		createStarGraph(star,inputGraph,graphH,mst);
		
		cleanUp(bucket,star,inputGraph,fi);

		findRi(bucket,star,inputGraph,fi);

		createXi(bucket,inputGraph,Xi,fi);
		bucket->constructBucketsFromXi(Xi,inputGraph,fi);
		bucket->cleanUpBucket(star,fi);
		//bucket->printBuckets();
		int revFi = bucket->size()-fi-1;

		for(int j= bucket->size()-1; j>revFi; j--)
			starInBucket[j]=0;
		starInBucket[revFi] = 1;
		/*bucket->saveStarGraph(star,starInBucket,fi);*/
	}
}


void stage(Graph &inputGraph,MST &mst)
{

	StarGraph star;
	int alpha,count,gj,phaseCount=0,i,j;
	float temp,c;
	
	
	alpha = ceil(inputGraph.getNoEdges()/(float) inputGraph.getNoVertices());

	float B = (float) BLOCK_SIZE/(float)(sizeof(Edge));
	temp = log10(B)/log10(alpha);
	temp = log10(temp) / log10(2);
	count = ceil(temp);
	Buckets *bucket=NULL;

	unsigned int upperLimit = ceil(inputGraph.getNoEdges()/ B);

	STXXL_MSG("Count of stages: "<<count<<" "<<" "<<inputGraph.getNoVertices()<<temp<<" "<<alpha<<"Limit: "<<upperLimit);


	for(j=0;j<count;j++)
	{
		c = pow(alpha,pow(2,j));
		c = log10(c)/log10(2);
		c = log10(c)/log10(2);
		gj = ceil(c) + 2;
		
		if(j==0)
		{
			bucket = new Buckets(inputGraph,gj);
			bucket->initBuckets(gj);
			bucket->constructFirstBucket();
		}
		else
			bucket->initBuckets(gj);

		bucket->constructBuckets();
		//bucket->printBuckets();

		int *starInBucket=new int[gj];

		temp = log10(alpha)/log10(2);
		temp = pow(2,j) * temp;
		phaseCount = ceil(temp);
		STXXL_MSG("Count of phases: "<<phaseCount);
		for(i=1;i< phaseCount; i++)
		{
			star.clear();
			phaseSteps(inputGraph,bucket,star,mst,starInBucket,i);
			if(inputGraph.getNoVertices() <= 0 || inputGraph.getNoVertices() == star.size() || mst.getMSTSize() == (unsigned int) mst.getNoVertices()-1)
				break;
		}
		if(phaseCount!=1)
		{
			bucket->createStarGraph(star,starInBucket);
			//star.print();
			cleanUp(bucket,star,inputGraph,gj-1);
			STXXL_MSG("End clean bucket "<<bucket->bucketSize(0));	
			//createNewEdgeList(bucket,inputGraph,gj-1);
			

		}
		
		
		bucket->cleanUpBucket();
		if(inputGraph.getNoVertices() == star.size() || bucket->bucketSize(0) == 0 || inputGraph.getNoVertices() < upperLimit || inputGraph.getNoVertices() <= 0 )
		{
			STXXL_MSG("Inside if: "<<bucket->bucketSize(0)<<" "<<inputGraph.getNoVertices());	
			if(bucket->bucketSize(0)  != 0 && inputGraph.getNoVertices() != 0)
				createNewEdgeList(bucket,inputGraph,0);
		 	return;
		}
		
	}
}
