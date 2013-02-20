#ifndef NODE_H
#define NODE_H
#endif

class Node
{
	unsigned int src,dst;
	unsigned int edgeWt;
	unsigned int succ;
	unsigned int root;

public:	
	Node() {
		succ= std::numeric_limits<unsigned int>::max();
		root= std::numeric_limits<unsigned int>::max();
	}
	Node(unsigned int src_,unsigned int dst_,unsigned int wt_): src(src_), dst(dst_),edgeWt(wt_) {
		succ= std::numeric_limits<unsigned int>::max();
		root= std::numeric_limits<unsigned int>::max();
	} 

	
	unsigned int getSrc() const;
	void setSrc(unsigned int src);
	unsigned int getDst() const;
	void setDst(unsigned int dst);
	unsigned int getEdgeWt() const;
	void setEdgeWt(unsigned int wt);
	unsigned int getSucc() const;
	void setSucc(unsigned int s);
	unsigned int getRoot() const;
	void setRoot(unsigned int rt);

		
};

//Getters and Setters
unsigned int Node::getSrc() const
{
	return src;
}
void Node::setSrc(unsigned int src)
{
	this->src = src;
}

unsigned int Node::getDst() const
{
	return dst;
}
void Node::setDst(unsigned int dst)
{
	this->dst = dst;
}

unsigned int Node::getEdgeWt() const
{
	return edgeWt;
}
void Node::setEdgeWt(unsigned int wt)
{
	edgeWt = wt;
}

unsigned int Node::getSucc() const
{
	return succ;
}
void Node::setSucc(unsigned int s)
{
	succ = s;
}

unsigned int Node::getRoot() const
{
	return root;
}
void Node::setRoot(unsigned int rt)
{
	root = rt;
}
