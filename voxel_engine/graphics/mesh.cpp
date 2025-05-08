#include "mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::vector<float> &buffer, size_t vertices, const std::vector<int>& attributes) : vertices(vertices)
{
	if (attributes.empty())
	{
		std::cerr << "Attributes array cannot be empty" << std::endl;
	}
	int vertex_size = 0;
	for (int attr : attributes)
	{
		vertex_size += attr;
	}
	//atributes
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices * sizeof(float) * vertex_size, buffer.data(), GL_STATIC_DRAW);

	int offset = 0;
	for (size_t i = 0; i < attributes.size(); i++)
	{
		int components = attributes[i];
		glVertexAttribPointer(i, components, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (GLvoid*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += components;
	}
	glBindVertexArray(0);

}
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
void Mesh::drawPrimitive(unsigned int primitive)
{
	glBindVertexArray(VAO);
	glDrawArrays(primitive, 0, vertices);
	glBindVertexArray(0);
}