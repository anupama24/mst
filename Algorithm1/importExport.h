#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H


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
void importEdgeVector(std::istream &in,graph &g)
{
    int noOfNodes,noOfEdges;
    edge *Edge;

    STXXL_MSG("Reading graph from istream");
    in >> noOfNodes >> noOfEdges;

    g.edge_list.clear();
    for (int i=0; i<noOfEdges; i++) {
	int source,target;
	int weight;
	
	in >> source >> target >> weight;
	
	Edge = new edge(i+1,randomNodeID(),randomEdgeWeight());
	g.edge_list.push_back(*Edge);
	g.edge_list.push_back(*Edge);
	g.edge_list.back().swap();
    }
    
    stxxl::sort(g.edge_list.begin(),g.edge_list.end(),cmp_edge_wt(),INTERNAL_MEMORY_FOR_SORTING);
    edge_itr NewEnd = std::unique(g.edge_list.begin(),g.edge_list.end());
    g.edge_list.resize(NewEnd - g.edge_list.begin());
	
    STXXL_MSG(" Edge list: "<<g.edge_list.size());    
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

   sourceX and targetX are node indizes; the first used node index should be 0.
   Each 32-bit number (sourceX, targetX, weightX) should be represented by 4 chars
   and there should be no spaces and newlines after the number of edges.
   If necessary, several files are used so that the file size limit isn't exceeded.
*/
void importEdgeVectorCompressed(const std::string &filename,graph &g)
{
    static const int edgesPerFile = 150000000;
    int edgesInCurrentFile = 0;
    char currentFileSuffix = '1';
    
    std::ifstream in(filename.c_str());
    
    int noOfNodes,noOfEdges;

    STXXL_MSG("Import started.");
    
   
    in >> noOfNodes >> noOfEdges;
    
    g.edge_list.clear();
    
    for (int i=0; i<noOfEdges; i++) {
	if (edgesInCurrentFile == edgesPerFile) {
	    STXXL_MSG("File " << currentFileSuffix << " completed. Altogether "<< i << " edges read. ");
	    edgesInCurrentFile = 0;
	    currentFileSuffix++;
	    in.close();
	    in.open((filename + "." + currentFileSuffix).c_str());
	}
	
	in >> source >> target >> weight;
	
	Edge = new edge(i+1,randomNodeID(),randomEdgeWeight());
	g.edge_list.push_back(*Edge);
	g.edge_list.push_back(*Edge);
	g.edge_list.back().swap();

	edgesInCurrentFile++;
    }
    STXXL_MSG("File "<<currentFileSuffix<<" completed. Altogether "<< noOfEdges << " edges read. ");

    stxxl::sort(g.edge_list.begin(),g.edge_list.end(),cmp_edge_wt(),INTERNAL_MEMORY_FOR_SORTING);
    edge_itr NewEnd = std::unique(g.edge_list.begin(),g.edge_list.end());
    g.edge_list.resize(NewEnd - g.edge_list.begin());
	
    STXXL_MSG(" Edge list size: "<<g.edge_list.size()); 
   
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
void exportEdgeVector(std::ostream &out,graph &g)
{
    
    out <<g.noOfVertices()<<std::endl<<g.noOfEdges();

    typedef typename graph::edge_itr edge_itr;
 
    edge_itr itr;
	
    for(itr=edge_list.begin();itr!=edge_list.end();itr++)
    { 
	out <<(itr->src)<<"\t" <<(itr->dst)<<"\t"<<(itr->edge_wt)<<endl;
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
*/
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


/**
   Returns the number of nodes of the graph which is stored in the file
   with the given name.
*/
int noOfNodesInFile(const std::string &filename,int noOfEdges)
{
    std::ifstream inFile(filename.c_str());
    if ((inFile.peek() == 'a') || (inFile.peek() == 'c')) {
	char dummy;
	inFile >> dummy;
    }
    NodeCount noOfNodes;
    inFile >> noOfNodes;
    inFile >> noOfEdges;
    return noOfNodes;
}