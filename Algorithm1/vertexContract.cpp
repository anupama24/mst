struct pqVertex
{
	Vertex v;
	unsigned int id;
	unsigned int edgeWt;
	unsigned int src,dst;
	pqVertex(){}
	pqVertex(Vertex v_,unsigned int id_,unsigned int wt_,unsigned int src_,unsigned int dst_):v(v_),id(id_),edgeWt(wt_),src(src_),dst(dst_){}
};

struct represVertex
{
	unsigned int first;
	unsigned int second;
	unsigned int  blockingValue;

	represVertex(){}
	represVertex(unsigned int fir_,unsigned int sec_,unsigned int value_):first(fir_),second(sec_),blockingValue(value_) {}

};
	

struct PQCmp 
{
     bool operator ()  (const pqVertex & a, const pqVertex & b) const
     {
	 return (a.edgeWt > b.edgeWt || (a.edgeWt == b.edgeWt && a.src > b.src) || (a.edgeWt == b.edgeWt && a.src == b.src && a.dst > b.dst));
     }

     pqVertex min_value() const
     {
	 return pqVertex(Vertex(0),0,std::numeric_limits <unsigned int >::max(),0,0);
     }
};

struct structList
{
	DirectedEdge dirEdge;
	unsigned int wt;
	unsigned int pos;

	structList(){}
	structList(DirectedEdge e,unsigned int wt_,unsigned int pos_):dirEdge(e),wt(wt_),pos(pos_){}

};

class VertexContract
{
public: 
	typedef typename DirectedGraph::dirEdgeType dirEdgeType;
	typedef typename DirectedGraph::dirEdgeItr edgeItr;
	

	typedef stxxl::VECTOR_GENERATOR<structList,VER_PAGE_SIZE,VER_NO_OF_PAGES,VER_BLOCK_SIZE>::result listType;
	
	typedef stxxl::VECTOR_GENERATOR<represVertex, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result represVector;
	typedef represVector::iterator represVerItr;


public:

	
	VertexContract(){}

	void contractVertices(DirectedGraph &dag,represVector &res);
	void createList(DirectedGraph::dirEdgeType &list,DirectedGraph &dag);
	bool checkEdges (const DirectedEdge &a, const DirectedEdge &b)
	{
		return (a.getSrc() == b.getSrc() && a.getDst() >= b.getDst()); //||(src == b.dst && dst == b.src));// && edge_wt == b.edge_wt);
	}
	
	//void replaceEdge(graph &g);

};

struct myCmpFir
{
  	represVertex min_value() const { 
		return represVertex(std::numeric_limits<unsigned int>::min(),0,0); };
	represVertex max_value() const { 
		return represVertex(std::numeric_limits<unsigned int>::max(),0,0); };
	bool operator () (const represVertex & a, const represVertex & b) const {
		return a.first < b.first || (a.first == b.first && a.second < b.second);
	}
};

struct myCmpSec
{
  	represVertex min_value() const { 
		return represVertex(std::numeric_limits<unsigned int>::min(),0,0); };
	represVertex max_value() const { 
		return represVertex(std::numeric_limits<unsigned int>::max(),0,0); };
	bool operator () (const represVertex & a, const represVertex & b) const {
		return a.second < b.second || (a.second == b.second && a.first < b.first);
	}
};


struct myCmpList
{

	structList min_value() const { 
		return structList(DirectedEdge(0,0,0),std::numeric_limits<unsigned int>::min(),0); };
	
  	structList max_value() const { 
		return structList(DirectedEdge(0,0,0),std::numeric_limits<unsigned int>::max(),0); };
	
	bool operator () (const structList & a, const structList & b) const {
		return (a.wt < b.wt || (a.wt == b.wt && a.pos < b.pos));
	}
};


