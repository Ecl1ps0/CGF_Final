#include "Shader.h"
#include "Smoke.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

int main() {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Something", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
	}

	int width = 128;
	int height = 128;
	int depth = 64;
	Smoke smoke = Smoke::Smoke(width, height, depth);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	const char* vertexShaderSource = R"(
        #version 330 core

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec2 inTexCoord;

        out vec2 TexCoord;

        void main() {
            TexCoord = inTexCoord;
            gl_Position = vec4(inPosition, 1.0);
        }
    )";

	const char* fragmentShaderSource = R"(
        #version 330 core

        in vec2 TexCoord;
        out vec4 FragColor;

        uniform sampler3D noiseTexture;
        uniform float animationTime;

        void main() {
            vec3 animatedTexCoord = vec3(TexCoord, animationTime * 0.1); // Adjust the scaling factor

            // Adjust the animation to repeat
            animatedTexCoord = fract(animatedTexCoord);

            vec3 noiseColor = texture(noiseTexture, animatedTexCoord).xyz;

            // Apply some adjustments for a smoke-like appearance
            float intensity = noiseColor.r * 0.8 + noiseColor.g * 0.15 + noiseColor.b * 0.05;
            intensity = smoothstep(0.9, 0.2, intensity);

            // Adjust color for a more varied smoke-like appearance
            vec3 smokeColor = mix(vec3(0.8), vec3(0.4), intensity);

            FragColor = vec4(smokeColor, intensity);
        }
    )";

	Shader smokeShader = Shader::Shader(vertexShaderSource, fragmentShaderSource);
	unsigned int smokeShaderProgram = smokeShader.createShaderProgram();

	glUseProgram(smokeShaderProgram);

	unsigned int vao, vbo, fbo, tex, rbo;
	smoke.createQuad(vao, vbo);

	GLint noiseTextureLocation = glGetUniformLocation(smokeShaderProgram, "noiseTexture");
	glUniform1i(noiseTextureLocation, 0);

	GLint animationTimeLocation = glGetUniformLocation(smokeShaderProgram, "animationTime");
	if (animationTimeLocation == -1) {
		std::cerr << "Uniform 'animationTime' not found in the shader program." << std::endl;
	}

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		unsigned int seed = static_cast<unsigned int>(currentTime * 100.0f);

		smoke.generateNoise(seed);

		// Upload the updated noise to the 3D texture
		glBindTexture(GL_TEXTURE_3D, textureID);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, width, height, depth, 0, GL_RED, GL_FLOAT, smoke.noiseData.data());

		// Render to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUniform1f(animationTimeLocation, currentTime);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, textureID);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// Render to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(smokeShaderProgram);
		glUniform1f(animationTimeLocation, currentTime);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glDeleteTextures(1, &textureID);
	glDeleteTextures(1, &tex);

	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);

	glDeleteProgram(smokeShaderProgram);

	glfwTerminate();
	return 0;
}