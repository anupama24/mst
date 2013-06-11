#include "graph.h"

/*** Getters and Setters for noVertices and noEdges  **/
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

/** Adds edge to the end of the edgeList **/
void Graph::addEdge(Edge &temp)
{
	edgeList.push_back(temp);
}

/** Adds vertex to the end of the vertexList **/
void Graph::addVertex(vertexType &v)
{
	vertexList.push_back(v);
}

/** Swaps the edge stored at the end of the edgeList. Calls the swap function of the Edge class. **/
void Graph::swapEdge()
{
	edgeList.back().swap();
}

/** Return the iterator to the first element of the vertexList **/
Graph::vertexItr Graph::getFirstVertex()
{
	vertexItr v_itr = vertexList.begin();
	return v_itr;
}

/** Return the iterator to the first element of the edgeList **/
Graph::edgeItr Graph::getFirstEdge()
{
	edgeItr e_itr = edgeList.begin();
	return e_itr;
}

/** Return the iterator to the last element of the edgeList **/
Graph::edgeItr Graph::getEdgeListEnd()
{
	edgeItr e_itr = edgeList.end();
	return e_itr;
}

/** Return the iterator to the last element of the vertexList **/
Graph::vertexItr Graph::getVertexListEnd()
{
	return vertexList.end();
}

/** Resizes the edge list to contain end - begin element **/
inline void Graph::resizeList(Graph::edgeItr end,Graph::edgeItr begin)
{
	edgeList.resize(end - begin);
}

/** Resizes the vertex list to contain end - begin element **/
inline void Graph::resizeVertexList(Graph::vertexItr end,Graph::vertexItr begin)
{
	vertexList.resize(end - begin);
}

/** Generates the specified no of edges
Selects a source and destination node randomly using rand function **/	
void Graph::generateEdges()
{
	unsigned int i;	
	edgeItr eItr,NewEnd;	
	edgeList.clear();
	Edge *e;

	//Generate one edge for each vertex in the graph
	for(i=0;i<noVertices;i++)
	{	
		e = new Edge(i+1,randomNodeID(),randomEdgeWeight());
		//Re-generate until source and destination nodes are different
		while(e->getDst() == e->getSrc()) 
			e->setDst(randomNodeID());
		edgeList.push_back(*e);
		edgeList.push_back(*e);
		edgeList.back().swap();
		delete e;
	}
		
	//Generate the remaining no. of edges	
	for(i=0;i<(noEdges-noVertices);i++)
	{
		Edge tempEdge(randomNodeID(),randomNodeID(),randomEdgeWeight());
		while(tempEdge.getDst() == tempEdge.getSrc()) 
			tempEdge.setDst(randomNodeID());
		
		edgeList.push_back(tempEdge);
		edgeList.push_back(tempEdge);
		edgeList.back().swap();
				
	}

	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	
	//Remove duplicate edges
	NewEnd = edgeList.begin() ;
	for(eItr=edgeList.begin()+1; eItr!= edgeList.end(); eItr++)
	{
		if(!(*NewEnd == *eItr))
		{
			NewEnd++;
			*NewEnd = *eItr;
		}
	}

	NewEnd++;
	edgeList.resize(NewEnd - edgeList.begin());

	//Store edges sorted the key <Source vertex, Edge weight>
	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	
	noEdges = edgeList.size()/2;	
	STXXL_MSG("Edge vector created of size: "<<edgeList.size());
}

/** Create the vertexList for the vertices in the graph
vertexList consists of vertex and edge iterator pair.
Finds the first edge with each vertex as the source and 
stores an iterator to that edge and the vertex **/
void Graph::generateVertexList()
{
	Vertex *v;
	vertexType result;	
	edgeItr itr;

	vertexList.clear();
	itr=edgeList.begin();
	for(unsigned int i=0;i<noVertices;i++)
	{
		v = new Vertex(i+1);
		result.first = *v;

		//Find first edge in edgeList with v as the source
		for(;itr!=edgeList.end()&&itr->getSrc()!=v->getVertexId();itr++);
		
		result.second= itr;
		vertexList.push_back(result);
		delete v;
	}
}


/** Generates a random graph **/
void Graph::generateRandomGraph()
{
	generateEdges();
	generateVertexList();
}

/** Generates a complete graph **/
void Graph::generateCompleteGraph()
{

	unsigned int weight;	
	edgeItr eItr,NewEnd;

	noEdges = noVertices * (noVertices - 1) / 2;
	edgeList.clear();
	
	//Generate  noVertices * (noVertices - 1) / 2 number of edges with edge weights genrated randomly 
	for (unsigned int i=0; i<noVertices; i++)
	{
		for (unsigned int j=0; j<i; j++) 
		{
			weight = randomEdgeWeight();
			Edge tempEdge(i+1,j+1,weight);
			edgeList.push_back(tempEdge);
			edgeList.push_back(tempEdge);
			edgeList.back().swap();
		}
	}

	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
	NewEnd = edgeList.begin() ;
	for(eItr=edgeList.begin()+1; eItr!= edgeList.end(); eItr++)
	{
		if(!(NewEnd == eItr))
		{
			NewEnd++;
			*NewEnd = *eItr;
		}
	}

	NewEnd++;
	edgeList.resize(NewEnd - edgeList.begin());
	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	noEdges = edgeList.size()/2;	
	STXXL_MSG("Edge vector created of size: "<<edgeList.size());
	
	//Generate the vertex list
	generateVertexList();
}

/** Prints the edges of the graph **/
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
