#include "mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::vector<float> &buffer, const std::vector<int> &attributes, size_t vertices) : vertices(vertices)
{
	if(attributes.empty()) 
	{
		std::cerr<<"Attributes array cannot be empty"<<std::endl;
	}
	vertices = 0;
	for (int attr : attributes) 
	{
		vertices += attr;
	}
	if (buffer.size() % vertices != 0)
	{
		std::cerr<<"Buffer size is wrong" << std::endl;
	}
	//atributes
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), buffer.data(), GL_STATIC_DRAW);

}

GLuint VAO, VBO;

//position
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
glBindVertexArray(0);