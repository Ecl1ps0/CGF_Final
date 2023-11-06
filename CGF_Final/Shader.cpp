#include "Shader.h"

Shader::Shader(std::string vsh, const std::string fsh) {
	this->vsh = vsh;
	this->fsh = fsh;
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		std::vector<char> message(lenght);
		glGetShaderInfoLog(id, lenght, &lenght, &message[0]);
		std::cout << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << &message[0] << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::createShaderProgram() {
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vertexShader = compileShader(this->vsh, GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(this->fsh, GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
