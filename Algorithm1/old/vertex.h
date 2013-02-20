/*	Vertex class 
Data members: VertexId and BlockingValue
Size: 128 bytes
Methids: Getters and Setters, Operator == and != */

class Vertex
{
	unsigned int vertexId;
	unsigned int blockingValue;
	char data[128 - 2 * sizeof(unsigned int)];

public:	
	Vertex()
	{
		vertex_id=0;
		blocking_value=std::numeric_limits<unsigned int>::max();
	}
	Vertex(unsigned int id)
	{
		vertex_id = id;
		blocking_value=std::numeric_limits<unsigned int>::max();
		
	}
	bool operator == (const vertex & b) const {
		return (vertex_id == b.vertex_id);
	}

	bool operator != (const vertex & b) const {
		return (vertex_id != b.vertex_id);
	}

	unsigned int getVertexId();
	void setVertexId(unsigned int id);
	unsigned int getBlockingValue();
	void setBlockingValue(unsigned int value);
		
};

unsigned int Vertex::getVertexId()
{
	return vertexId;
}

void Vertex::setVertexId(unsigned int id)
{
	vertexId = id;
}

unsigned int Vertex::getBlockingValue()
{
	return blockingValue;
}

void Vertex::setBlockingValue(unsigned int value)
{
	blockingValue = value;
}

