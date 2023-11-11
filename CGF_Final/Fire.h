#pragma once

#include "FastNoise.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Fire
{
private:
	int width;
	int height;
	int depth;

public:
	std::vector<float> noiseData;

	Fire(int width, int height, int depth);
	void generateNoise();
	void createQuad(unsigned int& vao, unsigned int& vbo);
};

