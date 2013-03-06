#ifndef DIREDGE_H
#define DIREDGE_H
#endif 


class DirectedEdge 
{
	unsigned int src,dst;
	unsigned int origSrc,origDst;
	unsigned int edgeWt;
	char data[128 - 5 * sizeof(unsigned int)];
	
public:	
	DirectedEdge() {}
	DirectedEdge(unsigned int src_,unsigned int dst_,unsigned origSrc_,unsigned origDst_,unsigned int wt_): src(src_), dst(dst_),origSrc(origSrc_),origDst(origDst_),edgeWt(wt_) {} 

	DirectedEdge(unsigned int src_,unsigned int dst_,unsigned int wt_): src(src_), dst(dst_),origSrc(src_),origDst(dst_),edgeWt(wt_) {} 

	bool operator == (const DirectedEdge & b) const {
		return (src == b.src && dst == b.dst && edgeWt==b.edgeWt);
	}
	bool equals (const DirectedEdge & b) const {
		return (src == b.dst && dst == b.src && edgeWt==b.edgeWt);
	}	

	
	unsigned int getSrc() const;
	void setSrc(unsigned int src);
	unsigned int getDst() const;
	void setDst(unsigned int dst);
	unsigned int getEdgeWt() const;
	void setEdgeWt(unsigned int wt);

	unsigned int getOrigSrc() const
	{
		return origSrc;
	}
	unsigned int getOrigDst() const
	{
		return origDst;
	}

	
};

//Getters and Setters
unsigned int DirectedEdge::getSrc() const
{
	return src;
}
void DirectedEdge::setSrc(unsigned int src)
{
	this->src = src;
}

unsigned int DirectedEdge::getDst() const
{
	return dst;
}
void DirectedEdge::setDst(unsigned int dst)
{
	this->dst = dst;
}

unsigned int DirectedEdge::getEdgeWt() const
{
	return edgeWt;
}
void DirectedEdge::setEdgeWt(unsigned int wt)
{
	edgeWt = wt;
}

//Comparator Functions

struct dirCmpWt
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<unsigned int>::min()); }
	DirectedEdge max_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<unsigned int>::max()); }
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.getEdgeWt() < b.getEdgeWt() || (a.getEdgeWt()==b.getEdgeWt() && a.getSrc()< b.getSrc()) || (a.getEdgeWt()==b.getEdgeWt() && a.getSrc() == b.getSrc() && a.getDst() < b.getDst());
	}
};

struct dirCmpWtRev
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<unsigned int>::min()); }
	DirectedEdge max_value() const { 
		return DirectedEdge(0,0,std::numeric_limits<unsigned int>::max()); }
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.getEdgeWt() < b.getEdgeWt() || (a.getEdgeWt()==b.getEdgeWt() && a.getDst()< b.getDst()) || (a.getEdgeWt()==b.getEdgeWt() && a.getDst() == b.getDst() && a.getSrc() < b.getSrc());
	}
};

struct dirCmpEdge
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::min(),0,0); }
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::max(),0,0); }
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.getSrc() < b.getSrc() || (a.getSrc() == b.getSrc() && a.getEdgeWt() < b.getEdgeWt());
	}
};

struct dirCmpSrc
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::min(),0,0); }
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::max(),0,0); }
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.getSrc() < b.getSrc() || (a.getSrc() == b.getSrc() && a.getDst() < b.getDst());
	}
};

struct dirCmpDst
{
  	DirectedEdge min_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::min(),0,0); }
	DirectedEdge max_value() const { 
		return DirectedEdge(std::numeric_limits<unsigned int>::max(),0,0); }
	bool operator () (const DirectedEdge & a, const DirectedEdge & b) const {
		return a.getDst() < b.getDst() || (a.getDst() == b.getDst() && a.getSrc() < b.getSrc());
	}
};