void VertexContract::createList(DirectedGraph::dirEdgeType &list,DirectedGraph &dag)
{

	STXXL_MSG("Inside createList");

	
	//DirectedEdge *e;
	unsigned int i=0;
	
	dirEdgeType secCompList,firCompList;
	listType tempList;
	
	list.clear();
	secCompList.clear();firCompList.clear();
	tempList.clear();
	

	dag.copyEdgeList(secCompList);
	firCompList = secCompList;
	//dag.copyEdgeList(firCompList);

	//sort one instance of the list of edges by the second component
	stxxl::sort(secCompList.begin(),secCompList.end(),dirCmpDst(),INTERNAL_MEMORY_FOR_SORTING);	
	//sort another instance by the first component
	stxxl::sort(firCompList.begin(),firCompList.end(),dirCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);	

	edgeItr srcItr, dstItr;
	
	for(srcItr = firCompList.begin(), dstItr = secCompList.begin(),i=0 ; dstItr!= secCompList.end();dstItr++)
	{

		structList elem(*dstItr,dstItr->getEdgeWt(),i++);
		tempList.push_back(elem);

		while(srcItr->getSrc() < dstItr->getDst()&& srcItr != firCompList.end())
			srcItr++;
		while(srcItr->getSrc() == dstItr->getDst()&& srcItr != firCompList.end())
		{	//mergeList.push_back(*srcItr);
			structList elem(*srcItr,dstItr->getEdgeWt(),i++);
			tempList.push_back(elem);
			srcItr++;
		}
	}
	
	/*STXXL_MSG("Created merge list");
	for(e_itr=mergeList.begin();e_itr!=mergeList.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
		
	}*/


	secCompList.clear();firCompList.clear();

	
	//Sort list structure on edge weight
	stxxl::sort(tempList.begin(),tempList.end(),myCmpList(),INTERNAL_MEMORY_FOR_SORTING);	
	
			
	for(listType::iterator itr=tempList.begin(); itr!=tempList.end();itr++)
	{
		list.push_back(itr->dirEdge);
		

	}
			
	
	STXXL_MSG("List created "<<list.size());
	/*for(e_itr=list.begin();e_itr!=list.end();e_itr++)
	{
		STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
		
	}*/



	tempList.clear();
	
}
void VertexContract::contractVertices(DirectedGraph &dag,represVector &resultVector )
{

	typedef stxxl::PRIORITY_QUEUE_GENERATOR < pqVertex, PQCmp, 10 * 1024 * 1024, PQUEUE_MAX_SIZE/sizeof(pqVertex)> pqGen;	
	typedef pqGen::result pqType;
	typedef pqType::block_type blockType;
	
	stxxl::read_write_pool<blockType> pool((PQUEUE_MEM_POOL / 2) / blockType::raw_size, (PQUEUE_MEM_POOL / 2) / blockType::raw_size);
        pqType pqFwdProcess(pool);
	

	dirEdgeType list;

	createList(list,dag);
	edgeItr rootItr,e_itr,temp;
	represVertex newRepres;
	Vertex *v; 

	resultVector.clear();

	
	for(rootItr=dag.getFirstRoot(),temp= dag.getFirstEdge(); !(dag.checkRootsEnd(rootItr)); rootItr++)
	{
		while(!(dag.checkEdgeListEnd(temp)) && temp->getSrc() != rootItr->getSrc())
			temp++;
		for(;!(dag.checkEdgeListEnd(temp)) && temp->getSrc() == rootItr->getSrc(); temp++)
			{	
				v = new Vertex(temp->getDst());
				//STXXL_MSG("PQ push:"<<v->getVertexId()<<" "<<rootItr->getSrc()<<" "<<temp->getEdgeWt());
				pqVertex pqElem(*v,rootItr->getSrc(),temp->getEdgeWt(),temp->getSrc(),temp->getDst());
				pqFwdProcess.push(pqElem);
				delete v;
			}
		
	}
	
	for(e_itr=list.begin();e_itr!=list.end()&&!pqFwdProcess.empty();)
	{
		assert(!pqFwdProcess.empty());
		
		pqVertex pqElem=pqFwdProcess.top();
		pqFwdProcess.pop();
		//STXXL_MSG("PQ pop:"<<(pqElem.v).getVertexId()<<" "<<pqElem.id<<" ");

		newRepres.first=(pqElem.v).getVertexId();
		newRepres.second=pqElem.id;	
		newRepres.blockingValue = (pqElem.v).getBlockingValue();
		resultVector.push_back(newRepres);

		for(temp=e_itr+1;temp!=list.end() && temp->getSrc() == e_itr->getDst() && !(checkEdges(*(temp-1),*temp));temp++)
		{
			v = new Vertex(temp->getDst());
			//STXXL_MSG("PQ push:"<<v->getVertexId()<<" "<<pqElem.id<<" "<<temp->getEdgeWt());
			pqVertex pqElem(*v,pqElem.id,temp->getEdgeWt(),temp->getSrc(),temp->getDst());
			pqFwdProcess.push(pqElem);
			delete v;
		}
		e_itr=temp;
		//STXXL_MSG(" (" <<(e_itr->getSrc())<<", " <<(e_itr->getDst())<<", "<<(e_itr->getEdgeWt())<<") ");
	
	}
	//STXXL_MSG("Size: "<<resultVector.size()<<"PQ: "<<pqFwdProcess.size());

	while(!pqFwdProcess.empty())
	{
		pqVertex pqElem=pqFwdProcess.top();
		pqFwdProcess.pop();
	
		newRepres.first=(pqElem.v).getVertexId();
		newRepres.second=pqElem.id;
		newRepres.blockingValue = (pqElem.v).getBlockingValue();
		resultVector.push_back(newRepres);
	}
			

	//dag.roots.clear();
	list.clear();
	dag.clearList();
	//pqFwdProcess.~priority_queue();
	STXXL_MSG("Vertices contracted "<<resultVector.size());
	

	/*for(represVerItr itr = resultVector.begin(); itr!=resultVector.end();itr++)
	{	
		
		STXXL_MSG(" "<<itr->first<<" --> " <<itr->second);
	}*/


	
	//exit(0);
	
	
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

	VertexContract vc(dag.getNoEdges());

	stats_begin = *stxxl::stats::get_instance();
    	Timer.reset();
	Timer.start();

	VertexContract::represVector res;

	vc.contractVertices(dag,res);
	
	STXXL_MSG("Vertex Contraction elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(num_v) / (Timer.mseconds() / 1000.)) << " edges per sec");

        std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
	return 0;
}
*/
