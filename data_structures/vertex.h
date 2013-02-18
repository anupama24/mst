/*	Vertex class 
Data members: VertexId and BlockingValue
Size: 128 bytes
Methids: Getters and Setters, Operator == and != */


#ifndef VERTEX_H
#define VERTEX_H
#endif 

class Vertex
{
	unsigned int vertexId;
	unsigned int blockingValue;
	char data[128 - 2 * sizeof(unsigned int)];

public:	
	Vertex()
	{
		vertexId=0;
		blockingValue=std::numeric_limits<unsigned int>::max();
	}
	Vertex(unsigned int id)
	{
		vertexId = id;
		blockingValue=std::numeric_limits<unsigned int>::max();
		
	}
	bool operator == (const Vertex & b) const {
		return (vertexId == b.vertexId);
	}

	bool operator != (const Vertex & b) const {
		return (vertexId != b.vertexId);
	}

	unsigned int getVertexId() const;
	void setVertexId(unsigned int id);
	unsigned int getBlockingValue() const;
	void setBlockingValue(unsigned int value);
		
};

unsigned int Vertex::getVertexId() const
{
	return vertexId;
}

void Vertex::setVertexId(unsigned int id)
{
	vertexId = id;
}

unsigned int Vertex::getBlockingValue() const
{
	return blockingValue;
}

void Vertex::setBlockingValue(unsigned int value)
{
	blockingValue = value;
}

