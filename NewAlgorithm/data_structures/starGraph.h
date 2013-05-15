#ifndef STARGRAPH_H
#define STARGRAPH_H

#include "starElem.h"

class StarGraph
{
	
	typedef stxxl::VECTOR_GENERATOR<starElem, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result starType;
	starType starGraph;	
	
public:
	StarGraph(){}

	typedef starType::iterator starItr;
	
	void add(starElem elem);

	inline starItr begin()
	{
		return (starGraph.begin());
	}
	inline starItr end()
	{
		return (starGraph.end());
	}
	inline unsigned int size()
	{
		return starGraph.size();
	}
	
	void resize(starItr end,starItr begin);
	void clear();
	void print();

};

void StarGraph::add(starElem elem)
{
	starGraph.push_back(elem);
}

inline void StarGraph::resize(starItr end,starItr begin)
{
	starGraph.resize(end-begin);
}
inline void StarGraph::clear()
{
	starGraph.clear();
}
void StarGraph::print()
{
	starItr itr;
	for(itr = starGraph.begin(); itr!=starGraph.end();itr++)
	{
		STXXL_MSG(itr->starEdge.getSrc()<<" "<<itr->starEdge.getDst()<<" "<<itr->starEdge.getEdgeWt());
	}
}
#endif
