#include "Smoke.h"

Smoke::Smoke(int width, int height, int depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;

    this->noiseData = std::vector<float>(width * height * depth);
}

void Smoke::generateNoise(unsigned int seed) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    noise.SetSeed(seed);

    for (int z = 0; z < this->depth; z++) {
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < this->width; x++) {
                float value = noise.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                value = (value + 1.0f) * 0.5f;
                value = pow(value, 1.5f);  // Adjust the power for a flame-like effect
                value *= 2.0f;

                this->noiseData[x + y * this->width + z * this->width * this->height] = value;
            }
        }
    }
}

void Smoke::createQuad(unsigned int& vao, unsigned int& vbo) {
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
