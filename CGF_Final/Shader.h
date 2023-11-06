#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>

class Shader
{
private:
	std::string vsh;
	std::string fsh;
public:
	Shader(std::string vsh, const std::string fsh);
	unsigned int compileShader(const std::string& source, unsigned int type);
	unsigned int createShaderProgram();
};

