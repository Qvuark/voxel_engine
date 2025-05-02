
#ifndef POLYGON_MESH_H
#define POLYGON_MESH_H
#include <vector>
#include <iostream>

class Mesh
{
public: 
	Mesh(const std::vector<float>& buffer, const std::vector<int>& attributes, size_t vertices);
	~Mesh();
	void drawPrimitive(unsigned int graphPrimitive);

	unsigned int VBO, VAO;
	size_t vertices;
};

#endif