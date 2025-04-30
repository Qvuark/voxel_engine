#ifndef GRAPHICS_SHADERS_H
#define GRAPHICS_SHADERS_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

class Shader 
{
public:
	Shader(unsigned int id);
	~Shader();
	unsigned int id;
	void use();
	void uniformMatrix(std::string name, glm::mat4 matrix);
};

extern Shader *loadShader(std::string vertexFile, std::string fragmentFile);
#endif

