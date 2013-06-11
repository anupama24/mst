#include "graph.h"

unsigned int Graph::getNoVertices()
{
	return noVertices;
}
void Graph::setNoVertices(unsigned int numV)
{
	noVertices = numV;
}

unsigned int Graph::getNoEdges()
{
	return noEdges;
}
void Graph::setNoEdges(unsigned int numE)
{
	noEdges = numE;
}

void Graph::addEdge(Edge &temp)
{
	edgeList.push_back(temp);
}

void Graph::addEdge(const Edge &temp)
{
	edgeList.push_back(temp);
}
void Graph::addVertex(vertexType &v)
{
	vertexList.push_back(v);
}

void Graph::swapEdge()
{
	edgeList.back().swap();
}


Graph::vertexItr Graph::getFirstVertex()
{

	vertexItr v_itr = vertexList.begin();
	return v_itr;
}

inline Graph::edgeItr Graph::getFirstEdge()
{

	edgeItr e_itr = edgeList.begin();
	return e_itr;
}

inline Graph::edgeItr Graph::getEdgeListEnd()
{

	edgeItr e_itr = edgeList.end();
	return e_itr;
}

inline Graph::vertexItr Graph::getVertexListEnd()
{
	return vertexList.end();
}

inline void Graph::resizeList(Graph::edgeItr end,Graph::edgeItr begin)
{
	edgeList.resize(end - begin);
}

inline void Graph::resizeVertexList(Graph::vertexItr end,Graph::vertexItr begin)
{
	vertexList.resize(end - begin);
}

	
void Graph::generateEdges()
{
	unsigned int i;
	
	edgeItr itr;
	
	edgeList.clear();
	Edge *e;
	//std::generate(edgeList.begin(),edgeList.end(),random_edge(noVertices,1e3));
	//srand ( time(NULL) );

	for(i=0;i<noVertices;i++)
	{	
		do
		{	e = new Edge(i+1,randomNodeID(),randomEdgeWeight());
			while(e->getDst() == e->getSrc()) 
				e->setDst(randomNodeID());
			itr = stxxl::find(edgeList.begin(), edgeList.end(), *e, 4);
			delete e;
					
		}
		while(itr!=edgeList.end());
		edgeList.push_back(*e);
		edgeList.push_back(*e);
		edgeList.back().swap();

		
	}
		
		
	for(i=0;i<(noEdges-noVertices);i++)
	{
		Edge tempEdge(i+1,randomNodeID(),randomEdgeWeight());
		while(tempEdge.getDst() == tempEdge.getSrc()) 
			tempEdge.setDst(randomNodeID());
		
		edgeList.push_back(tempEdge);
		edgeList.push_back(tempEdge);
		edgeList.back().swap();
				
	}

	
	
	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	Graph::edgeItr eItr,NewEnd;
	NewEnd = edgeList.begin() ;

	for(eItr=edgeList.begin()+1; eItr!= edgeList.end(); eItr++)
	{

		if(!(*NewEnd == *eItr))
		{
			//STXXL_MSG("Edge: "<<NewEnd->getSrc()<<" "<<NewEnd->getDst()<<" "<<NewEnd->getEdgeWt());
			NewEnd++;
			*NewEnd = *eItr;
		}
	}

	NewEnd++;
	//edgeItr NewEnd = std::unique(edgeList.begin(),edgeList.end());
	edgeList.resize(NewEnd - edgeList.begin());
	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	

	noEdges = edgeList.size()/2;	
	STXXL_MSG("Edge vector created of size: "<<edgeList.size());


}

void Graph::generateVertexList()
{
	Vertex *v;
	vertexType result;	
	vertexList.clear();
	edgeItr itr;

	itr=edgeList.begin();
	for(unsigned int i=0;i<noVertices;i++)
	{
		v = new Vertex(i+1);
		result.first = *v;
		for(;itr!=edgeList.end()&&itr->getSrc()!=v->getVertexId();itr++);
		
		result.second= itr;
		vertexList.push_back(result);	
		delete v;

			
	}
	
	
	
}


void Graph::generateGraph()
{
	generateEdges();
	generateVertexList();
}

void Graph::printGraph()
{
	edgeItr itr;
	
	STXXL_MSG("**Graph -  Edge Listing **"<<std::endl);
	STXXL_MSG("Vertices: " << noVertices);
	STXXL_MSG("Edges: " << noEdges);
	
	for(itr=edgeList.begin();itr!=edgeList.end();itr++)
	{
		STXXL_MSG(" (" <<(itr->getSrc())<<", " <<(itr->getDst())<<", "<<(itr->getEdgeWt())<<") ");
		
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
	g.generateGraph();
	g.printGraph();
	return 1;
}
*/

