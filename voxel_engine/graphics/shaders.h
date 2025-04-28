#ifndef GRAPHICS_SHADERS_H
#define GRAPHICS_SHADERS_H

#include <string>

class Shader 
{
public:
	Shader(unsigned int id);
	~Shader();
	unsigned int id;
	void use();
};

extern Shader *loadShader(std::string vertexFile, std::string fragmentFile);
#endif

