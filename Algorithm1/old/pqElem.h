#ifndef PQELEM_H
#define PQELEM_H
#endif

struct pqNode
{
	
	unsigned int srcId;
	unsigned int dstId;
	unsigned int rank;
	
	pqNode(){}
	pqNode(unsigned int src_,unsigned int dst_,unsigned int rank_):srcId(src_),dstId(dst_),rank(rank_){}
};

struct pqNodeCmp 
{
     bool operator ()  (const pqNode & a, const pqNode & b) const
     {
	 return (a.srcId < b.srcId || a.srcId == b.srcId && a.dstId < b.dstId);
     }

     pqNode min_value() const
     {
	 return pqNode(std::numeric_limits <unsigned int >::min(),0,0,0);
     }
};

struct pqRef
{
	unsigned int nodeId;
	unsigned int refNodeId;

	pqRef() {}
	pqRef(unsigned int id_,unsigned int ref_): nodeId(id_), refNodeId(ref_) {}

};

struct pqRefCmp 
{
     bool operator ()  (const pqRef & a, const pqRef & b) const
     {
	 return a.refNodeId > b.refNodeId;
     }

     pqRef min_value() const
     {
	 return pqRef(0,std::numeric_limits <unsigned int >::max());
     }
};

struct pqDist
{
	unsigned int nodeId;
	unsigned int refDist;

	pqDist() {}
	pqDist(unsigned int id_,unsigned int dist_): nodeId(id_), refDist(dist_){}

};

struct pqDistCmp 
{
     bool operator ()  (const pqDist & a, const pqDist & b) const
     {
	 return a.nodeId > b.nodeId;
     }

     pqDist min_value() const
     {
	 return pqDist(std::numeric_limits <unsigned int >::max(),0);
     }
};

