#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(unsigned int id) : id(id) { }

void Shader::use()
{
	glUseProgram(id);
}
void Shader::uniformMatrix(std::string name, glm::mat4 matrix)
{
	GLuint transformLoc = glGetUniformLocation(id, name.c_str());
	if (transformLoc == -1) 
	{
		std::cerr << "Uniform " << name << " not found" << std::endl;
	}
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
Shader* loadShader(std::string vertexFile, std::string fragmentFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFile.open(vertexFile);
		fragmentShaderFile.open(fragmentFile);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch(std::ifstream::failure &e)
	{
		std::cerr << "Shader file not succesfully read" << std::endl;
		return nullptr;
	}
	const GLchar *vertexShaderCode = vertexCode.c_str();
	const GLchar *fragmentShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << "Shader(vertex) copilation error\n" << infoLog << std::endl;
		return nullptr; 
	}

	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << "Shader(fragment) copilation error\n" << infoLog << std::endl;
		return nullptr;
	}

	//shader program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cerr << "Shader(program) copilation error\n" << infoLog << std::endl;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(id);
		return nullptr;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return new Shader(id);
}
Shader::~Shader()
{
	glDeleteProgram(id);
}