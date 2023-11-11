#include "Shader.h"
#include "Smoke.h"
#include "Fire.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

enum EffectType {
	SMOKE,
	FIRE
};

EffectType currentEffect = SMOKE;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE:
			currentEffect = (currentEffect == SMOKE) ? FIRE : SMOKE;
			break;
		}
	}
}

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
	Fire fire = Fire::Fire(width, height, depth);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	const char* smokeVSH = R"(
        #version 330 core

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec2 inTexCoord;

        out vec2 TexCoord;

        void main() {
            TexCoord = inTexCoord;
            gl_Position = vec4(inPosition, 1.0);
        }
    )";

	const char* smokeFSH = R"(
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

	const char* fireVSH = R"(
        #version 330 core

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec2 inTexCoord;

        out vec2 TexCoord;

        void main() {
            TexCoord = inTexCoord;
            gl_Position = vec4(inPosition, 1.0);
        }
    )";

	const char* fireFSH = R"(
        #version 330 core

        in vec2 TexCoord;
        out vec4 FragColor;

        uniform sampler3D noiseTexture;
        uniform float animationTime;

        const vec3 flameSource = vec3(0.5, 0.5, 0.5);

        void main() {
            vec3 animatedTexCoord = vec3(TexCoord, animationTime * 0.15); // Adjust the multiplier for faster animation
        
            // Adjust the animation to repeat
            animatedTexCoord.x = fract(animatedTexCoord.x);
            animatedTexCoord.y = fract(animatedTexCoord.y);
            animatedTexCoord.z = fract(animatedTexCoord.z);

            vec3 noiseColor = texture(noiseTexture, animatedTexCoord).xyz;

            float distance = distance(flameSource, animatedTexCoord);
            float intensity = 1.0 - smoothstep(0.0, 0.5, distance);

            noiseColor *= vec3(1.0, 0.5, 0.2); // Adjust color for flame-like appearance
            noiseColor = mix(vec3(0.0), noiseColor, intensity);

            FragColor = vec4(noiseColor, intensity);
        }
    )";

	Shader smokeShader = Shader::Shader(smokeVSH, smokeFSH);
	unsigned int smokeShaderProgram = smokeShader.createShaderProgram();

	Shader fireShader = Shader::Shader(fireVSH, fireFSH);
	unsigned int fireShaderProgram = fireShader.createShaderProgram();

	unsigned int smokeVAO, smokeVBO, smokeFBO, smokeTEX, smokeRBO;
	unsigned int fireVAO, fireVBO, fireFBO, fireTEX, fireRBO;

	unsigned int smokeNoiseTextureLocation, smokeAnimationTimeLocation;
	unsigned int fireNoiseTextureLocation, fireAnimationTimeLocation;

	glUseProgram(smokeShaderProgram);
	smoke.createQuad(smokeVAO, smokeVBO);

	smokeNoiseTextureLocation = glGetUniformLocation(smokeShaderProgram, "noiseTexture");
	glUniform1i(smokeNoiseTextureLocation, 0);

	smokeAnimationTimeLocation = glGetUniformLocation(smokeShaderProgram, "animationTime");
	if (smokeAnimationTimeLocation == -1) {
		std::cerr << "Uniform 'animationTime' not found in the shader program." << std::endl;
	}

	glGenFramebuffers(1, &smokeFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, smokeFBO);

	glGenTextures(1, &smokeTEX);
	glBindTexture(GL_TEXTURE_2D, smokeTEX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, smokeTEX, 0);

	glGenRenderbuffers(1, &smokeRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, smokeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, smokeRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(fireShaderProgram);
	fire.createQuad(fireVAO, fireVBO);

	fireNoiseTextureLocation = glGetUniformLocation(fireShaderProgram, "noiseTexture");
	glUniform1i(fireNoiseTextureLocation, 0);

	fireAnimationTimeLocation = glGetUniformLocation(fireShaderProgram, "animationTime");
	if (fireAnimationTimeLocation == -1) {
		std::cerr << "Uniform 'animationTime' not found in the shader program." << std::endl;
	}

	glGenFramebuffers(1, &fireFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, fireFBO);

	glGenTextures(1, &fireTEX);
	glBindTexture(GL_TEXTURE_2D, fireTEX);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fireTEX, 0);

	glGenRenderbuffers(1, &fireRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, fireRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fireRBO);

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

		if (currentEffect == SMOKE) {
			smoke.generateNoise(seed);

			// Upload the updated noise to the 3D texture
			glBindTexture(GL_TEXTURE_3D, textureID);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, width, height, depth, 0, GL_RED, GL_FLOAT, smoke.noiseData.data());

			// Render to framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, smokeFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUniform1f(smokeAnimationTimeLocation, currentTime);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, textureID);
			glBindVertexArray(smokeVAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			// Render to screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUseProgram(smokeShaderProgram);
			glUniform1f(smokeAnimationTimeLocation, currentTime);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, smokeTEX);
			glBindVertexArray(smokeVAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		else {
			fire.generateNoise();

			// Upload the updated noise to the 3D texture
			glBindTexture(GL_TEXTURE_3D, textureID);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, width, height, depth, 0, GL_RED, GL_FLOAT, fire.noiseData.data());

			// Render to framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, fireFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUniform1f(fireAnimationTimeLocation, currentTime);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, textureID);
			glBindVertexArray(fireVAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			// Render to screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUseProgram(fireShaderProgram);
			glUniform1f(fireAnimationTimeLocation, currentTime);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fireTEX);
			glBindVertexArray(fireVAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}

		glfwSetKeyCallback(window, key_callback);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &smokeVAO);
	glDeleteBuffers(1, &smokeVBO);

	glDeleteTextures(1, &textureID);
	glDeleteTextures(1, &smokeTEX);

	glDeleteRenderbuffers(1, &smokeRBO);
	glDeleteFramebuffers(1, &smokeFBO);

	glDeleteProgram(smokeShaderProgram);

	glfwTerminate();
	return 0;
}
