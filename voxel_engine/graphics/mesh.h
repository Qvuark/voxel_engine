
#ifndef POLYGON_MESH_H
#define POLYGON_MESH_H
#include <vector>
#include <iostream>

class Mesh
{
public:
	Mesh(const std::vector<float>& buffer, size_t vertices, const std::vector<int>& attributes);
	~Mesh();
	void drawPrimitive(unsigned int graphPrimitive);

	unsigned int VBO, VAO;
	size_t vertices;
};

#endif