#ifndef EXTERNALPRIM_H
#define EXTERNALPRIM_H
#endif 


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

