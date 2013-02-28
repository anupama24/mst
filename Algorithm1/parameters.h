#ifndef PARAMETERS_H
#define PARAMETERS_H



/**
   Parses the command-line arguments and provides methods to get the arguments
   in a convenient way.
*/
class Parameters
{
 public:
    Parameters( int argc, char *argv[] )
    {
	_inputSpecified = false;
	_noOfNodes = std::numeric_limits<unsigned int>::max();
	_noOfEdges = std::numeric_limits<unsigned int>::max();
	
	if (argc < 2) {
	    std::cerr << "Too few arguments !" << std::endl << std::endl;
	    usage(-1);
	}
	int index = 1;
	while (index < argc) {
	    if ( ! strcmp(argv[index], "-h") ) {
		usage(0);
	    }
	    
	    if ( ! strcmp(argv[index], "-r") ) {
		if ( _inputSpecified ) {
		    std::cerr << "Argument '-r' contradicts another argument !"<< std::endl << std::endl;
		    usage(-1);
		}
		if ( argc - (++index) < 2 ) {
		    std::cerr << "-r requires two additional arguments !"<< std::endl << std::endl;
		    usage(-1);
		}
		_noOfNodes = atof(argv[index++]);
		_noOfEdges = atof(argv[index++]);
		_inputSpecified = true;
		_randomGraph = true;
		continue;
	    }
	    
	    
	    if ( ! strcmp(argv[index], "-i") ) {
		if ( _inputSpecified ) {
		    std::cerr << "Argument '-i' contradicts another argument !"
			      << std::endl << std::endl;
		    usage(-1);
		}
		if ( argc - (++index) < 1 ) {
		    std::cerr << "-i requires one additional argument !"
			      << std::endl << std::endl;
		    usage(-1);
		}
		_importInputFilename = argv[index++];
		_noOfNodes = noOfNodesInFile(_importInputFilename,_noOfEdges);
		_inputSpecified = true;
		continue;
	    }
	    
	    
	    if ( ! strcmp(argv[index], "-o") ) {
		if ( argc - (++index) < 1 ) {
		    std::cerr << "-o requires one additional argument !"
			      << std::endl << std::endl;
		    usage(-1);
		}
		_outputFilename = argv[index++];
		continue;
	    }

	       
	    std::cerr << "Argument '" << argv[index] << "' unknown -> ignored." << std::endl;
	    index++;
	}
	
	if ( ! _inputSpecified ) {
	    std::cerr << "One of the arguments '-h', '-r', '-i', '-o' "<< "is required !"<< std::endl << std::endl;
	    usage(-1);
	}
    }

    bool randomGraph() const {return _randomGraph;}
    unsigned int noOfNodes() const {return _noOfNodes;}
    unsigned int noOfEdges() const {return _noOfEdges;}
    
    std::string importInputFilename() const {return _importInputFilename;}
    std::string outputFilename() const {return _outputFilename;}
    	
 private:
    bool _inputSpecified;
    bool _randomGraph;
    unsigned int _noOfNodes;
    unsigned int _noOfEdges;

    std::string _importInputFilename;
    std::string _outputFilename;
   
    void usage(int exitCode) const {
	std::cout << "usage: mst -h|(-r #nodes #edges)|"
		  << "(-i filenameImportInput) "
		  << std::endl <<"[-o filenameMST]"
		  << std::endl << std::endl
		  << "\t -h   display this help and exit" << std::endl
		  << "\t -r   generate random graph with the given numbers of nodes and edges" << std::endl
		  << std::endl
		  << "\t -i   specify file in order to import the input graph"
		  << std::endl << std::endl
		  << "\t -o   specify output file for the resulting mst"
		  << std::endl << std::endl;
	exit( exitCode );
    }
};

#endif // PARAMETERS_H
