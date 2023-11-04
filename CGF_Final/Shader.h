#pragma once

#include <GL/glew.h>

#include <iostream>

class Shader
{
private:
	const std::string& vsh;
	const std::string& fsh;
public:
	Shader(const std::string& vsh, const std::string& fsh);
	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShaderProgram();
};

