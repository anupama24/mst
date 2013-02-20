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

Graph::edgeItr Graph::getFirstEdge()
{

	edgeItr e_itr = edgeList.begin();
	return e_itr;
}

Graph::edgeItr Graph::getEdgeListEnd()
{

	edgeItr e_itr = edgeList.end();
	return e_itr;
}

void Graph::resizeList(Graph::edgeItr newEnd)
{
	edgeList.resize(newEnd - edgeList.begin());
}
	
void Graph::generateEdges()
{
	unsigned int i;
	Edge *tempEdge;
	edgeItr itr;
	
	edgeList.clear();
	
	//std::generate(edgeList.begin(),edgeList.end(),random_edge(noVertices,1e3));
	//srand ( time(NULL) );

	for(i=0;i<noVertices;i++)
	{	
		do
		{	tempEdge = new Edge(i+1,randomNodeID(),randomEdgeWeight());
			while(tempEdge->getDst() == tempEdge->getSrc()) 
				tempEdge->setDst(randomNodeID());
			itr = stxxl::find(edgeList.begin(), edgeList.end(), *tempEdge, 4);
					
		}
		while(itr!=edgeList.end());
		edgeList.push_back(*tempEdge);
		edgeList.push_back(*tempEdge);
		edgeList.back().swap();
		
	}
		
		
	for(i=0;i<(noEdges-noVertices);i++)
	{
		do
		{	tempEdge = new Edge(randomNodeID(),randomNodeID(),randomEdgeWeight());
			while(tempEdge->getDst() == tempEdge->getSrc()) 
				tempEdge->setDst(randomNodeID());
			itr = stxxl::find(edgeList.begin(), edgeList.end(), *tempEdge, 4);
					
		}
		while(itr!=edgeList.end());
		edgeList.push_back(*tempEdge);
		edgeList.push_back(*tempEdge);
		edgeList.back().swap();
				
	}

	
	stxxl::sort(edgeList.begin(),edgeList.end(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
	edgeItr NewEnd = std::unique(edgeList.begin(),edgeList.end());
	edgeList.resize(NewEnd - edgeList.begin());

	noEdges = edgeList.size()/2;	
	STXXL_MSG("Edge vector created of size: "<<edgeList.size());


}

void Graph::generateVertexList()
{
	Vertex *v;
	vertexType result;	
	vertexList.clear();
	edgeItr itr;

	for(unsigned int i=0;i<noVertices;i++)
	{
		v = new Vertex(i+1);
		result.first = *v;
		for(itr=edgeList.begin();itr!=edgeList.end()&&itr->getSrc()!=v->getVertexId();itr++);
		
		/*if(itr==edgeList.end())
		{
			edge *Edge = new edge(v->vertex_id,randomNodeID(),randomEdgeWeight());
			while(Edge->dst <= Edge->src) 
				Edge->dst = randomNodeID();
			edgeList.push_back(*Edge);
			edgeList.push_back(*Edge);
			edgeList.back().swap();
			stxxl::sort(edgeList.begin(),edgeList.end(),cmp_edge_wt(),150 * 1024*1024);
			itr = stxxl::find(edgeList.begin(), edgeList.end(), *Edge, 4);

		}*/
		
		result.second= itr;
		vertexList.push_back(result);

			
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

	/*vertex_itr v_itr;
	vertex_type result;
	edge e;
	STXXL_MSG("Size of vertex main: "<<vertexList.size());
	for(v_itr=vertexList.begin();v_itr!= vertexList.end();v_itr++)
	{
		result = *v_itr;
		e = *(result.second);
		STXXL_MSG(" V: "<<(v_itr->first).vertex_id<<" ("<<(e.src)<<", " <<(e.dst)<<", "<<(e.edge_wt)<<") ");
			
	}*/
}


/*
int main()
{
	int num_v,num_e,ed;	

	STXXL_MSG("Graph\n");
	STXXL_MSG("Num vertices");
	cin>>num_v;
	STXXL_MSG("Edges :");
	cin>>num_e;
	STXXL_MSG("Weight Range: ");
	cin>>ed;

	graph g(num_v,num_e,ed);

	
	g.generate_graph();
	g.print_graph();
	return 1;
}

*/
