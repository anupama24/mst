#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H


/**
   Imports an edgeList from a file.
   expected format in file:
   
   noOfNodes 
   noOfEdges
   source0 target0 weight0
   source1 target1 weight1
   source2 target2 weight2
   ...

   sourceX and targetX are nodes;
   File should contain both edges (u,v) and (v,u)
**/
void importEdgeVector(const std::string &filename,Graph &g)
{
    int noOfNodes,noOfEdges;
    std::ifstream in( filename.c_str() );
    
    in >> noOfNodes >> noOfEdges;
    STXXL_MSG("Reading graph from istream");
    Edge *edge;	
    g.clearList();
    for (int i=0; i< 2 * noOfEdges; i++) 
    {
	unsigned int source,target,weight;
	in >> source >> target >> weight;
	edge =new Edge(source,target,weight);
	g.addEdge(*edge);
	delete edge;
	
    }

    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
    
    Graph::edgeItr eItr,NewEnd;
    NewEnd = g.getFirstEdge();

    //Remove the duplicate edges
    for(eItr=g.getFirstEdge()+1; !(g.checkEdgeListEnd(eItr)); eItr++)
    {
	if(!(*NewEnd == *eItr))
	{
		NewEnd++;
		*NewEnd = *eItr;
	}
    }

    NewEnd++;
    g.resizeList(NewEnd,g.getFirstEdge());
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
  
    g.generateVertexList();
    g.setNoEdges(g.getEdgeListSize()/2);
    STXXL_MSG(" Edge list: "<<g.getEdgeListSize());    

}



/**
   Imports an edgeList from a file. Used for files downloaded from the DIMACS 9th challenge
   expected format:

   'c' noOfNodes noOfEdges
   'c' source0 target0 weight0
   'c' source1 target1 weight1
   'c' source2 target2 weight2
   ...

 */

void importFromFile(const std::string &filename,Graph &g)
{
    int noOfNodes,noOfEdges;
    char temp;

    std::ifstream in( filename.c_str() );
    in >> temp >> noOfNodes >> noOfEdges;
    STXXL_MSG("Reading graph from file");
    
    Edge *edge;	
    g.clearList();
    for (int i=0; i< noOfEdges; i++) 
    {
	unsigned int source,target,weight;
	in >> temp >> source >> target >> weight;
	edge =new Edge(source,target,weight);
	g.addEdge(*edge);
	delete edge;
	
    }

   
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
    Graph::edgeItr eItr,NewEnd;
    NewEnd = g.getFirstEdge() ;

    for(eItr=g.getFirstEdge()+1; !(g.checkEdgeListEnd(eItr)); eItr++)
    {
	if(!(*NewEnd == *eItr))
	{
		NewEnd++;
		*NewEnd = *eItr;
	}
    }

    NewEnd++;
    g.resizeList(NewEnd,g.getFirstEdge());
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
   
    g.generateVertexList();
    g.setNoEdges(g.getEdgeListSize()/2);
    STXXL_MSG("Vertices "<<g.getNoVertices()<<" Edge: "<<g.getNoEdges());
}

/**
   Exports an edgeList to an ostream.
   created format:

   noOfNodes
   noOfEdges
   source0 target0 weight0
   source1 target1 weight1
   source2 target2 weight2
  
   sourceX and targetX are nodes;
   File contains both edges (u,v) and (v,u)
*/
void exportEdgeVector(std::ostream &out,Graph &g)
{
    
    out <<g.getNoVertices()<<std::endl<<g.getNoEdges()<<std::endl;
    Graph::edgeItr itr;
	
    for(itr=g.getFirstEdge(); !(g.checkEdgeListEnd(itr));itr++)
    { 
	out <<(itr->getSrc())<<"\t" <<(itr->getDst())<<"\t"<<(itr->getEdgeWt())<<std::endl;
    }
    out << std::endl << "-1" << std::endl;
    
}


/**
   Returns the number of nodes and stores the no of edges in 'noOfEdges' of the graph which is stored in the file
   with the given name.
*/
int noOfNodesInFile(const std::string &filename,unsigned int &noOfEdges)
{
    std::ifstream inFile(filename.c_str());
    if ((inFile.peek() == 'a') || (inFile.peek() == 'c')) {
	char dummy;
	inFile >> dummy;
    }
    unsigned int noOfNodes;
    inFile >> noOfNodes;
    inFile >> noOfEdges;
    return noOfNodes;
}
#endif 
