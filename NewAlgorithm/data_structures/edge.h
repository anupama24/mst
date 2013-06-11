/*	Edge class 
Data members: Source vertex, Destination vertex,Edge weight, Original Source and Destination Vertex
Size: 128 bytes
Methids: Getters and Setters, Swap, Operator == and equals */


#ifndef EDGE_H
#define EDGE_H
#endif 

class Edge 
{
	unsigned int src,dst;
	unsigned int origSrc,origDst;
	unsigned int edgeWt;
	char data[128 - 5 * sizeof(unsigned int)];
public:
	Edge() {}
	Edge(unsigned int src_,unsigned int dst_,unsigned int wt_): src(src_), dst(dst_),origSrc(src_),origDst(dst_),edgeWt(wt_) {} 
	


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
	void setOrigDst(unsigned int dst) 
	{
		origDst = dst;
	}

	bool operator == (const Edge & b) const {
		return (src == b.src && dst == b.dst); //||(src == b.dst && dst == b.src));// && edge_wt == b.edge_wt);
	}

	bool equals (const Edge & b) const {
		return ((src == b.src && dst == b.dst)||(src == b.dst && dst == b.src));// && edge_wt == b.edge_wt);
	}

	void swap()
	{
		int temp = src;
		src = dst;
		dst = temp;
		origSrc =src;
		origDst =dst;
	}
	
};


unsigned int Edge::getSrc() const
{
	return src;
}
void Edge::setSrc(unsigned int src)
{
	this->src = src;
}

unsigned int Edge::getDst() const
{
	return dst;
}
void Edge::setDst(unsigned int dst)
{
	this->dst = dst;
}

unsigned int Edge::getEdgeWt() const
{
	return edgeWt;
}
void Edge::setEdgeWt(unsigned int wt)
{
	edgeWt = wt;
}

//Comparator Functions

struct myCmpSrc
{
  	Edge min_value() const { 
		return Edge(std::numeric_limits<unsigned int>::min(),0,0); };
	Edge max_value() const { 
		return Edge(std::numeric_limits<unsigned int>::max(),0,0); };
	bool operator () (const Edge &a, const Edge &b) const {
		return a.getSrc() < b.getSrc() || (a.getSrc() == b.getSrc() && a.getDst() < b.getDst()) || (a.getSrc() == b.getSrc() && a.getDst() == b.getDst() && a.getEdgeWt() < b.getEdgeWt());
	}
};


struct myCmpDst
{
  	Edge min_value() const { 
		return Edge(0,std::numeric_limits<unsigned int>::min(),0); };
	Edge max_value() const { 
		return Edge(0,std::numeric_limits<unsigned int>::max(),0); };
	bool operator () (const Edge & a, const Edge & b) const {
		return a.getDst() < b.getDst() || (a.getDst() == b.getDst() && a.getSrc() < b.getSrc()) || (a.getDst() == b.getDst() && a.getSrc() == b.getSrc() && a.getEdgeWt() < b.getEdgeWt());
	}
};

struct myCmpEdgeWt
{
  	Edge min_value() const { 
		return Edge(std::numeric_limits<unsigned int>::min(),0,0); };
	Edge max_value() const { 
		return Edge(std::numeric_limits<unsigned int>::max(),0,0); };
	bool operator () (const Edge & a, const Edge & b) const {
		return a.getSrc() < b.getSrc() || (a.getSrc() == b.getSrc() && a.getEdgeWt() < b.getEdgeWt()) || (a.getSrc() == b.getSrc() && a.getEdgeWt() == b.getEdgeWt() && a.getDst() < b.getDst());
	}
};


