/*	Vertex class 
Data members: VertexId and Ri
Size: 128 bytes
Methids: Getters and Setters, Operator == and != */


#ifndef VERTEX_H
#define VERTEX_H
#endif 

class Vertex
{
	unsigned int vertexId;
	unsigned int Ri;
	char data[128 - 2 * sizeof(unsigned int)];

public:	
	Vertex()
	{
		vertexId=0;
		Ri=std::numeric_limits<unsigned int>::max();
	}
	Vertex(unsigned int id)
	{
		vertexId = id;
		Ri=std::numeric_limits<unsigned int>::max();
		
	}
	Vertex(unsigned int id,unsigned int Ri_)
	{
		vertexId = id;
		Ri=Ri_;
		
	}
	bool operator == (const Vertex & b) const {
		return (vertexId == b.vertexId);
	}

	bool operator != (const Vertex & b) const {
		return (vertexId != b.vertexId);
	}

	unsigned int getVertexId() const;
	void setVertexId(unsigned int id);
	unsigned int getRi() const;
	void setRi(unsigned int value);
		
};

unsigned int Vertex::getVertexId() const
{
	return vertexId;
}

void Vertex::setVertexId(unsigned int id)
{
	vertexId = id;
}

unsigned int Vertex::getRi() const
{
	return Ri;
}

void Vertex::setRi(unsigned int value)
{
	Ri = value;
}

