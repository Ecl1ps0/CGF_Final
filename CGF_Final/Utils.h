#pragma once

#include <GL/glew.h>

#include <iostream>

class Utils
{
public:
	unsigned int compileShader(const std::string& source, unsigned int type);
	void createShaderProgram(unsigned int& shaderProgram, const std::string& VSH, const std::string& FSH);
};

