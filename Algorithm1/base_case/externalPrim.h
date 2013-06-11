#ifndef EXTERNALPRIM_H
#define EXTERNALPRIM_H
#endif 


/***********************************************************************
This class is for the base case i.e. Modified prim's algorithm 
It contains a comparator myMstCmp which is used by the external priority queue
Comparison is based on the edge weight
*************************************************************************/

class ExternalPrim
{
	
public:

	ExternalPrim() {}

	struct myMstCmp 
	{
	     Edge min_value() const
	     {
		 return Edge(0,0,std::numeric_limits <unsigned int >::max());
	     }
	
	      bool operator ()  (const Edge & a, const Edge & b) const
	     {
		 return a.getEdgeWt() > b.getEdgeWt();
	     }	     
	};

	void buildMST(Graph &g,MST &mst);	

};

