#pragma once

#include <glm/glm.hpp>

#include <math.h>

#include <vector>

#define M_PI 3.14159265358979323846

class Sphere
{
private:
	float radius = 1.f;
	int sectorCount = 100;
	int stackCount = 100;

public: 
	Sphere(float radius, int sectorCount, int stackCount);

	void createSphere(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords);
	void applyShadersToSphere();
};

