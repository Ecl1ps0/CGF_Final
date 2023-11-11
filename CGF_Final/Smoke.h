#pragma once

#include "FastNoise.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Smoke
{
private:
	int width;
	int height;
    int depth;

public:
	std::vector<float> noiseData;

	Smoke(int width, int height, int depth);
	void generateNoise(unsigned int seed);
	void createQuad(unsigned int& vao, unsigned int& vbo);
};

