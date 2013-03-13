#include "config.h"
#include "graph/graph.cpp"
#include "mst.h"
#include "directed_graph/directedGraph.cpp"
#include "vertexContract.cpp"
#include "base_case/externalPrim.cpp"
#include "importExport.h"
#include "parameters.h"
#include "superphase.cpp"



/** The main program. */
int main(int argc, char *argv[])
{

	Parameters param(argc, argv); // parse the command-line arguments
	
	if(param.noOfNodes() == std::numeric_limits<unsigned int>::max() || param.noOfEdges() ==std::numeric_limits<unsigned int>::max())
	{
		STXXL_MSG("Either number of vertices or edges is not speicifed. Exiting!!!");
		return 0;
	}
	Graph inputGraph(param.noOfNodes(),param.noOfEdges());

	if (param.randomGraph())
	{
		STXXL_MSG("Generating random graph");
		inputGraph.generateGraph();
		inputGraph.printGraph();

	}	
	else if (param.importInputFilename() != "") 
	{
		// import graph from file
		STXXL_MSG("Import graph" << std::endl );
		importEdgeVector( param.importInputFilename(),inputGraph );
	}

	// export input graph
	if (param.outputFilename() != "") 
	{
		STXXL_MSG("Export graph" << std::endl );	
		std::ofstream outFile(param.outputFilename().c_str());
		exportEdgeVector(outFile, inputGraph);

	}

	MST mst(inputGraph.getNoVertices());
	mst.clearMST();

	
	float c,t;
	float B = (float) BLOCK_SIZE/(float)(sizeof(Edge));
	t = log10((float) param.noOfNodes()*B / (float) param.noOfEdges())/log10(2.0);
	c = log10(t) / log10(1.5);
	int count = ceil(c);
	unsigned int upperLimit = ceil(param.noOfEdges()/ B);


	float M = (20 * 1024 * 1024)/(float)(sizeof(Edge));
	float N = param.noOfNodes()+2*param.noOfEdges();
	STXXL_MSG("N: "<<N<<" M: "<<M<<" B: "<<B<<" Sizeof Edge: "<<sizeof(Edge));

	stxxl::stats_data stats_begin(*stxxl::stats::get_instance());

    	stxxl::timer Timer;

	if(N > M)
	{
		
		Timer.start();
		superphaseAlgo(inputGraph,count,mst,upperLimit);
		STXXL_MSG("Elapsed time: " << (Timer.mseconds() / 1000.) <<
              " seconds : " << (double(inputGraph.getNoEdges()) / (Timer.mseconds() / 1000.)) << " edges per sec");
	        //std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin; 

	}

	STXXL_MSG("MST size: "<<mst.getMSTSize()<<" G V: "<<inputGraph.getNoVertices());	

	if(inputGraph.getNoEdges() != 0)
	{
		//stats_begin = *stxxl::stats::get_instance();
		stxxl::stats_data stats_begin(*stxxl::stats::get_instance());
    		Timer.reset();
		Timer.start();
		ExternalPrim prim;
		prim.buildMST(inputGraph,mst);
	
		
		STXXL_MSG("MST build elapsed time: " << (Timer.mseconds() / 1000.) <<" seconds : " << (double(inputGraph.getNoEdges()) / (Timer.mseconds() / 1000.)) << " edges per sec");

		std::cout << stxxl::stats_data(*stxxl::stats::get_instance()) - stats_begin;
		
	}
	
	mst.printMST();	
	
	return 0;
}

