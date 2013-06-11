#ifndef BUCKETS_H
#define BUCKETS_H



class Buckets
{


	private:
	/** The type of an external bucket which contains the edges of several nodes. */
	typedef stxxl::VECTOR_GENERATOR<Edge,PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result bucketType;
	
        CommonPool<bucketType> sharedBlock;



	public:

	typedef bucketType::iterator extIterator;
	typedef bucketType::const_iterator const_extIterator;

	/**
	@param graph a reference to an EdgeVector which represents the graph
	@param result a reference to a MST object which stores the result
	@param noOfBuckets the number of external buckets which should be used
	@param noOfNodesInIntMem the number of nodes which fit in internal memory
	*/
	Buckets(Graph &graph, unsigned int noOfBuckets): _graph( graph ),extBuckets( noOfBuckets )
	{
	   // initBuckets();
	    
	}

	~Buckets()
	{
		STXXL_MSG("Inside buckets destructor");
		for(unsigned int i=0;i<extBuckets.size();i++)
		{
			extBuckets[i]->clear();
			delete extBuckets[i];
		}
	}


	/** Adds the given edge to the bucket which is specified by bucketId. */
	void addToExternalBucket(const Edge &edge,unsigned int bucketId);

	/** Gets the edge in the specified bucketId and position */
	Edge getFromExternalBucket(unsigned int pos,unsigned int bucketId);

	/** Return iterator to the beginning of the specified bucketId */
	extIterator beginExternalBucket(unsigned int bucketId);

	/** Return iterator to the end of the specified bucketId */
	extIterator endExternalBucket(unsigned int bucketId);
	
	void initBuckets( unsigned int noOfBuckets);
	void constructBuckets();
	void constructBucketsFromXi(Graph::edgeType &Xi,Graph &inputGraph,int fi);
	void cleanUpBucket(StarGraph &star,unsigned int bucketId);
	void resizeBucket(extIterator end,extIterator begin,unsigned int bucketId);
	void createStarGraph(StarGraph &star,int bucketId[],int end);
	void saveStarGraph(int bucketId[],int fi);
	void printBuckets();
	unsigned int size()
	{
		return extBuckets.size();
	}
	int bucketSize(unsigned int bucketId);
	void constructFirstBucket();
	void cleanUpBucket();

	private:
	/** Reference to the given graph. */
	Graph &_graph;

	/**
	The external buckets.
	Each bucket contains the edges of several nodes.
	*/
	std::vector<bucketType*> extBuckets;

	/**
	Initializes the external buckets and distributes the edges to the
	external buckets.
	*/
	
	void cleanBucket(unsigned int bucketId);
	void flush(unsigned int bucketId);
	
	
	
		
	


};

/** Adds the given edge to the bucket which is specified by newBucketID. */
inline void Buckets::addToExternalBucket(const Edge &edge,unsigned int bucketId)
{
	extBuckets[bucketId]->push_back( edge );
}

/** Gets the edge in the specified bucketId and position */
inline Edge Buckets::getFromExternalBucket(unsigned int pos,unsigned int bucketId)
{
	return	(*extBuckets[bucketId])[pos];
}

inline Buckets::extIterator Buckets::beginExternalBucket(unsigned int bucketId)
{
	return extBuckets[bucketId]->begin();
}

inline Buckets::extIterator Buckets::endExternalBucket(unsigned int bucketId)
{
	return extBuckets[bucketId]->end();
}

inline void Buckets::cleanBucket(unsigned int bucketId)
{
	extBuckets[bucketId]->clear();
}
inline void Buckets::flush(unsigned int bucketId)
{
	extBuckets[bucketId]->flush();
}
inline void Buckets::resizeBucket(extIterator end,extIterator begin,unsigned int bucketId)
{
	extBuckets[bucketId]->resize(end-begin);
}
inline int Buckets::bucketSize(unsigned int bucketId)
{
	return extBuckets[bucketId]->size();
}
#endif // BUCKETS_H
