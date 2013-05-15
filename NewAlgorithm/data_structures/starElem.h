#ifndef STARELEM_H
#define STARELEM_H

struct starElem
{
	unsigned int pos;
	Edge starEdge;
	starElem(){}
	starElem(unsigned int pos_):pos(pos_){}
	starElem(unsigned int pos_,Edge *starEdge_):pos(pos_),starEdge(*starEdge_){}
};

struct starCmpPos
{
  	starElem min_value() const { 
		return starElem(std::numeric_limits<unsigned int>::min(),new Edge(0,0,0)); };
	starElem max_value() const { 
		return starElem(std::numeric_limits<unsigned int>::max(),new Edge(0,0,0)); };
	bool operator () (const starElem &a, const starElem &b) const {
		return a.pos < b.pos;
	}
};

struct starCmpSrc
{
  	starElem min_value() const { 
		return starElem(0,new Edge(std::numeric_limits<unsigned int>::min(),0,0)); };
	starElem max_value() const { 
		return starElem(0,new Edge(std::numeric_limits<unsigned int>::max(),0,0)); };
	bool operator () (const starElem &a, const starElem &b) const {
		return (a.starEdge.getSrc() < b.starEdge.getSrc() || (a.starEdge.getSrc() == b.starEdge.getSrc() && a.starEdge.getDst() < b.starEdge.getDst()) || (a.starEdge.getSrc() == b.starEdge.getSrc() && a.starEdge.getDst() == b.starEdge.getDst() && a.starEdge.getEdgeWt() < b.starEdge.getEdgeWt()));
	}
};

struct starCmpDst
{
  	starElem min_value() const { 
		return starElem(0,new Edge(0,std::numeric_limits<unsigned int>::min(),0)); };
	starElem max_value() const { 
		return starElem(0,new Edge(0,std::numeric_limits<unsigned int>::max(),0)); };
	bool operator () (const starElem &a, const starElem &b) const {
		return (a.starEdge.getDst() < b.starEdge.getDst() || (a.starEdge.getDst() == b.starEdge.getDst() && a.starEdge.getSrc() < b.starEdge.getSrc()) || (a.starEdge.getDst() == b.starEdge.getDst() && a.starEdge.getSrc() == b.starEdge.getSrc() && a.starEdge.getEdgeWt() < b.starEdge.getEdgeWt()));
	}
};

#endif
