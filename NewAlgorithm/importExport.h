#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H
#endif 

/**
   Imports an EdgeVector from an istream.
   expected format:
   
   noOfNodes noOfEdges
   source0 target0 weight0
   source1 target1 weight1
   source2 target2 weight2
   ...

   sourceX and targetX are node indizes; the first used node index should be 0.
*/
void importEdgeVector(const std::string &filename,Graph &g)
{
    int noOfNodes,noOfEdges;
   

    std::ifstream in( filename.c_str() );
    in >> noOfNodes >> noOfEdges;
    STXXL_MSG("Reading graph from istream");
	
    g.clearList();
    g.setNoVertices(noOfNodes);
    for (int i=0; i< 2 * noOfEdges; i++) {
	unsigned int source,target,weight;
	
	in >> source >> target >> weight;
	
	Edge edge(source,target,weight);
	g.addEdge(edge);
	//g.addEdge(*edge);
	//g.swapEdge();
    }

    STXXL_MSG(" Edge list: "<<g.getEdgeListSize());     
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
    Graph::edgeItr eItr,NewEnd;
    NewEnd = g.getFirstEdge() ;

	for(eItr=g.getFirstEdge()+1; !(g.checkEdgeListEnd(eItr)); eItr++)
	{
		if(!(*NewEnd == *eItr))
		{
			//STXXL_MSG(NewEnd->getSrc()<<" "<<NewEnd->getDst()<<" "<<NewEnd->getEdgeWt());
			NewEnd++;
			*NewEnd = *eItr;
		}
	}

    NewEnd++;
    //std::unique(g.getFirstEdge(),g.getEdgeListEnd());
    g.resizeList(NewEnd,g.getFirstEdge());
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
   
    g.generateVertexList();
    g.setNoEdges(g.getEdgeListSize()/2);
    STXXL_MSG(" Edge list: "<<g.getEdgeListSize());    
}



/**
   Imports an EdgeVector from a file.
   expected format:

   'c'
   noOfNodes noOfEdges
   source0 target0 weight0
   source1 target1 weight1
   source2 target2 weight2
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
    g.setNoVertices(noOfNodes);
    for (int i=0; i< noOfEdges; i++) {
	unsigned int source,target,weight;
	
	in >> temp >> source >> target >> weight;
	
	edge =new Edge(source,target,weight);
	g.addEdge(*edge);
	delete edge;
	
    }

    STXXL_MSG(" Edge list: "<<g.getEdgeListSize());
     
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpSrc(),INTERNAL_MEMORY_FOR_SORTING);
    Graph::edgeItr eItr,NewEnd;
    NewEnd = g.getFirstEdge() ;

	for(eItr=g.getFirstEdge()+1; !(g.checkEdgeListEnd(eItr)); eItr++)
	{
		if(!(*NewEnd == *eItr))
		{
			//STXXL_MSG(NewEnd->getSrc()<<" "<<NewEnd->getDst()<<" "<<NewEnd->getEdgeWt());
			NewEnd++;
			*NewEnd = *eItr;
		}
	}

    NewEnd++;
    //std::unique(g.getFirstEdge(),g.getEdgeListEnd());
    g.resizeList(NewEnd,g.getFirstEdge());
    stxxl::sort(g.getFirstEdge(),g.getEdgeListEnd(),myCmpEdgeWt(),INTERNAL_MEMORY_FOR_SORTING);
   
    g.generateVertexList();
    g.setNoEdges(g.getEdgeListSize()/2);
    STXXL_MSG("Vertices "<<g.getNoVertices()<<" Edge: "<<g.getNoEdges());
}


/**
   Exports an EdgeVector to an ostream.
   created format:

   'a'
   noOfNodes
   adjacency list of node 1
   -1
   adjacency list of node 2
   -1
   ...

   Each adjacency list is a list of pairs: node1 weight1 node2 weight2 ...
   The first used node index is 1.
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
   Exports an EdgeVector to a file.
   created format:

   'c'
   noOfNodes noOfEdges
   source0 target0 weight0
   source1 target1 weight1
   source2 target2 weight2
   ...

   sourceX and targetX are node indizes; the first used node index is 0.
   Each 32-bit number (sourceX, targetX, weightX) is represented by 4 chars
   and there are no spaces and newlines after the number of edges.
   If necessary, several files are used so that the file size limit isn't exceeded.

void exportEdgeVectorCompressed(const std::string &filename, EdgeVector<> &edges)
{
    static const EdgeCount edgesPerFile = 150000000;
    EdgeCount edgesInCurrentFile = 0;
    char currentFileSuffix = 'a';
    
    std::ofstream out( filename.c_str() );

    DS_VERBOSE1( std::cout << "Export started. " << std::flush; system("date") );
    out << 'c' << std::endl << edges.noOfNodes() << " " << edges.noOfEdges() << std::endl;
    for (EdgeCount i=0; i<edges.noOfEdges(); i++) {
	if (edgesInCurrentFile == edgesPerFile) {
	    DS_VERBOSE1(
		std::cout << "File " << currentFileSuffix << " completed. Altogether "
                          << i << " edges written. " << std::flush;
		system("date");
	    )
	    edgesInCurrentFile = 0;
	    currentFileSuffix++;
	    out << std::endl;
	    out.close();
	    out.open( (filename + "." + currentFileSuffix).c_str() );
	}
	
	unsigned int dataToWrite[] = {edges[i].source(), edges[i].target(), edges[i].weight()};
	for (int j=0; j<3; j++) {
	    for (int k=0; k<4; k++) {
		char tmp = dataToWrite[j] % 256;
		dataToWrite[j] /= 256;
		out << tmp;
	    }
	}
	
	edgesInCurrentFile++;
    }
    DS_VERBOSE1(
	std::cout << "File " << currentFileSuffix << " completed. Altogether "
	          << edges.noOfEdges() << " edges written. " << std::flush;
	system("date");
    )
    out << std::endl;
    out.close();
}

*/
/**
   Returns the number of nodes of the graph which is stored in the file
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
